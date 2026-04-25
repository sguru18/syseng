#include "bump.h"
#include <string.h>

void bump_init(bump_allocator_t *a, void *buffer, size_t capacity) {
    a->buffer = (uint8_t *)buffer;
    a->capacity = capacity;
    a->offset = 0;
}

void *bump_alloc(bump_allocator_t *a, size_t size, size_t align) {
    // TODO: Allocate `size` bytes with `align`-byte alignment.
    //
    // 1. Round up `a->offset` to the next multiple of `align`
    //    (What's the formula to round up x to the next multiple of a power-of-two n?
    //     Hint: (x + n - 1) & ~(n - 1))
    //
    // 2. Check if there's enough space: aligned_offset + size <= capacity
    //
    // 3. Save the aligned offset as the result pointer
    //
    // 4. Bump the offset past the allocation: offset = aligned_offset + size
    //
    // 5. Return the pointer: buffer + aligned_offset
    (void)a;
    (void)size;
    (void)align;
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
