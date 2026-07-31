# Exercise 2 — SPSC Ring Buffer

The SPSC queue is how every HFT system passes data between threads — from the network thread to the strategy thread, from the strategy to the order entry thread. It's lock-free by construction: the producer and consumer operate on different parts of the buffer and synchronize only through the head/tail pointers.

## Files

- `spsc_queue.hpp` — implement `push()`, `pop()`, `try_pop()`
- `test_spsc_queue.cpp` — correctness tests + throughput + latency benchmark

## Build & run

```
make
./test_spsc_queue
```
