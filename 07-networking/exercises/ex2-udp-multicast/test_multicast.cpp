#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include "multicast.hpp"
#include "common.hpp"

static void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// ── tests ─────────────────────────────────────────────────────────────────────

static void test_wire_format_size() {
    // Compile-time guarantee: packed struct must be exactly 24 bytes
    static_assert(sizeof(QuoteMsg) == 24, "QuoteMsg must be 24 bytes (packed)");
}

static void test_send_recv_100() {
    MulticastReceiver receiver;
    MulticastSender   sender;
    ASSERT_TRUE(receiver.init());
    ASSERT_TRUE(sender.init());

    static constexpr int N = 100;

    for (int i = 0; i < N; i++) {
        QuoteMsg msg{0, 10000 + i, 100, 200, static_cast<uint64_t>(i)};
        ASSERT_TRUE(sender.send(msg));
    }

    sleep_ms(100);  // let packets arrive in the socket buffer

    QuoteMsg out{};
    int received = 0;
    for (int i = 0; i < 2000 && received < N; i++) {
        if (receiver.recv_one(out)) received++;
    }

    ASSERT_EQ(N, received);
    ASSERT_EQ(0u, receiver.gaps_detected());
    ASSERT_EQ(static_cast<uint64_t>(N), receiver.packets_received());
}

static void test_gap_detection() {
    MulticastReceiver receiver;
    MulticastSender   sender;
    ASSERT_TRUE(receiver.init());
    ASSERT_TRUE(sender.init());

    // seq 0,1,2 then skip 3 and send 4 -> expect 1 gap
    uint64_t seqs[] = {0, 1, 2, 4};
    for (auto s : seqs) {
        QuoteMsg msg{1, 20000, 50, 75, s};
        ASSERT_TRUE(sender.send(msg));
    }

    sleep_ms(50);

    QuoteMsg out{};
    int count = 0;
    for (int i = 0; i < 200 && count < 4; i++) {
        if (receiver.recv_one(out)) count++;
    }

    ASSERT_EQ(4, count);
    ASSERT_EQ(1u, receiver.gaps_detected());
}

static void test_price_table() {
    PriceTable table;

    // 5 symbols, 10 updates each (50 total)
    for (int sym = 0; sym < 5; sym++) {
        for (int upd = 0; upd < 10; upd++) {
            QuoteMsg msg{
                static_cast<uint32_t>(sym),
                10000 + sym * 100 + upd,
                200 + upd,
                300 + upd,
                static_cast<uint64_t>(upd)
            };
            table.update(msg);
        }
    }

    ASSERT_EQ(5u, table.symbol_count());

    // Each symbol should reflect the last update (upd=9)
    for (int sym = 0; sym < 5; sym++) {
        auto q = table.get(sym);
        int32_t expected_price = 10000 + sym * 100 + 9;
        ASSERT_EQ(expected_price, q.bid);
        ASSERT_EQ(expected_price, q.ask);
        ASSERT_EQ(209, q.bid_qty);
        ASSERT_EQ(309, q.ask_qty);
    }

    // Unknown symbol -> {0,0,0,0}
    auto q = table.get(999);
    ASSERT_EQ(0, q.bid);
    ASSERT_EQ(0, q.ask);
}

static void test_throughput() {
    MulticastReceiver receiver;
    MulticastSender   sender;
    ASSERT_TRUE(receiver.init());
    ASSERT_TRUE(sender.init());

    static constexpr int N = 10000;

    auto t = bench::now();
    for (int i = 0; i < N; i++) {
        QuoteMsg msg{0, 10000, 100, 200, static_cast<uint64_t>(i)};
        sender.send(msg);
    }
    double send_ns = static_cast<double>(bench::elapsed_ns(t));
    bench::print_throughput("multicast send rate (10k pkts)", N, send_ns);

    // Drain
    QuoteMsg out{};
    int received = 0;
    auto drain = bench::now();
    while (bench::elapsed_ms(drain) < 500 && received < N) {
        if (receiver.recv_one(out)) received++;
    }
    printf("  Sent %d, drained %d packets\n", N, received);
}

// ── main ──────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    RUN_TEST(test_wire_format_size);
    RUN_TEST(test_send_recv_100);
    RUN_TEST(test_gap_detection);
    RUN_TEST(test_price_table);
    RUN_TEST(test_throughput);

    TEST_END();
}
