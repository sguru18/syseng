# Exercise 3 -- Hot Path Allocation

`malloc` is banned on the hot path in production trading code. Not because it's slow on average -- it's slow when it decides to call the OS, scan its free list under contention, or fragment. Pool allocators eliminate this jitter. P99 latency is what kills strategies, not average.

## Parts

**Part 1 -- Arena allocator.** Pre-allocate a large contiguous buffer at startup. Each allocation advances a byte offset (with alignment padding). `reset()` rewinds the offset to zero in O(1). No OS calls, no free list, no fragmentation.

**Part 2 -- Object pool.** Pre-allocate N fixed-size slots. Embed a free list directly inside each free slot (intrusive). `allocate()` pops the head in O(1); `deallocate()` pushes back in O(1). Individual frees are supported, unlike the arena.

**Part 3 -- Benchmarks.** 1 million allocations and frees. Compare `new`/`delete`, arena, and pool on average latency *and* P99/P99.9 tail. The P99 gap is the point.

## Expected output (Apple M-series)

```
  dist  new/delete              avg=45ns  p50=30ns  p99=180ns  p99.9=4200ns
  dist  Arena                   avg= 3ns  p50= 3ns  p99=  4ns  p99.9=   5ns
  dist  ObjectPool              avg= 3ns  p50= 3ns  p99=  3ns  p99.9=   4ns
```

The p99/p99.9 gap is why arenas and pools exist.

## Build

```bash
make
./test_hot_alloc
```
