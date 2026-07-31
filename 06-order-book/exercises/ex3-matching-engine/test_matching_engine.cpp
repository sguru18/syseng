#include "matching_engine.hpp"
#include "common.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <cstdio>

// ── generate_event_stream ─────────────────────────────────────────────────────
//
// Full implementation provided here — this is glue code, not the exercise.
//
// Produces a reproducible mixed stream:
//   ~60% AddOrder  (buys 98-100, sells 101-102, plus ~5% crossing orders that
//                   guarantee fills: buys at 102-104, sells at 96-98)
//   ~30% CancelOrder (random live order)
//   ~10% ModifyOrder (random live order, new random qty)
//
// Cancelled / filled IDs stay in the generator's live_ids list intentionally:
// the engine handles stale cancels gracefully (cancel_order returns false, no crash).
//
std::vector<OrderEvent> generate_event_stream(size_t n, uint64_t seed) {
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> roll_d(0, 9);
    std::uniform_int_distribution<int> side_d(0, 1);
    std::uniform_int_distribution<Qty> qty_d(1, 100);

    std::vector<OrderEvent> events;
    events.reserve(n);
    std::vector<OrderId> live_ids;
    live_ids.reserve(n / 2 + 1);
    OrderId next_id = 1;

    for (size_t i = 0; i < n; ++i) {
        int  roll   = roll_d(rng);
        bool do_add = live_ids.empty() || roll < 6;

        if (do_add) {
            OrderEvent ev{};
            ev.type = EventType::AddOrder;
            ev.id   = next_id++;
            ev.qty  = qty_d(rng);

            // Every ~19th add is a crossing buy (above ask range) and
            // every ~23rd add is a crossing sell (below bid range).
            if (i > 0 && i % 19 == 0) {
                ev.side  = Side::Buy;
                ev.price = 102 + (Price)(rng() % 3);   // 102-104 crosses 101-102 asks
            } else if (i > 0 && i % 23 == 0) {
                ev.side  = Side::Sell;
                ev.price = 98  - (Price)(rng() % 3);   // 96-98  crosses 98-100 bids
            } else {
                ev.side = side_d(rng) ? Side::Buy : Side::Sell;
                if (ev.side == Side::Buy)
                    ev.price = 98  + (Price)(rng() % 3);  // 98-100
                else
                    ev.price = 101 + (Price)(rng() % 2);  // 101-102
            }
            live_ids.push_back(ev.id);
            events.push_back(ev);

        } else if (roll < 9) {   // ~30% cancel
            size_t idx = rng() % live_ids.size();
            OrderEvent ev{};
            ev.type = EventType::CancelOrder;
            ev.id   = live_ids[idx];
            // Swap-and-pop to keep live_ids compact.
            std::swap(live_ids[idx], live_ids.back());
            live_ids.pop_back();
            events.push_back(ev);

        } else {                 // ~10% modify
            size_t idx = rng() % live_ids.size();
            OrderEvent ev{};
            ev.type = EventType::ModifyOrder;
            ev.id   = live_ids[idx];
            ev.qty  = qty_d(rng);
            events.push_back(ev);
        }
    }
    return events;
}

// ── Unit tests ────────────────────────────────────────────────────────────────

// Non-crossing orders produce no fills and both sides are visible in the book.
void test_no_fills_non_crossing() {
    MatchingEngine eng;
    std::vector<Fill> fills;

    OrderEvent ev{};
    ev.type = EventType::AddOrder; ev.id = 1; ev.side = Side::Buy;  ev.price = 99;  ev.qty = 10;
    eng.process_event(ev, fills);
    ev.type = EventType::AddOrder; ev.id = 2; ev.side = Side::Sell; ev.price = 101; ev.qty = 10;
    eng.process_event(ev, fills);

    ASSERT_EQ(0,   (int)fills.size());
    ASSERT_EQ(99,  eng.book().best_bid());
    ASSERT_EQ(101, eng.book().best_ask());
}

