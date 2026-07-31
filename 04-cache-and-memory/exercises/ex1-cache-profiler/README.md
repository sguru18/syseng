# Exercise 1 -- Cache Profiler

Cache latency is the most important number in HFT. An L1 hit is ~1 ns. RAM is ~100 ns. That's 100x. A single cache miss on the hot path can blow your latency budget. This exercise makes you measure it yourself.

## Parts

**Part 1 -- Pointer-chasing latency table.** Build a linked list through an array, shuffle the traversal order, and time how long each pointer chase takes. When the whole array fits in L1 the hardware prefetcher is irrelevant -- you stay fast. As the array grows past each cache tier the latency jumps.

**Part 2 -- False sharing.** Two threads increment adjacent counters that share a cache line. Every write from one thread invalidates the other's cached copy, triggering coherence traffic. Padding the counters onto separate 64-byte lines eliminates the penalty.

## Expected output (Apple M-series)

```
=== Cache Hierarchy Profiler ===

Array size    | Latency per access | Likely location
--------------|--------------------|-----------------
        1 KB  |              ~1 ns | L1
        4 KB  |              ~1 ns | L1
       32 KB  |              ~3 ns | L1/L2
      256 KB  |              ~8 ns | L2/L3
        4 MB  |             ~30 ns | L3
       64 MB  |            ~100 ns | RAM

=== False Sharing ===
  Shared:  ~12 ns per iteration
  Padded:   ~2 ns per iteration
  Speedup: ~6x
```

## Build

```bash
make
./cache_profiler
```
