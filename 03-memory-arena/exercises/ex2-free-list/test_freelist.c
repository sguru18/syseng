#include "common.h"
#include "freelist.h"

static uint8_t backing[4096] __attribute__((aligned(16)));

/* ── basic alloc ── */

void test_basic_alloc(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *p = freelist_alloc(&a, 100);
    ASSERT_NOT_NULL(p);
    ASSERT_TRUE((uint8_t *)p >= backing);
    ASSERT_TRUE((uint8_t *)p + 100 <= backing + sizeof(backing));
}

/* ── basic alloc and free ── */

void test_alloc_and_free(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *p = freelist_alloc(&a, 100);
    ASSERT_NOT_NULL(p);
    freelist_free(&a, p);
    /* after free, we should be able to allocate the full buffer again */
    void *p2 = freelist_alloc(&a, sizeof(backing) - HEADER_SIZE);
    ASSERT_NOT_NULL(p2);
}

/* ── multiple allocations ── */

void test_multiple_allocs(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *p1 = freelist_alloc(&a, 100);
    void *p2 = freelist_alloc(&a, 200);
    void *p3 = freelist_alloc(&a, 300);
    ASSERT_NOT_NULL(p1);
    ASSERT_NOT_NULL(p2);
    ASSERT_NOT_NULL(p3);

    /* no overlaps */
    ASSERT_TRUE((uint8_t *)p2 >= (uint8_t *)p1 + 100);
    ASSERT_TRUE((uint8_t *)p3 >= (uint8_t *)p2 + 200);
}

/* ── free and reuse ── */

void test_free_and_reuse(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *p1 = freelist_alloc(&a, 100);
    ASSERT_NOT_NULL(p1);
    freelist_free(&a, p1);

    void *p2 = freelist_alloc(&a, 100);
    ASSERT_NOT_NULL(p2);
    /* first-fit should reuse the same block */
    ASSERT_EQ((long long)(uintptr_t)p1, (long long)(uintptr_t)p2);
}

/* ── splitting ── */

void test_splitting(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    /* allocate a small block from the big free block */
    void *p1 = freelist_alloc(&a, 64);
    ASSERT_NOT_NULL(p1);

    /* there should still be plenty of room for another allocation */
    void *p2 = freelist_alloc(&a, 64);
    ASSERT_NOT_NULL(p2);

    /* both should be distinct */
    ASSERT_TRUE(p1 != p2);
}

/* ── coalescing forward: free B then C ── */

void test_coalesce_forward(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *pA = freelist_alloc(&a, 100);
    void *pB = freelist_alloc(&a, 200);
    void *pC = freelist_alloc(&a, 300);
    ASSERT_NOT_NULL(pA);
    ASSERT_NOT_NULL(pB);
    ASSERT_NOT_NULL(pC);

    freelist_free(&a, pB);
    freelist_free(&a, pC);

    /* B and C should have coalesced into a block large enough for 500+HEADER_SIZE bytes */
    void *pBig = freelist_alloc(&a, 500 + HEADER_SIZE);
    ASSERT_NOT_NULL(pBig);
}

/* ── coalescing backward: free B then A ── */

void test_coalesce_backward(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *pA = freelist_alloc(&a, 100);
    void *pB = freelist_alloc(&a, 200);
    void *pC = freelist_alloc(&a, 300);
    ASSERT_NOT_NULL(pA);
    ASSERT_NOT_NULL(pB);
    ASSERT_NOT_NULL(pC);

    freelist_free(&a, pA);
    freelist_free(&a, pB);

    /* A and B should have coalesced: room for 300+HEADER_SIZE */
    void *pBig = freelist_alloc(&a, 300 + HEADER_SIZE);
    ASSERT_NOT_NULL(pBig);
}

/* ── coalescing both directions ── */

void test_coalesce_both(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *pA = freelist_alloc(&a, 100);
    void *pB = freelist_alloc(&a, 200);
    void *pC = freelist_alloc(&a, 300);
    ASSERT_NOT_NULL(pA);
    ASSERT_NOT_NULL(pB);
    ASSERT_NOT_NULL(pC);

    freelist_free(&a, pA);
    freelist_free(&a, pC);
    freelist_free(&a, pB);  /* should coalesce with both A (before) and C (after) */

    /* all three should be one big block now */
    void *pBig = freelist_alloc(&a, 600 + 2 * HEADER_SIZE);
    ASSERT_NOT_NULL(pBig);
}

/* ── fragmentation: alternating frees can't satisfy large alloc ── */

