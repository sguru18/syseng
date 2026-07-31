#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include "common.hpp"
#include "hot_alloc.hpp"

// ─── Types used in tests and benchmarks ──────────────────────────────────────

struct alignas(64) Block64 {
    char data[64];
};

struct Point3D {
    float x, y, z;
    explicit Point3D(float x, float y, float z) : x(x), y(y), z(z) {}
};

// ─── Arena correctness tests ──────────────────────────────────────────────────

void test_arena_basic_alloc() {
    Arena arena(4096);
    void* p = arena.allocate(64);
    ASSERT_NOT_NULL(p);
    ASSERT_TRUE(arena.used() >= 64);
    ASSERT_TRUE(arena.capacity() == 4096);
}

void test_arena_alignment() {
    Arena arena(4096);

    void* p1 = arena.allocate(1, 1);
    ASSERT_NOT_NULL(p1);

    void* p64 = arena.allocate(64, 64);
    ASSERT_NOT_NULL(p64);
    ASSERT_TRUE((reinterpret_cast<uintptr_t>(p64) % 64) == 0);

    void* p16 = arena.allocate(32, 16);
    ASSERT_NOT_NULL(p16);
    ASSERT_TRUE((reinterpret_cast<uintptr_t>(p16) % 16) == 0);
}

void test_arena_overflow() {
    Arena arena(128);
    void* p = arena.allocate(128);
    ASSERT_NOT_NULL(p);
    void* overflow = arena.allocate(1);
    ASSERT_NULL(overflow);
}

void test_arena_reset() {
    Arena arena(256);
    void* p1 = arena.allocate(128);
    ASSERT_NOT_NULL(p1);
    arena.reset();
    ASSERT_EQ(0, (long long)arena.used());
    void* p2 = arena.allocate(128);
    ASSERT_NOT_NULL(p2);
    // After reset the arena should hand out the same base region again
    ASSERT_EQ((long long)arena.used(), (long long)128);
}

void test_arena_emplace() {
    Arena arena(1024);
    Point3D* pt = arena.emplace<Point3D>(1.0f, 2.0f, 3.0f);
    ASSERT_NOT_NULL(pt);
    ASSERT_EQ_F(1.0f, pt->x, 1e-6);
    ASSERT_EQ_F(2.0f, pt->y, 1e-6);
    ASSERT_EQ_F(3.0f, pt->z, 1e-6);
}

void test_arena_emplace_multiple() {
    Arena arena(4096);
    const int N = 10;
    Point3D* pts[N];
    for (int i = 0; i < N; ++i) {
        pts[i] = arena.emplace<Point3D>(
            static_cast<float>(i),
            static_cast<float>(i * 2),
            static_cast<float>(i * 3)
        );
        ASSERT_NOT_NULL(pts[i]);
    }
    for (int i = 0; i < N; ++i) {
        ASSERT_EQ_F(static_cast<float>(i),     pts[i]->x, 1e-5);
        ASSERT_EQ_F(static_cast<float>(i * 2), pts[i]->y, 1e-5);
    }
}

// ─── ObjectPool correctness tests ─────────────────────────────────────────────

void test_pool_basic_alloc() {
    ObjectPool<Block64> pool(4);
    ASSERT_EQ(4, (long long)pool.capacity());
    ASSERT_EQ(4, (long long)pool.available());

    Block64* p = pool.allocate();
    ASSERT_NOT_NULL(p);
    ASSERT_EQ(3, (long long)pool.available());
}

void test_pool_exhaustion() {
    ObjectPool<Block64> pool(2);
    Block64* p1 = pool.allocate();
    Block64* p2 = pool.allocate();
    ASSERT_NOT_NULL(p1);
    ASSERT_NOT_NULL(p2);
    ASSERT_EQ(0, (long long)pool.available());
    Block64* p3 = pool.allocate();
    ASSERT_NULL(p3);
}

void test_pool_deallocate() {
    ObjectPool<Block64> pool(1);
    Block64* p = pool.allocate();
    ASSERT_NOT_NULL(p);
    ASSERT_EQ(0, (long long)pool.available());
    pool.deallocate(p);
    ASSERT_EQ(1, (long long)pool.available());
    Block64* p2 = pool.allocate();
    ASSERT_NOT_NULL(p2);
}

