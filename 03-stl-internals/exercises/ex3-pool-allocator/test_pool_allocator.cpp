#include <cstdio>
#include <string>
#include <vector>
#include "common.hpp"
#include "pool_allocator.hpp"

// Helper: pool vector types
using PoolVecInt    = std::vector<int,         PoolAllocator<int>>;
using PoolVecString = std::vector<std::string, PoolAllocator<std::string>>;

// ─── Correctness tests ───────────────────────────────────────────────────────

void test_int_push_back() {
    pool_detail::reset();
    PoolVecInt v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    ASSERT_EQ(3, (int)v.size());
    ASSERT_EQ(1, v[0]);
    ASSERT_EQ(2, v[1]);
    ASSERT_EQ(3, v[2]);
}

void test_int_values_survive_realloc() {
    pool_detail::reset();
    PoolVecInt v;
    const int N = 512;
    for (int i = 0; i < N; ++i) v.push_back(i * 3);
    ASSERT_EQ(N, (int)v.size());
    for (int i = 0; i < N; ++i) ASSERT_EQ(i * 3, v[i]);
}

void test_string_push_back() {
    pool_detail::reset();
    PoolVecString v;
    v.push_back("hello");
    v.push_back("world");
    v.push_back("hft");
    ASSERT_EQ(3, (int)v.size());
    ASSERT_STR_EQ("hello", v[0]);
    ASSERT_STR_EQ("world", v[1]);
    ASSERT_STR_EQ("hft",   v[2]);
}

void test_clear_and_repopulate() {
    pool_detail::reset();
    PoolVecInt v;
    for (int i = 0; i < 16; ++i) v.push_back(i);
    v.clear();
    ASSERT_EQ(0, (int)v.size());
    v.push_back(99);
    ASSERT_EQ(1, (int)v.size());
    ASSERT_EQ(99, v[0]);
}

void test_pool_reset_between_uses() {
    pool_detail::reset();
    {
        PoolVecInt a;
        for (int i = 0; i < 64; ++i) a.push_back(i);
        ASSERT_EQ(64, (int)a.size());
    }
    pool_detail::reset();
    {
        PoolVecInt b;
        for (int i = 100; i < 164; ++i) b.push_back(i);
        ASSERT_EQ(64, (int)b.size());
        ASSERT_EQ(100, b[0]);
        ASSERT_EQ(163, b[63]);
    }
}

void test_large_reserve() {
    pool_detail::reset();
    PoolVecInt v;
    v.reserve(10000);
    for (int i = 0; i < 10000; ++i) v.push_back(i);
    ASSERT_EQ(10000, (int)v.size());
    ASSERT_EQ(0,    v[0]);
    ASSERT_EQ(9999, v[9999]);
}

void test_alignment_not_null() {
    // Verify allocate never returns nullptr
    pool_detail::reset();
    PoolAllocator<int> alloc;
    int* p = alloc.allocate(1);
    ASSERT_NOT_NULL(p);
    alloc.deallocate(p, 1);
}

void test_rebind() {
    // rebind is used internally by std::list etc.
    using ReboundAlloc = PoolAllocator<int>::rebind<double>::other;
    ReboundAlloc ra;
    pool_detail::reset();
    double* dp = ra.allocate(4);
    ASSERT_NOT_NULL(dp);
    ra.deallocate(dp, 4);
}

// ─── Benchmarks ──────────────────────────────────────────────────────────────

void bench_vector_push_back() {
    const int N = 100'000;

    // ── Default allocator ────────────────────────────────────────────────────
    std::vector<int> def_v;
    def_v.reserve(0);  // no pre-reserve; let it grow naturally
    auto t1 = bench::now();
    for (int i = 0; i < N; ++i) def_v.push_back(i);
    bench::do_not_optimize(def_v[0]);
    long long def_ns = bench::elapsed_ns(t1);

    // ── Pool allocator ───────────────────────────────────────────────────────
    pool_detail::reset();
    PoolVecInt pool_v;
    auto t2 = bench::now();
    for (int i = 0; i < N; ++i) pool_v.push_back(i);
    bench::do_not_optimize(pool_v[0]);
    long long pool_ns = bench::elapsed_ns(t2);

    std::printf("\n── push_back throughput (N=%d, no reserve) ──\n", N);
    bench::print_result("vector<int>  (default allocator)", (double)def_ns  / N, N);
    bench::print_result("vector<int>  (pool allocator)",   (double)pool_ns / N, N);
    if (pool_ns > 0)
        std::printf("  Ratio (pool/default): %.2fx  %s\n",
            (double)pool_ns / def_ns,
            pool_ns < def_ns ? "(pool faster)" : "(default faster — is 64B alignment correct?)");
}

void bench_alloc_latency_distribution() {
    const int K = 10'000;

    // ── Default allocator individual alloc latency ───────────────────────────
    std::vector<long long> def_samples, pool_samples;
    def_samples.reserve(K);
    pool_samples.reserve(K);

    for (int i = 0; i < K; ++i) {
        auto t = bench::now();
        int* p = static_cast<int*>(::operator new(64));
        def_samples.push_back(bench::elapsed_ns(t));
        ::operator delete(p);
    }

    pool_detail::reset();
    PoolAllocator<char> pa;
    for (int i = 0; i < K; ++i) {
        auto t = bench::now();
        char* p = pa.allocate(64);
        pool_samples.push_back(bench::elapsed_ns(t));
        (void)p;
    }

    std::printf("\n── Single-alloc latency distribution (%d samples) ──\n", K);
    bench::print_latency_dist("operator new (64B)", def_samples);
    bench::print_latency_dist("pool alloc   (64B)", pool_samples);
}

// ─── main ────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    RUN_TEST(test_int_push_back);
    RUN_TEST(test_int_values_survive_realloc);
    RUN_TEST(test_string_push_back);
    RUN_TEST(test_clear_and_repopulate);
    RUN_TEST(test_pool_reset_between_uses);
    RUN_TEST(test_large_reserve);
    RUN_TEST(test_alignment_not_null);
    RUN_TEST(test_rebind);

    bench_vector_push_back();
    bench_alloc_latency_distribution();

    TEST_END();
}
