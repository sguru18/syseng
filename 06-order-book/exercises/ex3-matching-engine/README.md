# Exercise 3 — Matching Engine

A matching engine receives order events over a network feed and must match incoming aggressive orders against resting limit orders in microseconds. This exercise connects everything: the order book, fill generation, and event-driven processing.

## Files

| File | Role |
|------|------|
| `matching_engine.hpp` | Your implementation — fill in the TODOs |
| `test_matching_engine.cpp` | Tests + 100k-event benchmark (read-only) |

**Depends on ex1 + ex2:** the Makefile passes both include paths; `matching_engine.hpp` already includes `order_book.hpp`.

## Build & run

```bash
make run
```

## Key concepts

- **Price-time priority:** match at the best (tightest) price first; within a price, first-in first-out.
- **Lazy deletion:** keep per-level FIFO queues of order IDs in the engine; skip any ID that's no longer in the book (was cancelled or fully filled). This avoids O(n) queue scans on every cancel.
- **Fill price = passive order's price**, not the aggressor's price.
- **Aggressor** = the incoming order; **passive** = the resting order it matches against.

## Matching rule (simplified price-time priority)

```
Buy  at P → match against resting sells where ask_price <= P  (best ask first)
Sell at P → match against resting buys  where bid_price >= P  (best bid first)
```

After matching, any unfilled remainder of the incoming order is added as a new resting order.
