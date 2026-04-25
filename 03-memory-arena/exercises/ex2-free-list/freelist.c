#include "freelist.h"
#include <stdio.h>
#include <string.h>

void freelist_init(freelist_allocator_t *a, void *buffer, size_t capacity) {
    a->buffer = (uint8_t *)buffer;
    a->capacity = capacity;

    /* Create one big free block spanning the entire buffer (minus its header) */
    a->first_block = (block_header_t *)buffer;
    a->first_block->size = capacity - HEADER_SIZE;
    a->first_block->is_free = 1;
    a->first_block->next = NULL;
}

void *freelist_alloc(freelist_allocator_t *a, size_t size) {
    // TODO: Find a free block that fits, allocate from it.
    //
    // 1. Walk the block list looking for a free block with block->size >= size
    //    (This is "first-fit". Think about: why not "best-fit"?)
    //
    // 2. If the block is much larger than needed, SPLIT it:
    //    - The minimum remainder for a split is HEADER_SIZE + 1 (room for a new
    //      header plus at least 1 byte of usable data)
    //    - Create the new free block at: (uint8_t *)block + HEADER_SIZE + size
    //    - Set its size to: block->size - size - HEADER_SIZE
    //    - Wire up the next pointers: new_block->next = block->next; block->next = new_block
    //    - Shrink block->size to exactly `size`
    //
    // 3. Mark the block as allocated (is_free = 0)
    //
    // 4. Return a pointer to the data region (right after the header):
    //    (void *)((uint8_t *)block + HEADER_SIZE)
    (void)a;
    (void)size;
    return NULL;
}

void freelist_free(freelist_allocator_t *a, void *ptr) {
    // TODO: Free the block and coalesce with adjacent free blocks.
    //
    // 1. Get the block header from the user pointer:
    //    block_header_t *block = (block_header_t *)((uint8_t *)ptr - HEADER_SIZE);
    //
    // 2. Mark it as free (is_free = 1)
    //
    // 3. Coalesce FORWARD: if block->next exists and block->next->is_free:
    //    - Absorb the next block: block->size += HEADER_SIZE + block->next->size
    //    - Skip over it: block->next = block->next->next
    //
    // 4. Coalesce BACKWARD: walk from a->first_block to find the block `prev`
    //    whose prev->next == block. If prev exists and prev->is_free:
    //    - Absorb: prev->size += HEADER_SIZE + block->size
    //    - Skip: prev->next = block->next
    (void)a;
    (void)ptr;
}

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
