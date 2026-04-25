#ifndef FREELIST_H
#define FREELIST_H

#include <stddef.h>
#include <stdint.h>

/* Block header -- sits right before each allocated/free region */
typedef struct block_header {
    size_t size;                    /* size of the data region (not including header) */
    int is_free;                    /* 1 if free, 0 if allocated */
    struct block_header *next;      /* next block in the list (sequential in memory) */
} block_header_t;

typedef struct {
    uint8_t *buffer;                /* backing memory */
    size_t capacity;                /* total size */
    block_header_t *first_block;    /* head of the block list */
} freelist_allocator_t;

#define HEADER_SIZE (sizeof(block_header_t))

/* Initialize: set up one big free block spanning the entire buffer */
void freelist_init(freelist_allocator_t *a, void *buffer, size_t capacity);

/* Allocate size bytes. Returns NULL if no suitable block found. */
void *freelist_alloc(freelist_allocator_t *a, size_t size);

/* Free a previously allocated pointer. */
void freelist_free(freelist_allocator_t *a, void *ptr);

/* Debug: print the block list */
void freelist_dump(const freelist_allocator_t *a);

#endif /* FREELIST_H */
