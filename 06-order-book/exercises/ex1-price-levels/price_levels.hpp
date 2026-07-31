#pragma once
// Structural includes for the provided API types and print_top():
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <map>
#include <vector>
#include <functional>
/* TODO: add any additional #include lines your implementation needs */

// ── Vocabulary types ──────────────────────────────────────────────────────────

using Price = int64_t;
using Qty   = int64_t;

enum class Side { Buy, Sell };

// A snapshot of a single price level.
struct Level {
    Price price;
    Qty   qty;
};

// ── PriceLevelBook ────────────────────────────────────────────────────────────
//
// Maintains bid and ask price levels as aggregate quantities per price.
//
//   Bids: sorted descending  (highest bid  == begin())
//   Asks: sorted ascending   (lowest  ask  == begin())
//
// Use std::map internally:
//   O(log n) for update/remove,  O(1) begin() for best bid/ask.
//
class PriceLevelBook {
public:
    // TODO: Set the quantity at (side, price).
    //       If qty == 0, remove the level entirely.
    void update(Side side, Price price, Qty qty) { (void)side; (void)price; (void)qty; }

    // TODO: Return the best (highest) bid price. Return -1 if no bids exist.
    Price best_bid() const { return -1; }

    // TODO: Return the best (lowest) ask price. Return -1 if no asks exist.
    Price best_ask() const { return -1; }

    // TODO: Return the quantity at (side, price). Return 0 if no level exists.
    Qty qty_at(Side side, Price price) const { (void)side; (void)price; return 0; }

    // TODO: Return the number of distinct price levels on the bid side.
    size_t bid_levels() const { return 0; }

    // TODO: Return the number of distinct price levels on the ask side.
    size_t ask_levels() const { return 0; }

    // TODO: Return spread = best_ask - best_bid.
    //       Return -1 if either side is empty.
    Price spread() const { return -1; }

    // Display top N levels on each side.
    // Provided — implement your TODOs first, then this will just work.
    // (Requires <cstdio> and <vector> in your include block above.)
    void print_top(int n = 5) const {
        // Collect the n tightest asks, then display high→low so the book looks right.
        std::vector<std::pair<Price, Qty>> top_asks;
        for (auto& [p, q] : asks_) {
            top_asks.push_back({p, q});
            if ((int)top_asks.size() >= n) break;
        }
        for (int i = (int)top_asks.size() - 1; i >= 0; --i)
            std::printf("  ask  %6lld   qty %8lld\n",
                        (long long)top_asks[i].first,
                        (long long)top_asks[i].second);
        std::printf("  ---  spread: %lld  ---\n", (long long)spread());
        // Bids are already highest-first.
        int cnt = 0;
        for (auto& [p, q] : bids_) {
            if (cnt >= n) break;
            std::printf("  bid  %6lld   qty %8lld\n",
                        (long long)p, (long long)q);
            ++cnt;
        }
    }

private:
    std::map<Price, Qty, std::greater<Price>> bids_;  // highest bid at begin()
    std::map<Price, Qty>                       asks_;  // lowest  ask at begin()
};
