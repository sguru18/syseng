# Exercise 1 — Price Levels

A market maker's core task: maintain the best bid/ask in real time as quotes arrive. The price level book is the read-mostly, write-sometimes structure at the heart of every market data handler.

## Files

| File | Role |
|------|------|
| `price_levels.hpp` | Your implementation — fill in the TODOs |
| `test_price_levels.cpp` | Tests + throughput benchmark (read-only) |

## Build & run

```bash
make run
```

## Key concepts

- Bids sorted **descending** → `std::map` with `std::greater<Price>` comparator; `begin()` is always the best bid.
- Asks sorted **ascending** → default `std::map`; `begin()` is always the best ask.
- `update(side, price, 0)` removes the level entirely.
