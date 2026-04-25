#ifndef BUMP_H
#define BUMP_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *buffer;      /* backing memory */
    size_t capacity;      /* total size of buffer */
    size_t offset;        /* current allocation offset (the "bump pointer") */
} bump_allocator_t;

/* Initialize a bump allocator with a pre-allocated buffer */
void bump_init(bump_allocator_t *a, void *buffer, size_t capacity);

/* Allocate `size` bytes with `align`-byte alignment. Returns NULL if out of space. */
void *bump_alloc(bump_allocator_t *a, size_t size, size_t align);

/* Convenience: allocate with default alignment (8 bytes) */
void *bump_alloc_default(bump_allocator_t *a, size_t size);

/* Reset the allocator (free everything at once) */
void bump_reset(bump_allocator_t *a);

/* How many bytes are currently in use? */
size_t bump_used(const bump_allocator_t *a);

/* How many bytes are remaining? */
size_t bump_remaining(const bump_allocator_t *a);

#endif /* BUMP_H */
