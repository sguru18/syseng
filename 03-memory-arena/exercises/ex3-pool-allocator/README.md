# Exercise 3: Pool Allocator

## The Insight

What if every object you allocate is exactly the same size?

Suddenly the problem gets *much* simpler. No splitting. No coalescing. No fragmentation. Just a stack of free blocks: alloc pops one off, free pushes one back. Both O(1).

This is a **pool allocator** (also called a slab allocator), and it's one of the most widely used allocation strategies in systems programming.

## The Elegant Trick

Where do you store the free list? In a free-list allocator, each block has a header. But pool blocks are fixed-size -- adding a header wastes space and complicates the layout.

The trick: **store the free list pointer inside the free block itself.** A free block isn't being used for anything, so its first `sizeof(void *)` bytes can hold a pointer to the next free block. When the block gets allocated, that pointer gets overwritten with real data. When the block is freed, you write the pointer back.

No extra metadata. The bookkeeping structure lives *inside* the data it manages. This is elegant.

```
 Buffer (5 blocks of 64 bytes each):
 ┌────────────────┬────────────────┬────────────────┬────────────────┬────────────────┐
 │ block 0        │ block 1        │ block 2        │ block 3        │ block 4        │
 │ [next→1]       │ [next→2]       │ [ALLOCATED]    │ [next→4]       │ [next→NULL]    │
 └────────────────┴────────────────┴────────────────┴────────────────┴────────────────┘
   ↑ free_head

 Free list: 0 → 1 → 3 → 4 → NULL    (block 2 is allocated, not in the list)
```

## Warm-Up

> You have 10 blocks of 64 bytes each. The free list looks like: `head → 3 → 7 → 1 → 0 → NULL`.
>
> **Q1:** You call `pool_alloc()`. Which block do you get? What does the free list look like after?
>
> **Q2:** You call `pool_free(block 5)`. What does the free list look like now?
>
> **Q3:** Why is this a LIFO (stack) structure? What are the implications for cache behavior? (Hint: recently freed blocks are "hot" in the CPU cache.)

## Your Task

Open `pool.c`. Four functions need implementing:

1. **`pool_init`** -- Carve the buffer into equal-sized blocks and thread them into a free list. Each free block's first `sizeof(void *)` bytes point to the next free block.

2. **`pool_alloc`** -- Pop a block from the free list head. One pointer read, one pointer write. Done.

3. **`pool_free`** -- Push a block onto the free list head. One pointer write, one pointer update. Done.

4. **`pool_reset`** -- Rebuild the free list from scratch (same as init).

The `pool_available` and `pool_allocated` stat functions are already implemented.

## Key Details

- `block_size` must be at least `sizeof(void *)` (you need room for the embedded free list pointer). The init function should enforce this.
- `block_count = buffer_size / block_size`. Any leftover bytes at the end of the buffer are wasted (that's fine).
- Block `i` starts at `buffer + i * block_size`.
- To read/write the embedded pointer in a block at address `addr`:
  ```c
  void *next = *(void **)addr;         // read
  *(void **)addr = some_pointer;       // write
  ```

## Files

| File | What's in it |
|------|-------------|
| `pool.h` | Data structure and function declarations |
| `pool.c` | Implementation -- **fill in `pool_init()`, `pool_alloc()`, `pool_free()`, `pool_reset()`** |
| `test_pool.c` | Tests (run with `make test`) |

## Build & Test

```bash
make test    # compile and run tests
make clean   # remove build artifacts
```

## Real-World Connections

- **Linux slab allocator** (`kmem_cache_create` / `kmem_cache_alloc`): the kernel uses pool allocators for every fixed-size object -- `task_struct`, `inode`, `dentry`, `sk_buff`, etc. This is the single most important allocator in the kernel.
- **nginx**: uses a pool allocator for per-connection memory management
- **Game engines**: entity-component systems allocate components (Transform, Rigidbody, Sprite) from per-type pools for cache-friendly iteration
- **Database systems**: tuple storage in row-oriented databases often uses fixed-size pools for each table's row format
- **Apache**: apr_pool for per-request allocation with O(1) cleanup
