#pragma once
#include "price_levels.hpp"   // ex1 — Price, Qty, Side, PriceLevelBook
/* TODO: add your #include lines */
//   Hint: <unordered_map>, <cstddef>

// ── Order types ───────────────────────────────────────────────────────────────

using OrderId = uint64_t;

struct Order {
    OrderId id;
    Side    side;
    Price   price;
    Qty     qty;
};

// ── LimitOrderBook ────────────────────────────────────────────────────────────
//
// Extends PriceLevelBook with per-order tracking so individual orders can be
// looked up and cancelled in O(1).
//
// Internal layout:
//   book_   — PriceLevelBook for aggregate bid/ask levels (best price, qty_at)
//   orders_ — hash map OrderId → Order for O(1) cancel and lookup
//
class LimitOrderBook {
public:
    // TODO: Add an order to the book.
    //   • Insert into orders_ and call book_.update() to raise the level qty.
    //   • Return false (no-op) if the order ID already exists.
    bool add_order(const Order& order) { (void)order; return false; }

    // TODO: Cancel an order by ID.
    //   • O(1) lookup in orders_, then O(log n) book_.update() to lower the level.
    //   • Return false if the order is not found.
    bool cancel_order(OrderId id) { (void)id; return false; }

    // TODO: Change an order's quantity in place (same price, same ID).
    //   • Equivalent to cancel + re-add at the same price — no time-priority change.
    //   • If new_qty <= 0, cancel the order entirely.
    //   • Return false if the order is not found.
    bool modify_order(OrderId id, Qty new_qty) { (void)id; (void)new_qty; return false; }

    // Delegated to the underlying price level book.
    Price best_bid()              const { return book_.best_bid(); }
    Price best_ask()              const { return book_.best_ask(); }
    Price spread()                const { return book_.spread(); }
    Qty   qty_at(Side s, Price p) const { return book_.qty_at(s, p); }
    void  print_top(int n = 5)    const { book_.print_top(n); }

    // TODO: Return the number of live orders currently in the book.
    size_t order_count() const { return 0; }

    // TODO: Return a pointer to the order with the given ID, or nullptr if not found.
    const Order* find_order(OrderId id) const { (void)id; return nullptr; }

private:
    PriceLevelBook book_;
    // TODO: add data structure(s) for O(1) order lookup by ID
    //   Hint: std::unordered_map<OrderId, Order> orders_;
};
