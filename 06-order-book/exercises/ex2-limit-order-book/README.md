# Exercise 2 — Limit Order Book

Exchanges receive millions of cancel/modify messages per second. The limit order book must support O(1) cancel by order ID. This requires both a price-level aggregation (for best bid/ask) and a hash map from order ID to order (for fast cancels).

## Files

| File | Role |
|------|------|
| `order_book.hpp` | Your implementation — fill in the TODOs |
| `test_order_book.cpp` | Tests + benchmark (read-only) |

**Depends on ex1:** the Makefile passes `-I../ex1-price-levels` so you get `price_levels.hpp` automatically.

## Build & run

```bash
make run
```

## Key concepts

- `std::unordered_map<OrderId, Order>` gives O(1) cancel and O(1) `find_order`.
- `cancel_order` must also decrement the aggregate in `PriceLevelBook`.
- `modify_order` is semantically a cancel + re-add at the same price (same ID, no time-priority change).
