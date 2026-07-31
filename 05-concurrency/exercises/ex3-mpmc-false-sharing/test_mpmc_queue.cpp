#include <atomic>
#include <array>
#include <optional>
#include <stdexcept>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdint>
#include <cstddef>
#include "common.hpp"
#include "mpmc_queue.hpp"

// ─── Single-threaded correctness ──────────────────────────────────────────

static void test_push_pop_basic() {
    MPMCQueue<int, 16> q;
    q.push(1);
    q.push(2);
    q.push(3);

    int v = 0;
    q.pop(v); ASSERT_EQ(1, v);
    q.pop(v); ASSERT_EQ(2, v);
    q.pop(v); ASSERT_EQ(3, v);
}

static void test_try_push_try_pop() {
    MPMCQueue<int, 4> q;

    // Fill all 3 available slots (capacity = N-1 = 3)
    ASSERT_TRUE(q.try_push(10));
    ASSERT_TRUE(q.try_push(20));
    ASSERT_TRUE(q.try_push(30));
    ASSERT_FALSE(q.try_push(40));   // full

    int v = 0;
    ASSERT_TRUE(q.try_pop(v)); ASSERT_EQ(10, v);
    ASSERT_TRUE(q.try_pop(v)); ASSERT_EQ(20, v);
    ASSERT_TRUE(q.try_pop(v)); ASSERT_EQ(30, v);
    ASSERT_FALSE(q.try_pop(v));    // empty
}

// ─── Multi-threaded correctness: 4P + 4C, 4M total items ──────────────────

static void test_mpmc_4p4c_correctness() {
    constexpr int N_PRODUCERS  = 4;
    constexpr int N_CONSUMERS  = 4;
    constexpr int ITEMS_EACH   = 250'000;             // 4 * 250k = 1M per producer group
    constexpr long long TOTAL  = (long long)N_PRODUCERS * ITEMS_EACH;
    constexpr std::size_t QSZ  = 1 << 14;

    MPMCQueue<int, QSZ> q;

    // Track how many times each value appeared
    std::vector<std::atomic<int>> seen(TOTAL);
    for (auto& a : seen) a.store(0);

    std::atomic<long long> consumed{0};

    auto producer = [&](int id) {
        for (int i = 0; i < ITEMS_EACH; ++i) {
            int val = id * ITEMS_EACH + i;
            q.push(val);
        }
    };

    auto consumer = [&]() {
        long long local = 0;
        while (true) {
            int v = 0;
            if (q.try_pop(v)) {
                seen[v].fetch_add(1, std::memory_order_relaxed);
                ++local;
            }
            long long already = consumed.load(std::memory_order_relaxed);
            if (already + local >= TOTAL) {
                consumed.fetch_add(local, std::memory_order_relaxed);
                break;
            }
        }
        consumed.fetch_add(local, std::memory_order_relaxed);
    };

    std::vector<std::thread> producers, consumers;
    for (int i = 0; i < N_PRODUCERS; ++i) producers.emplace_back(producer, i);
    for (int i = 0; i < N_CONSUMERS; ++i) consumers.emplace_back(consumer);

    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();

    // Drain any remaining items
    int leftover = 0;
    while (true) {
        int v = 0;
        if (!q.try_pop(v)) break;
        seen[v].fetch_add(1, std::memory_order_relaxed);
        ++leftover;
    }

    // Every value seen exactly once
    long long bad = 0;
    for (long long i = 0; i < TOTAL; ++i) {
        if (seen[i].load() != 1) ++bad;
    }
    ASSERT_EQ(0LL, bad);
}

// ─── Throughput benchmark: 4P/4C vs 1P/1C ─────────────────────────────────

static void bench_mpmc_throughput() {
    constexpr long long TOTAL  = 4'000'000LL;
    constexpr std::size_t QSIZ = 1 << 14;

    // 4P / 4C
    {
        MPMCQueue<int, QSIZ> q;
        std::atomic<long long> produced{0}, consumed{0};
        auto t = bench::now();

        auto producer = [&]() {
            while (true) {
                long long pos = produced.fetch_add(1, std::memory_order_relaxed);
                if (pos >= TOTAL) break;
                q.push((int)pos);
            }
        };
        auto consumer = [&]() {
            int v = 0;
            while (consumed.load(std::memory_order_relaxed) < TOTAL) {
                if (q.try_pop(v))
                    consumed.fetch_add(1, std::memory_order_relaxed);
            }
        };

        std::vector<std::thread> ps, cs;
        for (int i = 0; i < 4; ++i) ps.emplace_back(producer);
        for (int i = 0; i < 4; ++i) cs.emplace_back(consumer);
        for (auto& th : ps) th.join();
        for (auto& th : cs) th.join();

        double ns = (double)bench::elapsed_ns(t);
        bench::print_throughput("MPMC 4P/4C (4M items)", TOTAL, ns);
    }

    // 1P / 1C (SPSC-like through MPMC)
    {
        MPMCQueue<int, QSIZ> q;
        std::atomic<bool> done{false};
        long long cnt = 0;
        auto t = bench::now();

        std::thread consumer([&]() {
            int v = 0;
            while (cnt < TOTAL) {
                if (q.try_pop(v)) ++cnt;
            }
            done.store(true);
        });

        for (long long i = 0; i < TOTAL; ++i) q.push((int)i);
        while (!done.load()) {}
        consumer.join();

        double ns = (double)bench::elapsed_ns(t);
        bench::print_throughput("MPMC 1P/1C (4M items)", TOTAL, ns);
    }
}

// ─── False sharing benchmark ───────────────────────────────────────────────

static void bench_false_sharing() {
    constexpr std::size_t ITERS = 50'000'000ULL;

    long long bad_ns  = benchmark_false_sharing(ITERS);
    long long good_ns = benchmark_no_false_sharing(ITERS);

    double bad_per  = (double)bad_ns  / ITERS;
    double good_per = (double)good_ns / ITERS;
    double speedup  = bad_per / good_per;

    std::printf(CYAN "  bench" RESET
        " false sharing: %.2f ns/iter,  no false sharing: %.2f ns/iter,  speedup: %.1fx\n",
        bad_per, good_per, speedup);

    ASSERT_TRUE(speedup > 1.5);   // should be measurably better
}

// ─── Main ──────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    RUN_TEST(test_push_pop_basic);
    RUN_TEST(test_try_push_try_pop);
    RUN_TEST(test_mpmc_4p4c_correctness);

    std::printf("\n--- Benchmark ---\n");
    bench_mpmc_throughput();
    bench_false_sharing();

    TEST_END();
}