// Exactly-crossing orders produce one fill; both sides cleared.
void test_single_match() {
    MatchingEngine eng;
    std::vector<Fill> fills;
    OrderEvent ev{};

    // Resting sell at 100, qty 5.
    ev.type = EventType::AddOrder; ev.id = 1; ev.side = Side::Sell; ev.price = 100; ev.qty = 5;
    eng.process_event(ev, fills);
    ASSERT_EQ(0, (int)fills.size());

    // Aggressive buy at 100, qty 5 → full match.
    ev.type = EventType::AddOrder; ev.id = 2; ev.side = Side::Buy; ev.price = 100; ev.qty = 5;
    eng.process_event(ev, fills);

    ASSERT_EQ(1,   (int)fills.size());
    ASSERT_EQ(2,   (int)fills[0].aggressor_id);
    ASSERT_EQ(1,   (int)fills[0].passive_id);
    ASSERT_EQ(100, fills[0].price);
    ASSERT_EQ(5,   fills[0].qty);
    ASSERT_EQ(-1,  eng.book().best_bid());   // both orders consumed
    ASSERT_EQ(-1,  eng.book().best_ask());
}

// Aggressor larger than passive → partial fill, remainder rests.
void test_partial_fill() {
    MatchingEngine eng;
    std::vector<Fill> fills;
    OrderEvent ev{};

    ev.type = EventType::AddOrder; ev.id = 1; ev.side = Side::Sell; ev.price = 100; ev.qty = 5;
    eng.process_event(ev, fills);

    ev.type = EventType::AddOrder; ev.id = 2; ev.side = Side::Buy; ev.price = 100; ev.qty = 10;
    eng.process_event(ev, fills);

    ASSERT_EQ(1,   (int)fills.size());
    ASSERT_EQ(5,   fills[0].qty);          // only 5 filled
    ASSERT_EQ(100, fills[0].price);
    // Remaining 5 of the buy rests at 100.
    ASSERT_EQ(100, eng.book().best_bid());
    ASSERT_EQ(5,   eng.book().qty_at(Side::Buy, 100));
    ASSERT_EQ(-1,  eng.book().best_ask()); // sell fully consumed
}

// Aggressor sweeps multiple price levels → two fills, remainder rests.
void test_multiple_fills_sweep() {
    MatchingEngine eng;
    std::vector<Fill> fills;
    OrderEvent ev{};

    ev.type = EventType::AddOrder; ev.id = 1; ev.side = Side::Sell; ev.price = 100; ev.qty = 5;
    eng.process_event(ev, fills);
    ev.type = EventType::AddOrder; ev.id = 2; ev.side = Side::Sell; ev.price = 101; ev.qty = 5;
    eng.process_event(ev, fills);
    ASSERT_EQ(0, (int)fills.size());

    // Buy 20 at 105: sweeps both ask levels (5+5=10 filled), 10 remaining.
    ev.type = EventType::AddOrder; ev.id = 3; ev.side = Side::Buy; ev.price = 105; ev.qty = 20;
    eng.process_event(ev, fills);

    ASSERT_EQ(2,   (int)fills.size());
    ASSERT_EQ(100, fills[0].price); ASSERT_EQ(5, fills[0].qty);   // best ask first
    ASSERT_EQ(101, fills[1].price); ASSERT_EQ(5, fills[1].qty);
    // 10 remaining rests at 105.
    ASSERT_EQ(105, eng.book().best_bid());
    ASSERT_EQ(10,  eng.book().qty_at(Side::Buy, 105));
}

// Passive larger than aggressor → single fill, passive partially filled.
void test_passive_partially_filled() {
    MatchingEngine eng;
    std::vector<Fill> fills;
    OrderEvent ev{};

    ev.type = EventType::AddOrder; ev.id = 1; ev.side = Side::Sell; ev.price = 100; ev.qty = 20;
    eng.process_event(ev, fills);

    ev.type = EventType::AddOrder; ev.id = 2; ev.side = Side::Buy; ev.price = 100; ev.qty = 7;
    eng.process_event(ev, fills);

    ASSERT_EQ(1,   (int)fills.size());
    ASSERT_EQ(7,   fills[0].qty);
    // Sell order still resting with 13 remaining.
    ASSERT_EQ(100, eng.book().best_ask());
    ASSERT_EQ(13,  eng.book().qty_at(Side::Sell, 100));
}

