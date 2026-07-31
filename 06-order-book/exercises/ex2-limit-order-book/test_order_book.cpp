#include "order_book.hpp"
#include "common.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <cstdio>

// ── Unit tests ────────────────────────────────────────────────────────────────

void test_add_order_updates_level() {
    LimitOrderBook book;
    ASSERT_TRUE(book.add_order({1, Side::Buy, 100, 50}));
    ASSERT_EQ(1,   (int)book.order_count());
    ASSERT_EQ(100, book.best_bid());
    ASSERT_EQ(50,  book.qty_at(Side::Buy, 100));

    const Order* o = book.find_order(1);
    ASSERT_NOT_NULL(o);
    ASSERT_EQ(1,   (int)o->id);
    ASSERT_EQ(100, o->price);
    ASSERT_EQ(50,  o->qty);
}

void test_add_two_orders_same_level() {
    LimitOrderBook book;
    book.add_order({1, Side::Buy, 100, 30});
    book.add_order({2, Side::Buy, 100, 20});
    ASSERT_EQ(2,   (int)book.order_count());
    ASSERT_EQ(50,  book.qty_at(Side::Buy, 100));   // aggregate
    ASSERT_EQ(100, book.best_bid());
}

void test_cancel_order() {
    LimitOrderBook book;
    book.add_order({1, Side::Buy, 100, 50});
    ASSERT_TRUE(book.cancel_order(1));
    ASSERT_EQ(0,  (int)book.order_count());
    ASSERT_EQ(-1, book.best_bid());                // level removed
    ASSERT_EQ(0,  book.qty_at(Side::Buy, 100));
    ASSERT_NULL(book.find_order(1));
}

void test_cancel_partial_level() {
    LimitOrderBook book;
    book.add_order({1, Side::Buy, 100, 50});
    book.add_order({2, Side::Buy, 100, 30});
    ASSERT_TRUE(book.cancel_order(1));
    ASSERT_EQ(1,   (int)book.order_count());
    ASSERT_EQ(30,  book.qty_at(Side::Buy, 100));   // only order 2 remains
    ASSERT_EQ(100, book.best_bid());
}

void test_modify_order_qty() {
    LimitOrderBook book;
    book.add_order({1, Side::Buy, 100, 50});
    ASSERT_TRUE(book.modify_order(1, 30));
    ASSERT_EQ(30, book.qty_at(Side::Buy, 100));

    const Order* o = book.find_order(1);
    ASSERT_NOT_NULL(o);
    ASSERT_EQ(30, o->qty);
    ASSERT_EQ(1,  (int)book.order_count());
}

void test_modify_to_zero_cancels() {
    LimitOrderBook book;
    book.add_order({1, Side::Buy, 100, 50});
    ASSERT_TRUE(book.modify_order(1, 0));
    ASSERT_EQ(0,  (int)book.order_count());
    ASSERT_NULL(book.find_order(1));
    ASSERT_EQ(-1, book.best_bid());
}

void test_duplicate_order_id_rejected() {
    LimitOrderBook book;
    ASSERT_TRUE(book.add_order({1, Side::Buy, 100, 50}));
    ASSERT_FALSE(book.add_order({1, Side::Sell, 101, 20}));  // same ID
    ASSERT_EQ(1, (int)book.order_count());   // still only one order
}

void test_cancel_nonexistent_returns_false() {
    LimitOrderBook book;
    ASSERT_FALSE(book.cancel_order(999));
}

void test_modify_nonexistent_returns_false() {
    LimitOrderBook book;
    ASSERT_FALSE(book.modify_order(999, 10));
}

void test_best_bid_ask_delegation() {
    LimitOrderBook book;
    book.add_order({1, Side::Buy,  99,  100});
    book.add_order({2, Side::Buy,  100, 200});
    book.add_order({3, Side::Sell, 101, 150});
    book.add_order({4, Side::Sell, 102,  50});

    ASSERT_EQ(100, book.best_bid());
    ASSERT_EQ(101, book.best_ask());
    ASSERT_EQ(1,   book.spread());
}

// Simulation: add 10k orders, cancel 30%, verify consistency throughout.
void test_simulation() {
    LimitOrderBook book;
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<Price> price_d(95, 110);
    std::uniform_int_distribution<Qty>   qty_d(1, 200);
    std::uniform_int_distribution<int>   side_d(0, 1);

    const int N = 10'000;
    std::vector<OrderId> live;
    live.reserve(N);

    // Add N orders.
    for (int i = 0; i < N; ++i) {
        Order o;
        o.id    = (OrderId)(i + 1);
        o.side  = side_d(rng) ? Side::Buy : Side::Sell;
        o.price = price_d(rng);
        o.qty   = qty_d(rng);
        ASSERT_TRUE(book.add_order(o));
        live.push_back(o.id);
    }
    ASSERT_EQ(N, (int)book.order_count());

    // Cancel a random 30%.
    std::shuffle(live.begin(), live.end(), rng);
    int to_cancel = N * 3 / 10;
    for (int i = 0; i < to_cancel; ++i)
        ASSERT_TRUE(book.cancel_order(live[i]));

    int remaining = N - to_cancel;
    ASSERT_EQ(remaining, (int)book.order_count());

    // Every remaining order must be findable.
    for (int i = to_cancel; i < N; ++i)
        ASSERT_NOT_NULL(book.find_order(live[i]));

    // Every cancelled order must be gone.
    for (int i = 0; i < to_cancel; ++i)
        ASSERT_NULL(book.find_order(live[i]));
}

// ── Benchmark ─────────────────────────────────────────────────────────────────

void bench_add_cancel_1m() {
    const int N = 1'000'000;
    LimitOrderBook book;

    auto t = bench::now();
    for (int i = 0; i < N; ++i) {
        OrderId id    = (OrderId)(i + 1);
        Side    side  = (i % 2 == 0) ? Side::Buy  : Side::Sell;
        Price   price = (i % 2 == 0) ? (95 + i % 5) : (101 + i % 5);
        book.add_order({id, side, price, 100});
        book.cancel_order(id);
    }
    double total_ns = (double)bench::elapsed_ns(t);
    bench::print_result(    "1M add+cancel round-trips (avg)", total_ns / N, N);
    bench::print_throughput("add+cancel throughput (2M ops)",  (long long)N * 2, total_ns);
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();
    RUN_TEST(test_add_order_updates_level);
    RUN_TEST(test_add_two_orders_same_level);
    RUN_TEST(test_cancel_order);
    RUN_TEST(test_cancel_partial_level);
    RUN_TEST(test_modify_order_qty);
    RUN_TEST(test_modify_to_zero_cancels);
    RUN_TEST(test_duplicate_order_id_rejected);
    RUN_TEST(test_cancel_nonexistent_returns_false);
    RUN_TEST(test_modify_nonexistent_returns_false);
    RUN_TEST(test_best_bid_ask_delegation);
    RUN_TEST(test_simulation);
    std::printf("\n");
    bench_add_cancel_1m();
    TEST_END();
}
