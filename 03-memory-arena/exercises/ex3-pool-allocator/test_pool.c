#include "common.h"
#include "pool.h"

#define BLOCK_SIZE 64
#define NUM_BLOCKS 32
static uint8_t backing[BLOCK_SIZE * NUM_BLOCKS] __attribute__((aligned(16)));

/* ── init and stats ── */

void test_init_stats(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    ASSERT_EQ(NUM_BLOCKS, p.block_count);
    ASSERT_EQ(NUM_BLOCKS, pool_available(&p));
    ASSERT_EQ(0, pool_allocated(&p));
}

void test_block_size_minimum(void) {
    pool_allocator_t p;
    /* block_size smaller than sizeof(void*) should be clamped up */
    pool_init(&p, backing, sizeof(backing), 1);
    ASSERT_TRUE(p.block_size >= sizeof(void *));
}

/* ── single alloc ── */

void test_alloc_one(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    void *blk = pool_alloc(&p);
    ASSERT_NOT_NULL(blk);
    ASSERT_EQ(NUM_BLOCKS - 1, pool_available(&p));
    ASSERT_EQ(1, pool_allocated(&p));
}

void test_alloc_within_bounds(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    void *blk = pool_alloc(&p);
    ASSERT_NOT_NULL(blk);
    ASSERT_TRUE((uint8_t *)blk >= backing);
    ASSERT_TRUE((uint8_t *)blk + BLOCK_SIZE <= backing + sizeof(backing));
}

/* ── allocate all blocks ── */

void test_alloc_all(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    void *ptrs[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; i++) {
        ptrs[i] = pool_alloc(&p);
        ASSERT_NOT_NULL(ptrs[i]);
    }

    ASSERT_EQ(0, pool_available(&p));
    ASSERT_EQ(NUM_BLOCKS, pool_allocated(&p));

    /* all pointers should be distinct */
    for (int i = 0; i < NUM_BLOCKS; i++) {
        for (int j = i + 1; j < NUM_BLOCKS; j++) {
            ASSERT_TRUE(ptrs[i] != ptrs[j]);
        }
    }
}

/* ── exhaustion returns NULL ── */

void test_exhaustion(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    for (int i = 0; i < NUM_BLOCKS; i++) {
        ASSERT_NOT_NULL(pool_alloc(&p));
    }

    void *extra = pool_alloc(&p);
    ASSERT_NULL(extra);
    ASSERT_EQ(NUM_BLOCKS, pool_allocated(&p));
}

/* ── free and realloc ── */

void test_free_and_realloc(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    void *blk = pool_alloc(&p);
    ASSERT_NOT_NULL(blk);
    pool_free(&p, blk);
    ASSERT_EQ(NUM_BLOCKS, pool_available(&p));
    ASSERT_EQ(0, pool_allocated(&p));

    void *blk2 = pool_alloc(&p);
    ASSERT_NOT_NULL(blk2);
}

/* ── LIFO behavior: last freed is first allocated ── */

void test_lifo_order(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    void *a = pool_alloc(&p);
    void *b = pool_alloc(&p);
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(b);

    pool_free(&p, a);
    pool_free(&p, b);

    /* LIFO: b was freed last, so it should be allocated first */
    void *c = pool_alloc(&p);
    ASSERT_EQ((long long)(uintptr_t)b, (long long)(uintptr_t)c);

    void *d = pool_alloc(&p);
    ASSERT_EQ((long long)(uintptr_t)a, (long long)(uintptr_t)d);
}

/* ── reset ── */

void test_reset(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    /* exhaust the pool */
    for (int i = 0; i < NUM_BLOCKS; i++) {
        ASSERT_NOT_NULL(pool_alloc(&p));
    }
    ASSERT_EQ(0, pool_available(&p));

    pool_reset(&p);
    ASSERT_EQ(NUM_BLOCKS, pool_available(&p));
    ASSERT_EQ(0, pool_allocated(&p));

    /* should be able to allocate all blocks again */
    for (int i = 0; i < NUM_BLOCKS; i++) {
        ASSERT_NOT_NULL(pool_alloc(&p));
    }
    ASSERT_EQ(0, pool_available(&p));
}

/* ── stats remain consistent under random alloc/free ── */

