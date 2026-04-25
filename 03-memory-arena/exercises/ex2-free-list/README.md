# Exercise 2: Free List Allocator

## The Problem

The bump allocator is fast, but it can't free individual objects. What if you need to allocate object A, allocate object B, then free A while keeping B alive?

You need an allocator that supports **individual frees**. Welcome to the classic malloc problem.

## The Approach: Explicit Free List

The idea is simple: maintain a **linked list of memory blocks**. Each block has a small header that records its size and whether it's free or allocated.

```
┌──────────┬────────────────────┬──────────┬──────────────┬──────────┬────────────────┐
│ header A │      data A        │ header B │    data B    │ header C │     data C     │
│ size=100 │    (allocated)     │ size=200 │   (free)     │ size=50  │  (allocated)   │
│ free=0   │                    │ free=1   │              │ free=0   │                │
│ next→B   │                    │ next→C   │              │ next=NULL│                │
└──────────┴────────────────────┴──────────┴──────────────┴──────────┴────────────────┘
```

**Alloc** walks the list looking for a free block that's big enough (first-fit). If the block is much larger than needed, **split** it: shrink the block to the requested size and create a new free block from the leftover space.

**Free** marks a block as free, then **coalesces** (merges) with adjacent free blocks to prevent fragmentation.

## Warm-Up

> You have three blocks: A (100 bytes, allocated), B (200 bytes, free), C (50 bytes, allocated).
>
> **Q1:** You call `alloc(80)`. Which block gets used? What happens to the remaining 120 bytes?
>
> **Q2:** You free block A. Now A (100 bytes) and the left part of old-B (80 bytes, now allocated) sit next to each other. A is free, but old-B's left part is allocated. Can they coalesce? Why not?
>
> **Q3:** Now you free old-B's left part too. Both A and the 80-byte block are free and adjacent. What should happen? What if you DON'T merge them? (Hint: you'd have two free blocks of 100 and 120 bytes, but you couldn't satisfy a 200-byte allocation even though 220 bytes are free. That's fragmentation.)

## Your Task

Open `freelist.c`. Two functions need implementing:

1. **`freelist_alloc`** -- Find a free block (first-fit), optionally split it, mark as allocated, return a pointer to the data region.

2. **`freelist_free`** -- Mark a block as free, then coalesce with adjacent free neighbors.

The `freelist_init` and `freelist_dump` functions are already implemented. Use `freelist_dump` liberally when debugging -- it prints the entire block list so you can see what's happening.

## Key Details

- The **header** (`block_header_t`) sits right before each data region in memory.
- When you return a pointer from `freelist_alloc`, it should point to the byte right after the header: `(void *)((uint8_t *)block + HEADER_SIZE)`.
- When the user calls `freelist_free(ptr)`, you recover the header: `(block_header_t *)((uint8_t *)ptr - HEADER_SIZE)`.
- **Splitting**: only split if the remainder is large enough for a header plus at least 1 byte of data. Otherwise, just give the whole block to the user (a few wasted bytes is better than a useless fragment).
- **Coalescing**: after freeing, check if the next block is free (merge forward). Then walk from the beginning to find the previous block and check if it's free (merge backward). You can optimize the backward walk later, but correctness first.

## Files

| File | What's in it |
|------|-------------|
| `freelist.h` | Data structure and function declarations |
| `freelist.c` | Implementation -- **fill in `freelist_alloc()` and `freelist_free()`** |
| `test_freelist.c` | Tests (run with `make test`) |

## Build & Test

```bash
make test    # compile and run tests
make clean   # remove build artifacts
```

## Hints

- For splitting: the new block header goes at `(uint8_t *)block + HEADER_SIZE + size`. Make sure there's room for a header AND at least 1 byte of data.
- For coalescing forward: if `block->next` exists and `block->next->is_free`, absorb it by adding its size + HEADER_SIZE to your block's size, and updating the `next` pointer.
- For coalescing backward: walk from `first_block` to find the block whose `next` is the block you just freed. If that predecessor is free, merge.
- Use `freelist_dump` to visualize the state after each operation. It's your best debugging tool.

## Real-World Connections

- **K&R malloc** (Section 8.7 of "The C Programming Language") uses exactly this approach
- **dlmalloc** (Doug Lea's allocator) and early **glibc malloc** are sophisticated versions of this pattern
- **Embedded systems** often use explicit free lists because they can't afford the overhead of a full malloc implementation