void test_pool_lifo_order() {
    // The free list is LIFO: last freed = first allocated.
    ObjectPool<Block64> pool(3);
    Block64* a = pool.allocate();
    Block64* b = pool.allocate();
    Block64* c = pool.allocate();
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(b);
    ASSERT_NOT_NULL(c);

    // Free in order a, b, c. Next alloc should return c (LIFO head).
    pool.deallocate(a);
    pool.deallocate(b);
    pool.deallocate(c);
    ASSERT_EQ(3, (long long)pool.available());

    Block64* first = pool.allocate();
    ASSERT_EQ((long long)(uintptr_t)c, (long long)(uintptr_t)first);
}

void test_pool_all_slots_unique() {
    const size_t N = 8;
    ObjectPool<Block64> pool(N);
    Block64* ptrs[N];
    for (size_t i = 0; i < N; ++i) {
        ptrs[i] = pool.allocate();
        ASSERT_NOT_NULL(ptrs[i]);
    }
    // All pointers must be distinct
    for (size_t i = 0; i < N; ++i)
        for (size_t j = i + 1; j < N; ++j)
            ASSERT_TRUE(ptrs[i] != ptrs[j]);
}

// ─── Benchmarks ───────────────────────────────────────────────────────────────

void run_benchmarks() {
    const size_t N = 1'000'000;

    printf("\n=== Benchmark: %zu individual 64-byte alloc+free cycles ===\n\n", N);

    // ── new / delete ──────────────────────────────────────────────────────────
    {
        std::vector<long long> samples;
        samples.reserve(N);
        for (size_t i = 0; i < N; ++i) {
            auto t = bench::now();
            auto* p = new Block64;
            bench::do_not_optimize(p);
            delete p;
            samples.push_back(bench::elapsed_ns(t));
        }
        bench::print_latency_dist("new/delete", samples);
    }

    // ── Arena ─────────────────────────────────────────────────────────────────
    // Each round: allocate sizeof(Block64) bytes, measure, reset every 8192 allocs
    // so the arena never overflows and the bump pointer stays warm in cache.
    {
        const size_t arena_capacity = sizeof(Block64) * 8192 + 4096;
        Arena arena(arena_capacity);
        std::vector<long long> samples;
        samples.reserve(N);
        for (size_t i = 0; i < N; ++i) {
            if (i % 8192 == 0) arena.reset();
            auto t = bench::now();
            void* p = arena.allocate(sizeof(Block64), alignof(Block64));
            bench::do_not_optimize(p);
            samples.push_back(bench::elapsed_ns(t));
        }
        bench::print_latency_dist("Arena", samples);
    }

    // ── ObjectPool ────────────────────────────────────────────────────────────
    {
        ObjectPool<Block64> pool(1);
        std::vector<long long> samples;
        samples.reserve(N);
        for (size_t i = 0; i < N; ++i) {
            auto t = bench::now();
            Block64* p = pool.allocate();
            bench::do_not_optimize(p);
            pool.deallocate(p);
            samples.push_back(bench::elapsed_ns(t));
        }
        bench::print_latency_dist("ObjectPool", samples);
    }

    printf("\n(p99 and p99.9 gaps are why arenas and pools exist)\n");
}

// ─── main ─────────────────────────────────────────────────────────────────────

int main() {
    printf("=== Hot Path Allocation: Correctness + Benchmarks ===\n\n");
    printf("── Correctness tests ────────────────────────────────\n");

    TEST_BEGIN();

    RUN_TEST(test_arena_basic_alloc);
    RUN_TEST(test_arena_alignment);
    RUN_TEST(test_arena_overflow);
    RUN_TEST(test_arena_reset);
    RUN_TEST(test_arena_emplace);
    RUN_TEST(test_arena_emplace_multiple);

    RUN_TEST(test_pool_basic_alloc);
    RUN_TEST(test_pool_exhaustion);
    RUN_TEST(test_pool_deallocate);
    RUN_TEST(test_pool_lifo_order);
    RUN_TEST(test_pool_all_slots_unique);

    // Print test summary; capture exit code manually so we can still run benchmarks.
    int result = (_tests_failed == 0) ? 0 : 1;
    std::printf("\n%s%d/%d tests passed%s\n",
        (_tests_failed == 0) ? GREEN BOLD : RED BOLD,
        _tests_passed, _tests_run, RESET);

    if (result != 0) {
        printf("\nFix the correctness failures above before trusting the benchmarks.\n");
        return result;
    }

    run_benchmarks();
    return 0;
}