// Cancel a resting order before a crossing aggressor arrives → no fill.
void test_cancel_before_match() {
    MatchingEngine eng;
    std::vector<Fill> fills;
    OrderEvent ev{};

    // Resting sell at 100.
    ev.type = EventType::AddOrder; ev.id = 1; ev.side = Side::Sell; ev.price = 100; ev.qty = 5;
    eng.process_event(ev, fills);

    // Cancel it.
    ev = {}; ev.type = EventType::CancelOrder; ev.id = 1;
    eng.process_event(ev, fills);

    // Aggressive buy at 100 — book has no asks, so no fill.
    ev = {}; ev.type = EventType::AddOrder; ev.id = 2;
    ev.side = Side::Buy; ev.price = 100; ev.qty = 5;
    eng.process_event(ev, fills);

    ASSERT_EQ(0,   (int)fills.size());
    ASSERT_EQ(100, eng.book().best_bid());  // buy rests
    ASSERT_EQ(-1,  eng.book().best_ask()); // no asks
}

// Sell-side aggressor: resting buy at 100, incoming sell at 99 → fill at 100.
void test_sell_aggressor_matches_bid() {
    MatchingEngine eng;
    std::vector<Fill> fills;
    OrderEvent ev{};

    ev.type = EventType::AddOrder; ev.id = 1; ev.side = Side::Buy;  ev.price = 100; ev.qty = 8;
    eng.process_event(ev, fills);

    ev.type = EventType::AddOrder; ev.id = 2; ev.side = Side::Sell; ev.price = 99;  ev.qty = 8;
    eng.process_event(ev, fills);

    ASSERT_EQ(1,   (int)fills.size());
    ASSERT_EQ(100, fills[0].price);   // fill at passive's price (100), not aggressor's (99)
    ASSERT_EQ(8,   fills[0].qty);
    ASSERT_EQ(-1,  eng.book().best_bid());
    ASSERT_EQ(-1,  eng.book().best_ask());
}

// Stats counters reflect all processed events and generated fills.
void test_stats_counters() {
    MatchingEngine eng;
    std::vector<Fill> fills;
    OrderEvent ev{};

    ev.type = EventType::AddOrder; ev.id = 1; ev.side = Side::Sell; ev.price = 100; ev.qty = 5;
    eng.process_event(ev, fills);
    ev.type = EventType::AddOrder; ev.id = 2; ev.side = Side::Buy;  ev.price = 100; ev.qty = 5;
    eng.process_event(ev, fills);

    ASSERT_EQ(2, (int)eng.orders_processed());
    ASSERT_EQ(1, (int)eng.fills_generated());
}

// ── Benchmark ─────────────────────────────────────────────────────────────────

void bench_100k_events() {
    const size_t N = 100'000;
    auto events = generate_event_stream(N, /*seed=*/99);

    std::vector<Fill>      fills;
    std::vector<long long> latencies;
    fills.reserve(N / 5);
    latencies.reserve(N);

    MatchingEngine eng;
    auto total_start = bench::now();
    for (const auto& ev : events) {
        auto t = bench::now();
        eng.process_event(ev, fills);
        latencies.push_back(bench::elapsed_ns(t));
    }
    double total_ns = (double)bench::elapsed_ns(total_start);

    std::printf("\n");
    std::printf("  events processed : %llu\n",
                (unsigned long long)eng.orders_processed());
    std::printf("  fills generated  : %llu\n",
                (unsigned long long)eng.fills_generated());
    bench::print_throughput("matching engine events", (long long)N, total_ns);
    bench::print_latency_dist("per-event latency", latencies);
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();
    RUN_TEST(test_no_fills_non_crossing);
    RUN_TEST(test_single_match);
    RUN_TEST(test_partial_fill);
    RUN_TEST(test_multiple_fills_sweep);
    RUN_TEST(test_passive_partially_filled);
    RUN_TEST(test_cancel_before_match);
    RUN_TEST(test_sell_aggressor_matches_bid);
    RUN_TEST(test_stats_counters);
    bench_100k_events();
    TEST_END();
}
