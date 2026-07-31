# Exercise 1 – Container Benchmark

HFT interviews ask "which container would you use for X?" The answer is never "it depends" — it's "vector for N<100, unordered_map otherwise, because cache locality and O(1) amortized." This exercise gives you the empirical data to back that claim.

## Real-World Relevance

- **Order book price levels**: sorted vector (cache-friendly scan) vs map (ordered, O(log n)) vs unordered_map (O(1) lookup)
- **Symbol table**: intern 10k symbols once, look them up millions of times → unordered_map wins
- **Tick history buffer**: append-only, sequential reads → deque or vector dominates

## What You Build

Implement a benchmark harness that measures **insert**, **lookup**, and **iteration** for five containers at N=100,000. Run it. Read the numbers. Answer 4 questions in comments at the bottom of the file.

Containers under test:
- `std::vector` (sorted + `lower_bound` lookup)
- `std::map`
- `std::unordered_map`
- `std::list`
- `std::deque`

## Running

```bash
make test
```

Expected output: a formatted table of `ns/op` for each container and operation.
