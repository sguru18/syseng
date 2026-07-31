#include <common.hpp>
#include <cstdint>
#include <cstring>
#include "buffer.hpp"

// ── Correctness ───────────────────────────────────────────────────────────────

void test_basic_allocation() {
    Buffer b(64);
    ASSERT_NOT_NULL(b.data());
    ASSERT_EQ(64, (int)b.size());
    // Constructor must zero the bytes
    for (size_t i = 0; i < b.size(); ++i)
        ASSERT_EQ(0, (int)b[i]);
}

void test_copy_independence() {
    Buffer src(32);
    ASSERT_NOT_NULL(src.data());
    ASSERT_EQ(32, (int)src.size());
    for (size_t i = 0; i < src.size(); ++i) src[i] = static_cast<uint8_t>(i);

    Buffer dst = src; // copy constructor

    // Deep copy: different allocations
    ASSERT_TRUE(dst.data() != src.data());
    ASSERT_EQ((int)src.size(), (int)dst.size());

    // Modify dst -- src must be unaffected
    dst[0] = 0xFF;
    ASSERT_EQ(0x00, (int)src[0]);
    ASSERT_EQ(0xFF, (int)dst[0]);
}

void test_copy_count() {
    Buffer::reset_counters();
    Buffer src(64);
    Buffer dst = src; // one copy
    ASSERT_EQ(1, Buffer::copy_count());
    ASSERT_EQ(0, Buffer::move_count());
}

void test_move_transfers_ownership() {
    Buffer src(128);
    ASSERT_NOT_NULL(src.data());
    src[0] = 42;
    uint8_t* original_ptr = src.data();

    Buffer dst = std::move(src);

    ASSERT_NULL(src.data());           // source must be cleared
    ASSERT_EQ(0, (int)src.size());
    ASSERT_EQ(original_ptr, dst.data()); // same allocation, no copy
    ASSERT_EQ(128, (int)dst.size());
    ASSERT_EQ(42, (int)dst[0]);
}

void test_move_count() {
    Buffer::reset_counters();
    Buffer src(64);
    Buffer dst = std::move(src); // one move
    ASSERT_EQ(1, Buffer::move_count());
    ASSERT_EQ(0, Buffer::copy_count());
}

void test_copy_assign() {
    Buffer::reset_counters();
    Buffer a(16), b(32);
    b = a; // copy assign
    ASSERT_EQ((int)a.size(), (int)b.size());
    ASSERT_TRUE(b.data() != a.data());
    ASSERT_EQ(1, Buffer::copy_count());
}

void test_move_assign() {
    Buffer::reset_counters();
    Buffer a(16), b(32);
    uint8_t* ptr_a = a.data();
    b = std::move(a); // move assign
    ASSERT_EQ(ptr_a, b.data());  // b now owns a's original allocation
    ASSERT_NULL(a.data());
    ASSERT_EQ(1, Buffer::move_count());
}

// ── NRVO / RVO observation ────────────────────────────────────────────────────

static Buffer make_named_buffer(size_t n) {
    Buffer b(n); // named local -- NRVO candidate
    return b;
}

void test_nrvo_no_copy() {
    Buffer::reset_counters();
    Buffer b = make_named_buffer(256);
    ASSERT_NOT_NULL(b.data());
    ASSERT_EQ(256, (int)b.size());
    // With NRVO, neither copy nor move fires.
    // Without NRVO (pessimistic), at most one move fires. Either way: 0 copies.
    ASSERT_EQ(0, Buffer::copy_count());
}

// ── make_buffer (perfect forwarding) ─────────────────────────────────────────

void test_make_buffer() {
    Buffer::reset_counters();
    Buffer b = make_buffer(512);
    ASSERT_NOT_NULL(b.data());
    ASSERT_EQ(512, (int)b.size());
    ASSERT_EQ(0, Buffer::copy_count()); // forwarding must not introduce copies
}

// ── Benchmarks ────────────────────────────────────────────────────────────────

void bench_copy_vs_move() {
    const size_t N = 1024 * 1024; // 1 MB
    Buffer src(N);
    if (!src.data()) {
        std::printf("  [skip] Buffer not implemented -- allocate in constructor first\n");
        return;
    }
    for (size_t i = 0; i < N; ++i) src[i] = static_cast<uint8_t>(i & 0xFF);

    // Copy benchmark: each iteration does a full memcpy of N bytes
    double copy_ns = bench::median_ns([&]() {
        Buffer tmp = src;
        bench::do_not_optimize(tmp.data());
    }, 1000);

    // Move benchmark: each iteration is a pointer swap -- O(1)
    double move_ns = bench::median_ns([&]() {
        Buffer a(N);
        Buffer b = std::move(a);
        bench::do_not_optimize(b.data());
    }, 1000);

    bench::print_result("copy 1MB buffer", copy_ns, 1000);
    bench::print_result("move 1MB buffer", move_ns, 1000);

    if (move_ns > 0.5) {
        std::printf("  ratio (copy/move): %.0fx  -- expect ~1000x\n",
            copy_ns / move_ns);
    }
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    std::printf(BOLD "\n── Correctness ───────────────────────────────────\n" RESET);
    RUN_TEST(test_basic_allocation);
    RUN_TEST(test_copy_independence);
    RUN_TEST(test_copy_count);
    RUN_TEST(test_move_transfers_ownership);
    RUN_TEST(test_move_count);
    RUN_TEST(test_copy_assign);
    RUN_TEST(test_move_assign);

    std::printf(BOLD "\n── NRVO & Factories ──────────────────────────────\n" RESET);
    RUN_TEST(test_nrvo_no_copy);
    RUN_TEST(test_make_buffer);

    std::printf(BOLD "\n── Benchmarks ────────────────────────────────────\n" RESET);
    RUN_TEST(bench_copy_vs_move);

    TEST_END();
}
