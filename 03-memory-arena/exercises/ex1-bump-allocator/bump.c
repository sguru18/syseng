/*
 * Bump Allocator
 *
 * Read bump.h for the interface and the README for how bump allocation works.
 *
 * INCLUDES: You know the drill by now.
 */

/* TODO: add your #include lines here */

void bump_init(bump_allocator_t *a, void *buffer, size_t capacity) {
    a->buffer = (uint8_t *)buffer;
    a->capacity = capacity;
    a->offset = 0;
}

/* TODO: Allocate size bytes with align-byte alignment. Return NULL if out of space. */
void *bump_alloc(bump_allocator_t *a, size_t size, size_t align) {
    (void)a; (void)size; (void)align;
    return NULL;
}

void *bump_alloc_default(bump_allocator_t *a, size_t size) {
    return bump_alloc(a, size, 8);
}

void bump_reset(bump_allocator_t *a) {
    a->offset = 0;
}

size_t bump_used(const bump_allocator_t *a) {
    return a->offset;
}

size_t bump_remaining(const bump_allocator_t *a) {
    return a->capacity - a->offset;
}
