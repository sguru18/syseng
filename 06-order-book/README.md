# Module 06 — Order Book & Matching Engine

The limit order book is the core data structure of every exchange and market-maker. This module builds one from scratch, then adds a matching engine. By the end you can process a stream of orders, manage a live book, and generate fills — the actual product of HFT trading systems.

## Exercises

| # | Directory | What you build |
|---|-----------|----------------|
| 1 | `ex1-price-levels` | `PriceLevelBook` — O(log n) bid/ask aggregation, O(1) best bid/ask |
| 2 | `ex2-limit-order-book` | `LimitOrderBook` — adds per-order tracking with O(1) cancel by ID |
| 3 | `ex3-matching-engine` | `MatchingEngine` — price-time priority matching, fill generation |

Each exercise **builds on the previous one**: ex2 includes ex1's header, ex3 includes ex2's.

## Build

```bash
make          # build all three exercises
make run      # run all test suites
make clean    # clean all binaries
```
