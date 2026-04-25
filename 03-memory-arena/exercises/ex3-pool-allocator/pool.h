#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *buffer;        /* backing memory */
    size_t block_size;      /* size of each block (must be >= sizeof(void*)) */
    size_t block_count;     /* total number of blocks */
    void *free_head;        /* head of the embedded free list (points into buffer) */
    size_t allocated_count; /* number of currently allocated blocks */
} pool_allocator_t;

/* Initialize: carve the buffer into fixed-size blocks and build the free list */
void pool_init(pool_allocator_t *p, void *buffer, size_t buffer_size, size_t block_size);

/* Allocate one block. Returns NULL if pool is exhausted. */
void *pool_alloc(pool_allocator_t *p);

/* Free one block back to the pool. */
void pool_free(pool_allocator_t *p, void *ptr);

/* Reset: return all blocks to the free list */
void pool_reset(pool_allocator_t *p);

/* Stats */
size_t pool_available(const pool_allocator_t *p);
size_t pool_allocated(const pool_allocator_t *p);

#endif /* POOL_H */
