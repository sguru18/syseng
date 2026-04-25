#include "common.h"
#include "bump.h"

static uint8_t backing[4096] __attribute__((aligned(16)));

/* ── basic allocation ── */

void test_basic_alloc(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    void *p = bump_alloc(&a, 10, 1);
    ASSERT_NOT_NULL(p);
    ASSERT_TRUE((uint8_t *)p >= backing);
    ASSERT_TRUE((uint8_t *)p + 10 <= backing + sizeof(backing));
}

void test_alloc_returns_start_of_buffer(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    void *p = bump_alloc(&a, 16, 1);
    ASSERT_EQ((long long)(uintptr_t)backing, (long long)(uintptr_t)p);
}

/* ── sequential allocations don't overlap ── */

void test_sequential_no_overlap(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    void *p1 = bump_alloc(&a, 32, 1);
    void *p2 = bump_alloc(&a, 64, 1);
    ASSERT_NOT_NULL(p1);
    ASSERT_NOT_NULL(p2);
    ASSERT_TRUE((uint8_t *)p2 >= (uint8_t *)p1 + 32);
}

/* ── alignment (8-byte) ── */

void test_alignment_8(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    bump_alloc(&a, 1, 1);  /* offset is now 1 */
    void *p = bump_alloc(&a, 16, 8);
    ASSERT_NOT_NULL(p);
    ASSERT_EQ(0, (uintptr_t)p % 8);
}

/* ── alignment (16-byte) ── */

void test_alignment_16(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    bump_alloc(&a, 3, 1);  /* offset is now 3 */
    void *p = bump_alloc(&a, 32, 16);
    ASSERT_NOT_NULL(p);
    ASSERT_EQ(0, (uintptr_t)p % 16);
}

/* ── alignment (64-byte) ── */

void test_alignment_64(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    bump_alloc(&a, 7, 1);
    void *p = bump_alloc(&a, 10, 64);
    ASSERT_NOT_NULL(p);
    ASSERT_EQ(0, (uintptr_t)p % 64);
}

/* ── default alignment is 8 ── */

void test_default_alignment(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    bump_alloc(&a, 1, 1);
    void *p = bump_alloc_default(&a, 16);
    ASSERT_NOT_NULL(p);
    ASSERT_EQ(0, (uintptr_t)p % 8);
}

/* ── out of space returns NULL ── */

void test_out_of_space(void) {
    bump_allocator_t a;
    uint8_t small_buf[64];
    bump_init(&a, small_buf, sizeof(small_buf));

    void *p1 = bump_alloc(&a, 60, 1);
    ASSERT_NOT_NULL(p1);

    void *p2 = bump_alloc(&a, 10, 1);
    ASSERT_NULL(p2);
}

void test_exact_fit(void) {
    bump_allocator_t a;
    uint8_t small_buf[64];
    bump_init(&a, small_buf, sizeof(small_buf));

    void *p = bump_alloc(&a, 64, 1);
    ASSERT_NOT_NULL(p);
    ASSERT_EQ(0, bump_remaining(&a));

    void *p2 = bump_alloc(&a, 1, 1);
    ASSERT_NULL(p2);
}

/* ── out of space due to alignment padding ── */

void test_alignment_causes_oom(void) {
    bump_allocator_t a;
    uint8_t small_buf[32];
    bump_init(&a, small_buf, sizeof(small_buf));

    bump_alloc(&a, 25, 1);  /* 7 bytes left */
    void *p = bump_alloc(&a, 1, 16);  /* needs to align to 16, might push past end */
    if (p != NULL) {
        ASSERT_EQ(0, (uintptr_t)p % 16);
        ASSERT_TRUE((uint8_t *)p + 1 <= small_buf + sizeof(small_buf));
    }
}

/* ── reset lets you reallocate ── */

void test_reset(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    bump_alloc(&a, 100, 1);
    ASSERT_EQ(100, bump_used(&a));

    bump_reset(&a);
    ASSERT_EQ(0, bump_used(&a));
    ASSERT_EQ(sizeof(backing), bump_remaining(&a));

    void *p = bump_alloc(&a, 100, 1);
    ASSERT_NOT_NULL(p);
    ASSERT_EQ((long long)(uintptr_t)backing, (long long)(uintptr_t)p);
}