void test_fragmentation(void) {
    freelist_allocator_t a;
    uint8_t small_buf[512] __attribute__((aligned(16)));
    freelist_init(&a, small_buf, sizeof(small_buf));

    /* allocate 8 blocks of 32 bytes each */
    void *ptrs[8];
    for (int i = 0; i < 8; i++) {
        ptrs[i] = freelist_alloc(&a, 32);
        ASSERT_NOT_NULL(ptrs[i]);
    }

    /* free every other block: creates non-adjacent holes */
    for (int i = 0; i < 8; i += 2) {
        freelist_free(&a, ptrs[i]);
    }

    /* try to alloc a block larger than any individual hole */
    void *big = freelist_alloc(&a, 64);
    /* should fail because free blocks aren't adjacent */
    ASSERT_NULL(big);
}

/* ── out of space ── */

void test_out_of_space(void) {
    freelist_allocator_t a;
    uint8_t small_buf[128] __attribute__((aligned(16)));
    freelist_init(&a, small_buf, sizeof(small_buf));

    void *p = freelist_alloc(&a, 128);
    /* can't fit 128 data bytes + header in 128 total bytes */
    ASSERT_NULL(p);
}

/* ── data integrity across alloc/free ── */

void test_data_integrity(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    int *x = (int *)freelist_alloc(&a, sizeof(int));
    int *y = (int *)freelist_alloc(&a, sizeof(int));
    ASSERT_NOT_NULL(x);
    ASSERT_NOT_NULL(y);

    *x = 12345;
    *y = 67890;

    /* free x, allocate something else, y should be undisturbed */
    freelist_free(&a, x);
    void *z = freelist_alloc(&a, sizeof(int));
    ASSERT_NOT_NULL(z);

    ASSERT_EQ(67890, *y);
}

/* ── stress: alloc/free in a loop ── */

void test_stress_alloc_free(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *ptrs[100];
    memset(ptrs, 0, sizeof(ptrs));

    for (int round = 0; round < 50; round++) {
        /* allocate a batch */
        for (int i = 0; i < 10; i++) {
            ptrs[i] = freelist_alloc(&a, 32);
            if (ptrs[i]) {
                memset(ptrs[i], 0xAB, 32);
            }
        }
        /* free them all */
        for (int i = 0; i < 10; i++) {
            if (ptrs[i]) {
                freelist_free(&a, ptrs[i]);
                ptrs[i] = NULL;
            }
        }
    }

    /* after freeing everything, the whole buffer should be available again */
    void *big = freelist_alloc(&a, sizeof(backing) - HEADER_SIZE);
    ASSERT_NOT_NULL(big);
}

/* ── alloc returns data right after header ── */

void test_alloc_pointer_position(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    void *p = freelist_alloc(&a, 64);
    ASSERT_NOT_NULL(p);
    /* pointer should be HEADER_SIZE bytes into the buffer */
    ASSERT_EQ((long long)(uintptr_t)(backing + HEADER_SIZE), (long long)(uintptr_t)p);
}

/* ── write patterns and verify no corruption ── */

void test_write_patterns(void) {
    freelist_allocator_t a;
    freelist_init(&a, backing, sizeof(backing));

    uint8_t *p1 = (uint8_t *)freelist_alloc(&a, 128);
    uint8_t *p2 = (uint8_t *)freelist_alloc(&a, 128);
    ASSERT_NOT_NULL(p1);
    ASSERT_NOT_NULL(p2);

    memset(p1, 0xAA, 128);
    memset(p2, 0xBB, 128);

    /* verify patterns */
    for (int i = 0; i < 128; i++) {
        ASSERT_EQ(0xAA, p1[i]);
        ASSERT_EQ(0xBB, p2[i]);
    }
}

int main(void) {
    TEST_BEGIN();
    RUN_TEST(test_basic_alloc);
    RUN_TEST(test_alloc_and_free);
    RUN_TEST(test_multiple_allocs);
    RUN_TEST(test_free_and_reuse);
    RUN_TEST(test_splitting);
    RUN_TEST(test_coalesce_forward);
    RUN_TEST(test_coalesce_backward);
    RUN_TEST(test_coalesce_both);
    RUN_TEST(test_fragmentation);
    RUN_TEST(test_out_of_space);
    RUN_TEST(test_data_integrity);
    RUN_TEST(test_stress_alloc_free);
    RUN_TEST(test_alloc_pointer_position);
    RUN_TEST(test_write_patterns);
    TEST_END();
}
