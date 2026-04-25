# Module 03: Memory Arena

## Why Write Your Own Allocator?

`malloc` works. It handles any size, any lifetime, any thread. That generality has a cost: metadata overhead per allocation, lock contention, fragmentation, and unpredictable latency from coalescing and system calls.

But what if you *know* your allocation pattern?

- All allocations happen in a burst, then all are freed at once? **Bump allocator.** Zero per-allocation overhead.
- Objects come and go individually, but all from one buffer? **Free list.** Classic explicit allocator.
- Every object is exactly the same size? **Pool allocator.** O(1) alloc and free, zero fragmentation.

When you match the allocator to the pattern, you get **10-100x** better performance than general-purpose malloc. Not because malloc is bad -- because specialization always beats generalization.

## The Key Insight

Memory management is just bookkeeping. An allocator is a **data structure** that tracks which bytes are in use. The backing memory is just a big array. The allocator decides who gets which slice.

That's it. There's no magic. No OS kernel required. Just a buffer and some pointers.

## The Progression

The three exercises form a natural progression:

| Exercise | Allocator | Alloc Cost | Free Cost | Fragmentation | Complexity |
|----------|-----------|-----------|-----------|---------------|------------|
| 1 | **Bump** | O(1) | N/A (bulk reset) | None | Trivial |
| 2 | **Free List** | O(n) | O(n) | Possible | Moderate |
| 3 | **Pool** | O(1) | O(1) | None | Simple |

1. **Bump Allocator** -- The simplest possible allocator. One pointer, bumped forward. No individual frees. Start here to internalize the idea that "allocation" is just pointer arithmetic.

2. **Free List Allocator** -- The classic. Individual alloc and free with splitting and coalescing. This is the CS241 malloc lab distilled. You'll understand *why* fragmentation happens and what coalescing does about it.

3. **Pool Allocator** -- Fixed-size blocks, O(1) everything, zero fragmentation. The elegant trick: store the free list *inside* the free blocks themselves. This pattern powers the Linux kernel's slab allocator.

## Real-World Examples

These aren't toy exercises. These are the actual strategies used by production systems:

- **Game engines** use bump/frame allocators for per-frame scratch memory (allocate everything for one frame, reset, repeat)
- **Databases** use pool allocators for fixed-size page buffers and tuple storage
- **Language runtimes** (Go, Java, OCaml) use arena-style allocation for garbage-collected heaps
- **LLVM** has a `BumpPtrAllocator` that powers most of its IR data structures
- **Linux kernel** uses the slab allocator (`kmem_cache`) -- a pool allocator -- for kernel objects like `task_struct` and `inode`
- **nginx** uses a pool allocator for per-request memory management

## Getting Started

```bash
# Run all exercises
make test

# Or work on one at a time
cd exercises/ex1-bump-allocator && make test
```

Each exercise has its own README with context, warm-up questions, and hints. Start with Exercise 1.