/* ── zero-size allocation ── */

void test_zero_size_alloc(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    void *p = bump_alloc(&a, 0, 1);
    /* zero-size should succeed (returns current position) or return NULL -- either is acceptable */
    /* but it must not corrupt the allocator */
    void *p2 = bump_alloc(&a, 16, 1);
    ASSERT_NOT_NULL(p2);
    (void)p;
}

/* ── used/remaining tracking ── */

void test_used_remaining(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    ASSERT_EQ(0, bump_used(&a));
    ASSERT_EQ(sizeof(backing), bump_remaining(&a));

    bump_alloc(&a, 100, 1);
    ASSERT_EQ(100, bump_used(&a));
    ASSERT_EQ(sizeof(backing) - 100, bump_remaining(&a));

    bump_alloc(&a, 200, 1);
    ASSERT_EQ(300, bump_used(&a));
    ASSERT_EQ(sizeof(backing) - 300, bump_remaining(&a));
}

/* ── used accounts for alignment padding ── */

void test_used_includes_padding(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    bump_alloc(&a, 1, 1);      /* offset = 1 */
    bump_alloc(&a, 1, 8);      /* offset aligns to 8, then +1 = 9 */
    ASSERT_EQ(9, bump_used(&a));
}

/* ── stress test: 1000 small allocations ── */

void test_stress_many_allocs(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    void *ptrs[1000];
    int count = 0;

    for (int i = 0; i < 1000; i++) {
        void *p = bump_alloc(&a, 1, 1);
        if (p == NULL) break;
        ptrs[count++] = p;
    }

    ASSERT_TRUE(count > 0);

    /* all pointers are within bounds */
    for (int i = 0; i < count; i++) {
        ASSERT_TRUE((uint8_t *)ptrs[i] >= backing);
        ASSERT_TRUE((uint8_t *)ptrs[i] < backing + sizeof(backing));
    }

    /* all pointers are distinct */
    for (int i = 1; i < count; i++) {
        ASSERT_TRUE(ptrs[i] != ptrs[i - 1]);
    }
}

/* ── stress test: mixed sizes with alignment ── */

void test_stress_mixed_sizes(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    size_t sizes[]  = {1, 7, 16, 3, 64, 8, 100, 1, 33, 256};
    size_t aligns[] = {1, 4, 16, 1, 64, 8,   8, 1,  8,  16};
    int n = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < n; i++) {
        void *p = bump_alloc(&a, sizes[i], aligns[i]);
        ASSERT_NOT_NULL(p);
        ASSERT_EQ(0, (uintptr_t)p % aligns[i]);
        /* verify we can write to the allocated memory */
        memset(p, 0xAB, sizes[i]);
    }
}

/* ── write data and verify it persists ── */

void test_data_integrity(void) {
    bump_allocator_t a;
    bump_init(&a, backing, sizeof(backing));

    int *x = (int *)bump_alloc(&a, sizeof(int), sizeof(int));
    int *y = (int *)bump_alloc(&a, sizeof(int), sizeof(int));
    ASSERT_NOT_NULL(x);
    ASSERT_NOT_NULL(y);

    *x = 42;
    *y = 99;
    ASSERT_EQ(42, *x);
    ASSERT_EQ(99, *y);
}

int main(void) {
    TEST_BEGIN();
    RUN_TEST(test_basic_alloc);
    RUN_TEST(test_alloc_returns_start_of_buffer);
    RUN_TEST(test_sequential_no_overlap);
    RUN_TEST(test_alignment_8);
    RUN_TEST(test_alignment_16);
    RUN_TEST(test_alignment_64);
    RUN_TEST(test_default_alignment);
    RUN_TEST(test_out_of_space);
    RUN_TEST(test_exact_fit);
    RUN_TEST(test_alignment_causes_oom);
    RUN_TEST(test_reset);
    RUN_TEST(test_zero_size_alloc);
    RUN_TEST(test_used_remaining);
    RUN_TEST(test_used_includes_padding);
    RUN_TEST(test_stress_many_allocs);
    RUN_TEST(test_stress_mixed_sizes);
    RUN_TEST(test_data_integrity);
    TEST_END();
}