void test_stress_stats_consistent(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    void *ptrs[NUM_BLOCKS];
    memset(ptrs, 0, sizeof(ptrs));
    int alloc_count = 0;

    /* pseudo-random alloc/free pattern */
    for (int round = 0; round < 500; round++) {
        if (alloc_count < NUM_BLOCKS && (round % 3 != 0 || alloc_count == 0)) {
            void *blk = pool_alloc(&p);
            if (blk) {
                ptrs[alloc_count++] = blk;
            }
        } else if (alloc_count > 0) {
            alloc_count--;
            pool_free(&p, ptrs[alloc_count]);
            ptrs[alloc_count] = NULL;
        }

        ASSERT_EQ(alloc_count, (int)pool_allocated(&p));
        ASSERT_EQ(NUM_BLOCKS - alloc_count, (int)pool_available(&p));
    }
}

/* ── data integrity: writing to allocated blocks doesn't corrupt others ── */

void test_data_integrity(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    uint8_t *blk1 = (uint8_t *)pool_alloc(&p);
    uint8_t *blk2 = (uint8_t *)pool_alloc(&p);
    uint8_t *blk3 = (uint8_t *)pool_alloc(&p);
    ASSERT_NOT_NULL(blk1);
    ASSERT_NOT_NULL(blk2);
    ASSERT_NOT_NULL(blk3);

    /* write distinct patterns */
    memset(blk1, 0xAA, BLOCK_SIZE);
    memset(blk2, 0xBB, BLOCK_SIZE);
    memset(blk3, 0xCC, BLOCK_SIZE);

    /* free blk2, verify blk1 and blk3 are undisturbed */
    pool_free(&p, blk2);

    for (int i = 0; i < BLOCK_SIZE; i++) {
        ASSERT_EQ(0xAA, blk1[i]);
        ASSERT_EQ(0xCC, blk3[i]);
    }
}

/* ── all blocks are aligned to block_size boundaries within the buffer ── */

void test_block_alignment(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    for (int i = 0; i < NUM_BLOCKS; i++) {
        void *blk = pool_alloc(&p);
        ASSERT_NOT_NULL(blk);
        /* each block should start at buffer + k*BLOCK_SIZE for some k */
        size_t offset = (uint8_t *)blk - backing;
        ASSERT_EQ(0, offset % BLOCK_SIZE);
    }
}

/* ── free updates stats correctly ── */

void test_free_stats(void) {
    pool_allocator_t p;
    pool_init(&p, backing, sizeof(backing), BLOCK_SIZE);

    void *a = pool_alloc(&p);
    void *b = pool_alloc(&p);
    ASSERT_EQ(2, pool_allocated(&p));
    ASSERT_EQ(NUM_BLOCKS - 2, pool_available(&p));

    pool_free(&p, a);
    ASSERT_EQ(1, pool_allocated(&p));
    ASSERT_EQ(NUM_BLOCKS - 1, pool_available(&p));

    pool_free(&p, b);
    ASSERT_EQ(0, pool_allocated(&p));
    ASSERT_EQ(NUM_BLOCKS, pool_available(&p));
}

/* ── use with struct-sized blocks ── */

void test_struct_blocks(void) {
    typedef struct { int x; int y; float z; } vec3_t;

    uint8_t buf[sizeof(vec3_t) * 16] __attribute__((aligned(16)));
    pool_allocator_t p;
    pool_init(&p, buf, sizeof(buf), sizeof(vec3_t));

    vec3_t *v1 = (vec3_t *)pool_alloc(&p);
    vec3_t *v2 = (vec3_t *)pool_alloc(&p);
    ASSERT_NOT_NULL(v1);
    ASSERT_NOT_NULL(v2);

    v1->x = 1; v1->y = 2; v1->z = 3.0f;
    v2->x = 4; v2->y = 5; v2->z = 6.0f;

    ASSERT_EQ(1, v1->x);
    ASSERT_EQ(5, v2->y);

    pool_free(&p, v1);
    /* v2 should still be valid */
    ASSERT_EQ(4, v2->x);
    ASSERT_EQ(6.0f, (int)v2->z);
}

int main(void) {
    TEST_BEGIN();
    RUN_TEST(test_init_stats);
    RUN_TEST(test_block_size_minimum);
    RUN_TEST(test_alloc_one);
    RUN_TEST(test_alloc_within_bounds);
    RUN_TEST(test_alloc_all);
    RUN_TEST(test_exhaustion);
    RUN_TEST(test_free_and_realloc);
    RUN_TEST(test_lifo_order);
    RUN_TEST(test_reset);
    RUN_TEST(test_stress_stats_consistent);
    RUN_TEST(test_data_integrity);
    RUN_TEST(test_block_alignment);
    RUN_TEST(test_free_stats);
    RUN_TEST(test_struct_blocks);
    TEST_END();
}
