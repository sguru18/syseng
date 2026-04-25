#include "common.h"
#include "backtrace.h"

/*
 * Mark all test functions noinline so they actually appear as
 * separate frames on the call stack.
 */

#define MAX_FRAMES 32

__attribute__((noinline))
static int call_capture(stack_frame_t *frames, int max) {
    return capture_backtrace(frames, max);
}

__attribute__((noinline))
static int func_c(stack_frame_t *frames, int max) {
    return call_capture(frames, max);
}

__attribute__((noinline))
static int func_b(stack_frame_t *frames, int max) {
    return func_c(frames, max);
}

__attribute__((noinline))
static int func_a(stack_frame_t *frames, int max) {
    return func_b(frames, max);
}

/* ── Tests ───────────────────────────────────────────── */

void test_basic_backtrace(void) {
    stack_frame_t frames[MAX_FRAMES];
    int count = func_a(frames, MAX_FRAMES);
    /* main -> test runner -> test_basic_backtrace -> func_a -> func_b -> func_c -> call_capture -> capture_backtrace */
    ASSERT_TRUE(count >= 4);
}

void test_return_addresses_non_null(void) {
    stack_frame_t frames[MAX_FRAMES];
    int count = func_a(frames, MAX_FRAMES);
    ASSERT_TRUE(count >= 4);
    for (int i = 0; i < count; i++) {
        ASSERT_NOT_NULL(frames[i].return_address);
    }
}

void test_frame_pointers_non_null(void) {
    stack_frame_t frames[MAX_FRAMES];
    int count = func_a(frames, MAX_FRAMES);
    ASSERT_TRUE(count >= 4);
    for (int i = 0; i < count; i++) {
        ASSERT_NOT_NULL(frames[i].frame_pointer);
    }
}

void test_frame_pointers_distinct(void) {
    stack_frame_t frames[MAX_FRAMES];
    int count = func_a(frames, MAX_FRAMES);
    ASSERT_TRUE(count >= 4);
    for (int i = 1; i < count; i++) {
        ASSERT_TRUE(frames[i].frame_pointer != frames[i - 1].frame_pointer);
    }
}

void test_max_frames_limit(void) {
    stack_frame_t frames[3];
    int count = func_a(frames, 3);
    ASSERT_TRUE(count <= 3);
    ASSERT_TRUE(count == 3);
}

__attribute__((noinline))
static int recurse(stack_frame_t *frames, int max, int depth) {
    if (depth <= 0) {
        return capture_backtrace(frames, max);
    }
    return recurse(frames, max, depth - 1);
}

void test_recursive_depth(void) {
    stack_frame_t frames[MAX_FRAMES];
    int count5 = recurse(frames, MAX_FRAMES, 5);
    ASSERT_TRUE(count5 >= 6);

    int count10 = recurse(frames, MAX_FRAMES, 10);
    ASSERT_TRUE(count10 >= 11);
    ASSERT_TRUE(count10 > count5);
}

void test_single_frame(void) {
    stack_frame_t frames[1];
    int count = capture_backtrace(frames, 1);
    ASSERT_EQ(1, count);
    ASSERT_NOT_NULL(frames[0].frame_pointer);
}

void test_zero_max_frames(void) {
    stack_frame_t frames[1];
    frames[0].frame_pointer = (void *)0xDEAD;
    int count = capture_backtrace(frames, 0);
    ASSERT_EQ(0, count);
    ASSERT_TRUE(frames[0].frame_pointer == (void *)0xDEAD);
}

void test_backtrace_print(void) {
    stack_frame_t frames[MAX_FRAMES];
    int count = func_a(frames, MAX_FRAMES);
    ASSERT_TRUE(count >= 4);
    printf("  (visual check -- backtrace below should show a chain of addresses)\n");
    print_backtrace(frames, count);
}

void test_return_addresses_in_code_segment(void) {
    stack_frame_t frames[MAX_FRAMES];
    int count = func_a(frames, MAX_FRAMES);
    ASSERT_TRUE(count >= 4);
    /* All return addresses should be in roughly the same region (code segment) */
    uintptr_t first = (uintptr_t)frames[0].return_address;
    for (int i = 1; i < count && i < 6; i++) {
        uintptr_t addr = (uintptr_t)frames[i].return_address;
        uintptr_t diff = (addr > first) ? addr - first : first - addr;
        ASSERT_TRUE(diff < 0x100000);
    }
}

int main(void) {
    printf("\n=== Stack Frames: Backtrace Walker ===\n\n");

    TEST_BEGIN();

    RUN_TEST(test_basic_backtrace);
    RUN_TEST(test_return_addresses_non_null);
    RUN_TEST(test_frame_pointers_non_null);
    RUN_TEST(test_frame_pointers_distinct);
    RUN_TEST(test_max_frames_limit);
    RUN_TEST(test_recursive_depth);
    RUN_TEST(test_single_frame);
    RUN_TEST(test_zero_max_frames);
    RUN_TEST(test_backtrace_print);
    RUN_TEST(test_return_addresses_in_code_segment);

    TEST_END();
}
