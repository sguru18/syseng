/*
 * Pool Allocator
 *
 * Read pool.h for the interface and the README for the embedded free list trick.
 *
 * INCLUDES: Add what you need.
 */

/* TODO: add your #include lines here */

/* TODO: Initialize the pool. Carve buffer into blocks, build the embedded free list. */
void pool_init(pool_allocator_t *p, void *buffer, size_t buffer_size, size_t block_size) {
    p->buffer = (uint8_t *)buffer;
    p->block_size = (block_size < sizeof(void *)) ? sizeof(void *) : block_size;
    p->block_count = 0;
    p->free_head = NULL;
    p->allocated_count = 0;
    (void)buffer_size;
}

/* TODO: Pop a block from the free list. NULL if exhausted. */
void *pool_alloc(pool_allocator_t *p) {
    (void)p;
    return NULL;
}

/* TODO: Push a block back onto the free list. */
void pool_free(pool_allocator_t *p, void *ptr) {
    (void)p; (void)ptr;
}

/* TODO: Rebuild the free list (return all blocks to the pool). */
void pool_reset(pool_allocator_t *p) {
    (void)p;
}

size_t pool_available(const pool_allocator_t *p) {
    return p->block_count - p->allocated_count;
}

size_t pool_allocated(const pool_allocator_t *p) {
    return p->allocated_count;
}
