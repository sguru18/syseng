# Module 05 — Concurrency and Memory Model

`std::mutex` on the hot path can cost 100-1000 ns per acquisition under contention. Lock-free data structures using atomics and careful memory ordering eliminate that. These are the patterns that appear in every HFT inter-thread communication layer.

## Exercises

| # | Exercise | Concept |
|---|----------|---------|
| 1 | [ex1-seqlock](exercises/ex1-seqlock/) | Sequence lock — lock-free read-mostly sync |
| 2 | [ex2-spsc-queue](exercises/ex2-spsc-queue/) | SPSC ring buffer — zero-mutex thread comms |
| 3 | [ex3-mpmc-false-sharing](exercises/ex3-mpmc-false-sharing/) | MPMC queue + false sharing benchmark |
