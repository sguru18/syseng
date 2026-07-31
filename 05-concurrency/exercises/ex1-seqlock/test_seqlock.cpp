#include <atomic>
#include <cstdint>
#include <thread>
#include <chrono>
#include <functional>
#include <shared_mutex>
#include <vector>
#include "common.hpp"
#include "seqlock.hpp"

// ─── Data types used in tests ──────────────────────────────────────────────

struct Triple {
    int a, b, c;
};

// ─── Single-threaded correctness ───────────────────────────────────────────

static void test_single_write_read() {
    SeqLock<int> sl;
    sl.write(42);
    ASSERT_EQ(42, sl.read());
}

static void test_multiple_writes() {
    SeqLock<int> sl;
    for (int i = 0; i < 100; ++i) sl.write(i);
    ASSERT_EQ(99, sl.read());
}

static void test_struct_write_read() {
    SeqLock<Triple> sl;
    sl.write({10, 20, 30});
    Triple v = sl.read();
    ASSERT_EQ(10, v.a);
    ASSERT_EQ(20, v.b);
    ASSERT_EQ(30, v.c);
}

static void test_seq_even_after_write() {
    SeqLock<int> sl;
    sl.write(1);
    ASSERT_EQ(0, (int)(sl.seq() % 2));   // must be even when not writing
}

// ─── Multi-threaded correctness ────────────────────────────────────────────

static void test_concurrent_no_torn_reads() {
    // Writer increments a Triple where a==b==c.
    // Readers verify a==b==c on every read they accept.
    SeqLock<Triple> sl;
    sl.write({0, 0, 0});

    std::atomic<bool> stop{false};
    std::atomic<long long> torn_detections{0};   // retries inside read()
    std::atomic<long long> reads_ok{0};

    // 4 reader threads
    auto reader = [&]() {
        while (!stop.load(std::memory_order_relaxed)) {
            Triple v = sl.read();
            // If the seqlock is correct, a==b==c always holds here
            ASSERT_TRUE(v.a == v.b && v.b == v.c);
            reads_ok.fetch_add(1, std::memory_order_relaxed);
        }
    };

    // 1 writer thread
    auto writer = [&]() {
        int n = 0;
        while (!stop.load(std::memory_order_relaxed)) {
            sl.write({n, n, n});
            ++n;
        }
    };

    std::vector<std::thread> readers;
    for (int i = 0; i < 4; ++i) readers.emplace_back(reader);
    std::thread wt(writer);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    stop.store(true, std::memory_order_relaxed);

    wt.join();
    for (auto& t : readers) t.join();

    std::printf("        reads_ok=%lld  (torn-read retries internal to seqlock)\n",
                reads_ok.load());
    ASSERT_TRUE(reads_ok.load() > 0);
}

// ─── Benchmark: seqlock vs shared_mutex ───────────────────────────────────

static void bench_seqlock_vs_shared_mutex() {
    const int N_READERS = 4;
    const long long ITERS = 2'000'000LL;

    // --- SeqLock ---
    {
        SeqLock<int> sl;
        sl.write(0);
        std::atomic<bool> go{false};
        std::atomic<long long> total{0};

        auto reader = [&]() {
            while (!go.load(std::memory_order_acquire)) {}
            long long cnt = 0;
            auto t = bench::now();
            for (long long i = 0; i < ITERS; ++i) {
                bench::do_not_optimize(sl.read());
                ++cnt;
            }
            total.fetch_add(bench::elapsed_ns(t), std::memory_order_relaxed);
        };

        std::vector<std::thread> ts;
        for (int i = 0; i < N_READERS; ++i) ts.emplace_back(reader);
        go.store(true, std::memory_order_release);
        for (auto& t : ts) t.join();

        double ns = (double)total.load() / N_READERS;
        bench::print_result("SeqLock read (4 readers)", ns / ITERS, ITERS);
    }

    // --- shared_mutex ---
    {
        std::shared_mutex mu;
        int data = 0;
        std::atomic<bool> go{false};
        std::atomic<long long> total{0};

        auto reader = [&]() {
            while (!go.load(std::memory_order_acquire)) {}
            long long cnt = 0;
            auto t = bench::now();
            for (long long i = 0; i < ITERS; ++i) {
                std::shared_lock<std::shared_mutex> lk(mu);
                bench::do_not_optimize(data);
                ++cnt;
            }
            total.fetch_add(bench::elapsed_ns(t), std::memory_order_relaxed);
        };

        std::vector<std::thread> ts;
        for (int i = 0; i < N_READERS; ++i) ts.emplace_back(reader);
        go.store(true, std::memory_order_release);
        for (auto& t : ts) t.join();

        double ns = (double)total.load() / N_READERS;
        bench::print_result("shared_mutex read (4 readers)", ns / ITERS, ITERS);
    }
}

// ─── Main ──────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    RUN_TEST(test_single_write_read);
    RUN_TEST(test_multiple_writes);
    RUN_TEST(test_struct_write_read);
    RUN_TEST(test_seq_even_after_write);
    RUN_TEST(test_concurrent_no_torn_reads);

    std::printf("\n--- Benchmark ---\n");
    bench_seqlock_vs_shared_mutex();

    TEST_END();
}
