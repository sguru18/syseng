# Exercise 2 – Circular Buffer with STL Iterator

Ring buffers are everywhere in HFT: inter-thread queues, market data windows, rolling statistics. Making one STL-compatible means you get sorting, searching, and algorithms for free — with zero heap allocation.

## Real-World Relevance

- **Kafka-style circular log**: fixed-size window of the last N events, oldest auto-evicted
- **Rolling VWAP window**: sum and count over last N ticks, iterator feeds straight into `std::accumulate`
- **Order event queue**: SPSC ring buffer is the canonical inter-thread transport in low-latency systems

## What You Build

`CircularBuffer<T, N>`: a fixed-size, stack-allocated ring buffer with a proper `LegacyRandomAccessIterator`. Once the iterator is correct, the full STL algorithm library works on it for free.

Key milestones:
1. `push_back` with ring overwrite, `operator[]`, `size()`, `full()`, `clear()`
2. Iterator — all typedefs and operators for `std::random_access_iterator_tag`
3. `std::sort`, `std::find_if`, `std::accumulate`, `std::copy_n` all work on the buffer
4. Benchmark: compare 1M push throughput vs `std::deque`

## Running

```bash
make test
```
