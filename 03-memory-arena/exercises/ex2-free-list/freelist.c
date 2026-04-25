/*
 * Free List Allocator
 *
 * Read freelist.h for the interface and the README for how free lists work.
 *
 * INCLUDES: Add what you need.
 */

/* TODO: add your #include lines here */

void freelist_init(freelist_allocator_t *a, void *buffer, size_t capacity) {
    a->buffer = (uint8_t *)buffer;
    a->capacity = capacity;
    a->first_block = (block_header_t *)buffer;
    a->first_block->size = capacity - HEADER_SIZE;
    a->first_block->is_free = 1;
    a->first_block->next = NULL;
}

/* TODO: Find a free block that fits, allocate from it. Split if the block is much larger. */
void *freelist_alloc(freelist_allocator_t *a, size_t size) {
    (void)a; (void)size;
    return NULL;
}

/* TODO: Free the block and coalesce with adjacent free blocks. */
void freelist_free(freelist_allocator_t *a, void *ptr) {
    (void)a; (void)ptr;
}

/* Provided: print the block list for debugging. */
void freelist_dump(const freelist_allocator_t *a) {
    printf("── Free List Dump ──\n");
    int i = 0;
    for (block_header_t *b = a->first_block; b != NULL; b = b->next) {
        printf("  [%d] %p: size=%-6zu %s  (next=%p)\n",
               i, (void *)b, b->size,
               b->is_free ? "FREE" : "USED",
               (void *)b->next);
        i++;
    }
    printf("── end (%d blocks) ──\n\n", i);
}
