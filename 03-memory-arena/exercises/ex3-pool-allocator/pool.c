#include "pool.h"
#include <string.h>

void pool_init(pool_allocator_t *p, void *buffer, size_t buffer_size, size_t block_size) {
    // TODO: Initialize the pool allocator.
    //
    // 1. Ensure block_size >= sizeof(void*) (we store the free list pointer in free blocks)
    //
    // 2. Calculate how many blocks fit: buffer_size / block_size
    //
    // 3. Build the embedded free list: for each block i (from 0 to block_count-1),
    //    store a pointer to the next free block in the block's first sizeof(void*) bytes:
    //      uint8_t *block = (uint8_t *)buffer + i * block_size;
    //      *(void **)block = <pointer to next block, or NULL for the last one>;
    //
    // 4. Set free_head to the first block (or NULL if block_count is 0)
    //
    // Think about it: we're using the FREE memory itself as our bookkeeping structure.
    // No extra metadata needed. This is elegant.
    p->buffer = (uint8_t *)buffer;
    p->block_size = (block_size < sizeof(void *)) ? sizeof(void *) : block_size;
    p->block_count = 0;
    p->free_head = NULL;
    p->allocated_count = 0;
    (void)buffer_size;
}

void *pool_alloc(pool_allocator_t *p) {
    // TODO: Pop a block from the free list.
    //
    // 1. If free_head is NULL, pool is exhausted -- return NULL
    //
    // 2. Save free_head as the block to return
    //
    // 3. Advance free_head to the next free block:
    //    p->free_head = *(void **)p->free_head;
    //
    // 4. Increment allocated_count
    //
    // 5. Return the block
    (void)p;
    return NULL;
}

void pool_free(pool_allocator_t *p, void *ptr) {
    // TODO: Push a block back onto the free list.
    //
    // 1. Store the current free_head pointer at the start of the block being freed:
    //    *(void **)ptr = p->free_head;
    //
    // 2. Set free_head to point to this block:
    //    p->free_head = ptr;
    //
    // 3. Decrement allocated_count
    //
    // This is a stack push operation. O(1). Beautiful.
    (void)p;
    (void)ptr;
}

void pool_reset(pool_allocator_t *p) {
    // TODO: Rebuild the entire free list, same as in pool_init.
    //
    // Reset allocated_count to 0, then thread all blocks back into the free list
    // exactly like pool_init does.
    //
    // Hint: you already wrote this logic in pool_init. Factor it out or just
    // repeat it here.
    (void)p;
}

size_t pool_available(const pool_allocator_t *p) {
    return p->block_count - p->allocated_count;
}

size_t pool_allocated(const pool_allocator_t *p) {
    return p->allocated_count;
}
