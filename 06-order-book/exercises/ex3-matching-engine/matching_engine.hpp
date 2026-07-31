#pragma once
#include "order_book.hpp"   // ex2 — LimitOrderBook, Order, OrderId, Side, Price, Qty
// Structural includes for the provided API types:
#include <vector>
#include <deque>
#include <map>
#include <functional>
/* TODO: add any additional #include lines your implementation needs */

// ── Event types ───────────────────────────────────────────────────────────────

enum class EventType : uint8_t {
    AddOrder    = 1,
    CancelOrder = 2,
    ModifyOrder = 3,
};

struct OrderEvent {
    EventType type;
    OrderId   id;
    Side      side;
    Price     price;
    Qty       qty;    // used for AddOrder and ModifyOrder; ignored for CancelOrder
};

// A completed match: aggressor matched against a passive resting order.
struct Fill {
    OrderId aggressor_id;
    OrderId passive_id;
    Price   price;   // fill price = passive order's resting price
    Qty     qty;
};

// ── MatchingEngine ────────────────────────────────────────────────────────────
//
// Processes a stream of OrderEvents.  When an incoming order crosses the book,
// fills are emitted into the `fills` vector passed to process_event().
//
// Design pattern — lazy deletion:
//   Maintain per-level FIFO queues (bid_queue_, ask_queue_) that hold OrderIds
//   in arrival order.  On cancel/fill, do NOT scan queues; instead, when popping
//   the front of a queue, check book_.find_order(id).  If null, the order was
//   already removed — skip it and try the next one.
//
// Matching rule:
//   Buy  at P → loop while best_ask() <= P: fill against front of ask_queue_[best_ask()]
//   Sell at P → loop while best_bid() >= P: fill against front of bid_queue_[best_bid()]
//   Fill qty = min(aggressor remaining, passive qty).
//   Fill price = passive order's price.
//   Fully filled passive orders are cancelled from book_.
//   Partially filled passive orders are modified (qty reduced) in book_.
//   Any unfilled remainder of the aggressor is added to book_ as a resting order.
//
class MatchingEngine {
public:
    // TODO: Process one event and append any resulting fills to `fills`.
    void process_event(const OrderEvent& ev, std::vector<Fill>& fills) {
        (void)ev; (void)fills;
    }

    // TODO: Process a batch of events (loop over process_event).
    void process_events(const std::vector<OrderEvent>& events,
                        std::vector<Fill>& fills) {
        (void)events; (void)fills;
    }

    uint64_t orders_processed() const { return orders_processed_; }
    uint64_t fills_generated()  const { return fills_generated_; }
    const LimitOrderBook& book() const { return book_; }

private:
    LimitOrderBook book_;
    uint64_t orders_processed_ = 0;
    uint64_t fills_generated_  = 0;

    // Per-level FIFO queues for price-time priority (lazy deletion).
    // TODO: declare bid_queue_ and ask_queue_
    //   Hint:
    //     std::map<Price, std::deque<OrderId>, std::greater<Price>> bid_queue_;
    //     std::map<Price, std::deque<OrderId>>                       ask_queue_;
};

// Generate a reproducible mixed event stream for testing and benchmarking.
// ~60% AddOrder (non-crossing mix of buys 98-100, sells 101-102, plus
// some crossing orders to guarantee fills), ~30% CancelOrder, ~10% ModifyOrder.
// Definition is in test_matching_engine.cpp (glue code, not the exercise).
std::vector<OrderEvent> generate_event_stream(size_t n_events, uint64_t seed = 42);
