#include "price_levels.hpp"
#include "common.hpp"
#include <random>
#include <vector>
#include <cstdio>

// ── Unit tests ────────────────────────────────────────────────────────────────

void test_update_adds_levels() {
    PriceLevelBook book;
    book.update(Side::Buy,  100, 50);
    ASSERT_EQ(1,  (int)book.bid_levels());
    ASSERT_EQ(50, book.qty_at(Side::Buy, 100));

    book.update(Side::Buy,  99, 30);
    ASSERT_EQ(2,  (int)book.bid_levels());

    book.update(Side::Sell, 101, 40);
    ASSERT_EQ(1,  (int)book.ask_levels());
    ASSERT_EQ(40, book.qty_at(Side::Sell, 101));
}

void test_update_removes_level() {
    PriceLevelBook book;
    book.update(Side::Buy, 100, 50);
    book.update(Side::Buy, 100, 0);   // remove by setting qty = 0
    ASSERT_EQ(0, (int)book.bid_levels());
    ASSERT_EQ(0, book.qty_at(Side::Buy, 100));
}

void test_update_modifies_qty() {
    PriceLevelBook book;
    book.update(Side::Buy, 100, 50);
    book.update(Side::Buy, 100, 75);   // overwrite qty
    ASSERT_EQ(75, book.qty_at(Side::Buy, 100));
    ASSERT_EQ(1,  (int)book.bid_levels());  // still one level
}

void test_best_bid_ask_empty() {
    PriceLevelBook book;
    ASSERT_EQ(-1, book.best_bid());
    ASSERT_EQ(-1, book.best_ask());
}

void test_best_bid_returns_highest() {
    PriceLevelBook book;
    book.update(Side::Buy, 99,  10);
    book.update(Side::Buy, 100, 20);
    book.update(Side::Buy, 98,   5);
    ASSERT_EQ(100, book.best_bid());
}

void test_best_ask_returns_lowest() {
    PriceLevelBook book;
    book.update(Side::Sell, 102, 10);
    book.update(Side::Sell, 101, 20);
    book.update(Side::Sell, 103,  5);
    ASSERT_EQ(101, book.best_ask());
}

void test_best_bid_after_removal() {
    PriceLevelBook book;
    book.update(Side::Buy, 100, 50);
    book.update(Side::Buy, 99,  30);
    book.update(Side::Buy, 100, 0);   // remove best
    ASSERT_EQ(99, book.best_bid());
}

void test_spread() {
    PriceLevelBook book;
    ASSERT_EQ(-1, book.spread());   // both sides empty

    book.update(Side::Buy, 99, 10);
    ASSERT_EQ(-1, book.spread());   // only one side

    book.update(Side::Sell, 101, 10);
    ASSERT_EQ(2, book.spread());    // 101 - 99 = 2
}

void test_spread_zero() {
    PriceLevelBook book;
    book.update(Side::Buy,  100, 10);
    book.update(Side::Sell, 100, 10);
    ASSERT_EQ(0, book.spread());
}

// Simulation: 1000 random updates with non-crossing prices.
// Invariant: best_bid() < best_ask() whenever both sides are non-empty.
void test_simulation_invariants() {
    PriceLevelBook book;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int>   side_d(0, 1);
    std::uniform_int_distribution<Price> bid_p(95, 99);
    std::uniform_int_distribution<Price> ask_p(101, 105);
    // qty=0 roughly 1/4 of the time to exercise removals
    std::uniform_int_distribution<Qty>   qty_d(0, 3);

    for (int i = 0; i < 1000; ++i) {
        Side  side  = side_d(rng) ? Side::Buy : Side::Sell;
        Price price = (side == Side::Buy) ? bid_p(rng) : ask_p(rng);
        Qty   qty   = qty_d(rng) == 0 ? 0 : (1 + (Qty)(rng() % 200));
        book.update(side, price, qty);

        Price bb = book.best_bid();
        Price ba = book.best_ask();
        if (bb != -1 && ba != -1)
            ASSERT_TRUE(bb < ba);
    }
}

void test_print_top_no_crash() {
    PriceLevelBook book;
    for (int p = 95; p <= 99;  ++p) book.update(Side::Buy,  p, (Price)(100 * (100 - p)));
    for (int p = 101; p <= 105; ++p) book.update(Side::Sell, p, (Price)(100 * p));
    book.print_top(3);   // must not crash or assert
}

// ── Benchmark ─────────────────────────────────────────────────────────────────

void bench_1m_updates() {
    PriceLevelBook book;
    const int N = 1'000'000;

    auto t = bench::now();
    for (int i = 0; i < N; ++i) {
        Side  side  = (i % 2 == 0) ? Side::Buy  : Side::Sell;
        Price price = (i % 2 == 0) ? (95 + i % 5) : (101 + i % 5);
        Qty   qty   = 1 + (i % 100);
        book.update(side, price, qty);
        bench::do_not_optimize(price);
    }
    double total_ns = (double)bench::elapsed_ns(t);
    bench::print_result(    "1M price-level updates (avg)", total_ns / N, N);
    bench::print_throughput("price-level update throughput", N, total_ns);
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();
    RUN_TEST(test_update_adds_levels);
    RUN_TEST(test_update_removes_level);
    RUN_TEST(test_update_modifies_qty);
    RUN_TEST(test_best_bid_ask_empty);
    RUN_TEST(test_best_bid_returns_highest);
    RUN_TEST(test_best_ask_returns_lowest);
    RUN_TEST(test_best_bid_after_removal);
    RUN_TEST(test_spread);
    RUN_TEST(test_spread_zero);
    RUN_TEST(test_simulation_invariants);
    RUN_TEST(test_print_top_no_crash);
    std::printf("\n");
    bench_1m_updates();
    TEST_END();
}
