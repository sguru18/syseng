#include <common.hpp>
#include <cstdint>
#include <type_traits>
#include "primitives.hpp"

// ── Price ─────────────────────────────────────────────────────────────────────

void test_price_construction() {
    Price p(500);
    ASSERT_EQ(500, p.value());
    Price zero;
    ASSERT_EQ(0, zero.value());
}

void test_price_arithmetic() {
    Price a(300), b(120);
    ASSERT_EQ(420, (a + b).value());
    ASSERT_EQ(180, (a - b).value());
}

void test_price_comparisons() {
    Price lo(50), hi(200);
    ASSERT_TRUE(lo < hi);
    ASSERT_TRUE(hi > lo);
    ASSERT_TRUE(lo <= lo);
    ASSERT_TRUE(hi >= hi);
    ASSERT_TRUE(lo != hi);
    ASSERT_TRUE(Price(99) == Price(99));
}

// ── Quantity ──────────────────────────────────────────────────────────────────

void test_quantity_construction() {
    Quantity q(1000);
    ASSERT_EQ(1000, q.value());
}

void test_quantity_arithmetic() {
    Quantity a(400), b(100);
    ASSERT_EQ(500, (a + b).value());
    ASSERT_EQ(300, (a - b).value());
    ASSERT_EQ(800, (a * 2).value());
}

void test_quantity_comparisons() {
    Quantity small(10), big(99);
    ASSERT_TRUE(small < big);
    ASSERT_TRUE(big > small);
    ASSERT_TRUE(Quantity(7) == Quantity(7));
    ASSERT_TRUE(small != big);
}

// ── Type safety ───────────────────────────────────────────────────────────────

void test_types_are_distinct() {
    static_assert(!std::is_same_v<Price, Quantity>,
        "Price and Quantity must be distinct types");
    // Intentionally does NOT compile -- uncomment to see the error:
    //   Price p(100); Quantity q(10);
    //   auto x = p + q;  // error: no match for operator+(Price, Quantity)
}

// ── Side ──────────────────────────────────────────────────────────────────────

void test_side_opposite() {
    ASSERT_TRUE(opposite(Side::Buy)  == Side::Sell);
    ASSERT_TRUE(opposite(Side::Sell) == Side::Buy);
}

void test_side_str() {
    ASSERT_STR_EQ("Buy",  side_str(Side::Buy));
    ASSERT_STR_EQ("Sell", side_str(Side::Sell));
}

// ── Order ─────────────────────────────────────────────────────────────────────

void test_order_construction() {
    Order o(42, Price(9850), Quantity(500), Side::Sell);
    ASSERT_EQ(42,   (int64_t)o.id);
    ASSERT_EQ(9850, o.price.value());
    ASSERT_EQ(500,  o.qty.value());
    ASSERT_TRUE(o.side == Side::Sell);
}

void test_order_round_trip() {
    Price  p(1234);
    Quantity q(10);
    Order  buy(1, p, q, Side::Buy);
    Order  sell(2, p, q * 2, Side::Sell);

    ASSERT_EQ(1234, buy.price.value());
    ASSERT_EQ(20,   sell.qty.value());
    ASSERT_TRUE(buy.side  == Side::Buy);
    ASSERT_TRUE(sell.side == Side::Sell);
    ASSERT_TRUE(opposite(buy.side) == sell.side);
}

// ── Symbol table ──────────────────────────────────────────────────────────────

void test_symbol_id_known() {
    ASSERT_EQ(0, symbol_id("AAPL"));
    ASSERT_EQ(1, symbol_id("MSFT"));
    ASSERT_EQ(2, symbol_id("GOOG"));
    ASSERT_EQ(3, symbol_id("AMZN"));
    ASSERT_EQ(4, symbol_id("META"));
    ASSERT_EQ(5, symbol_id("NVDA"));
    ASSERT_EQ(6, symbol_id("TSLA"));
    ASSERT_EQ(7, symbol_id("JPM"));
}

void test_symbol_id_unknown() {
    ASSERT_EQ(-1, symbol_id("UNKNOWN"));
    ASSERT_EQ(-1, symbol_id("XYZ"));
    ASSERT_EQ(-1, symbol_id(""));
    ASSERT_EQ(-1, symbol_id("aapl")); // case-sensitive
}

void test_symbol_id_constexpr() {
    // symbol_id must be evaluable at compile time.
    // Unknown symbols return -1 -- verifiable even with a stub:
    static_assert(symbol_id("UNKNOWN") == -1, "unknown symbols must return -1");

    // Once symbol_id is implemented, uncomment to verify compile-time lookup:
    // static_assert(symbol_id("AAPL") == 0);
    // static_assert(symbol_id("NVDA") == 5);
    // static_assert(symbol_id("JPM")  == 7);
}

void test_num_symbols() {
    ASSERT_EQ(8, NUM_SYMBOLS);
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    std::printf(BOLD "\n── Price ─────────────────────────────────────────\n" RESET);
    RUN_TEST(test_price_construction);
    RUN_TEST(test_price_arithmetic);
    RUN_TEST(test_price_comparisons);

    std::printf(BOLD "\n── Quantity ──────────────────────────────────────\n" RESET);
    RUN_TEST(test_quantity_construction);
    RUN_TEST(test_quantity_arithmetic);
    RUN_TEST(test_quantity_comparisons);

    std::printf(BOLD "\n── Type Safety ───────────────────────────────────\n" RESET);
    RUN_TEST(test_types_are_distinct);

    std::printf(BOLD "\n── Side ──────────────────────────────────────────\n" RESET);
    RUN_TEST(test_side_opposite);
    RUN_TEST(test_side_str);

    std::printf(BOLD "\n── Order ─────────────────────────────────────────\n" RESET);
    RUN_TEST(test_order_construction);
    RUN_TEST(test_order_round_trip);

    std::printf(BOLD "\n── Symbol Table ──────────────────────────────────\n" RESET);
    RUN_TEST(test_symbol_id_known);
    RUN_TEST(test_symbol_id_unknown);
    RUN_TEST(test_symbol_id_constexpr);
    RUN_TEST(test_num_symbols);

    TEST_END();
}
