#include <atomic>
#include <array>
#include <optional>
#include <cstddef>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdint>
#include <cstring>
#include "common.hpp"
#include "spsc_queue.hpp"

// ─── Event type used in throughput / latency benchmarks ───────────────────

struct Event {
    uint64_t timestamp;
    int32_t  price;
    int32_t  qty;
};

// ─── Single-threaded correctness ───────────────────────────────────────────

static void test_push_pop_basic() {
    SPSCQueue<int, 16> q;
    ASSERT_TRUE(q.empty());
    ASSERT_TRUE(q.push(1));
    ASSERT_TRUE(q.push(2));
    ASSERT_TRUE(q.push(3));
    ASSERT_FALSE(q.empty());

    auto v1 = q.pop();
    ASSERT_TRUE(v1.has_value());
    ASSERT_EQ(1, v1.value());

    auto v2 = q.pop();
    ASSERT_EQ(2, v2.value());

    auto v3 = q.pop();
    ASSERT_EQ(3, v3.value());

    ASSERT_TRUE(q.empty());
    auto empty = q.pop();
    ASSERT_FALSE(empty.has_value());
}

static void test_try_pop() {
    SPSCQueue<int, 8> q;
    q.push(99);
    int out = 0;
    ASSERT_TRUE(q.try_pop(out));
    ASSERT_EQ(99, out);

    int out2 = 0;
    ASSERT_FALSE(q.try_pop(out2));
    ASSERT_EQ(0, out2);
}

static void test_full_behavior() {
    // Capacity is N-1 = 7 slots for N=8
    SPSCQueue<int, 8> q;
    int pushes = 0;
    while (q.push(pushes)) ++pushes;
    ASSERT_EQ(7, pushes);
    ASSERT_TRUE(q.full());
    ASSERT_FALSE(q.push(999));
}

static void test_wrap_around() {
    SPSCQueue<int, 8> q;
    // Fill to capacity
    for (int i = 0; i < 7; ++i) ASSERT_TRUE(q.push(i));
    // Drain
    for (int i = 0; i < 7; ++i) {
        auto v = q.pop();
        ASSERT_TRUE(v.has_value());
        ASSERT_EQ(i, v.value());
    }
    ASSERT_TRUE(q.empty());
    // Fill again to verify wrap-around works
    for (int i = 0; i < 7; ++i) ASSERT_TRUE(q.push(i + 100));
    for (int i = 0; i < 7; ++i) {
        auto v = q.pop();
        ASSERT_TRUE(v.has_value());
        ASSERT_EQ(i + 100, v.value());
    }
    ASSERT_TRUE(q.empty());
}

static void test_move_push() {
    SPSCQueue<Event, 16> q;
    Event e{12345ULL, 100, 500};
    ASSERT_TRUE(q.push(std::move(e)));
    int out_price = 0;
    Event out{};
    ASSERT_TRUE(q.try_pop(out));
    ASSERT_EQ(100, out.price);
    ASSERT_EQ(500, out.qty);
}

// ─── Multi-threaded throughput ─────────────────────────────────────────────

static void bench_throughput() {
    constexpr long long TOTAL = 10'000'000LL;
    constexpr std::size_t QSIZE = 1 << 16;

    SPSCQueue<Event, QSIZE> q;
    std::atomic<bool> consumer_done{false};
    long long consumer_count = 0;

    auto t_start = bench::now();

    std::thread consumer([&]() {
        Event e{};
        long long cnt = 0;
        while (cnt < TOTAL) {
            if (q.try_pop(e)) ++cnt;
        }
        consumer_count = cnt;
        consumer_done.store(true, std::memory_order_release);
    });

    {
        long long cnt = 0;
        while (cnt < TOTAL) {
            Event e{(uint64_t)cnt, (int32_t)(cnt & 0x7fffffff), (int32_t)(cnt >> 32)};
            while (!q.push(e)) {}
            ++cnt;
        }
    }

    consumer.join();

    double total_ns = (double)bench::elapsed_ns(t_start);
    ASSERT_EQ(TOTAL, consumer_count);
    bench::print_throughput("SPSC throughput (10M events)", TOTAL, total_ns);
}

// ─── Multi-threaded latency ────────────────────────────────────────────────

static void bench_latency() {
    constexpr int SAMPLES = 10'000;
    constexpr std::size_t QSIZE = 1 << 12;

    SPSCQueue<uint64_t, QSIZE> q;    // producer writes timestamp, consumer reads
    std::vector<long long> latencies;
    latencies.reserve(SAMPLES);
    std::atomic<bool> ready{false};
    std::atomic<bool> done{false};

    std::thread consumer([&]() {
        uint64_t ts = 0;
        int collected = 0;
        while (!ready.load(std::memory_order_acquire)) {}
        while (collected < SAMPLES) {
            if (q.try_pop(ts)) {
                uint64_t recv = (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()).count();
                latencies.push_back((long long)(recv - ts));
                ++collected;
            }
        }
        done.store(true, std::memory_order_release);
    });

    ready.store(true, std::memory_order_release);

    for (int i = 0; i < SAMPLES; ++i) {
        uint64_t ts = (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        while (!q.push(ts)) {}
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }

    consumer.join();
    bench::print_latency_dist("SPSC end-to-end latency", latencies);
}

// ─── Main ──────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    RUN_TEST(test_push_pop_basic);
    RUN_TEST(test_try_pop);
    RUN_TEST(test_full_behavior);
    RUN_TEST(test_wrap_around);
    RUN_TEST(test_move_push);

    std::printf("\n--- Benchmark ---\n");
    bench_throughput();
    bench_latency();

    TEST_END();
}
