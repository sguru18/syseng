#include "common.h"
#include "sort.h"
#include <string.h>

/* ── Helpers ──────────────────────────────────────────── */

static int is_sorted_int(const int *arr, int n, int ascending) {
    for (int i = 1; i < n; i++) {
        if (ascending && arr[i - 1] > arr[i]) return 0;
        if (!ascending && arr[i - 1] < arr[i]) return 0;
    }
    return 1;
}

/* ── Integer sort tests ──────────────────────────────── */

void test_sort_int_ascending(void) {
    int arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    generic_sort(arr, 10, sizeof(int), cmp_int_asc);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(i, arr[i]);
    }
}

void test_sort_int_descending(void) {
    int arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    generic_sort(arr, 10, sizeof(int), cmp_int_desc);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(9 - i, arr[i]);
    }
}

void test_sort_int_already_sorted(void) {
    int arr[] = {1, 2, 3, 4, 5};
    generic_sort(arr, 5, sizeof(int), cmp_int_asc);
    ASSERT_TRUE(is_sorted_int(arr, 5, 1));
    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(5, arr[4]);
}

void test_sort_int_reverse_sorted(void) {
    int arr[] = {5, 4, 3, 2, 1};
    generic_sort(arr, 5, sizeof(int), cmp_int_asc);
    ASSERT_TRUE(is_sorted_int(arr, 5, 1));
    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(5, arr[4]);
}

void test_sort_int_duplicates(void) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    generic_sort(arr, 10, sizeof(int), cmp_int_asc);
    ASSERT_TRUE(is_sorted_int(arr, 10, 1));
    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(1, arr[1]);
    ASSERT_EQ(9, arr[9]);
}

void test_sort_int_all_same(void) {
    int arr[] = {7, 7, 7, 7, 7};
    generic_sort(arr, 5, sizeof(int), cmp_int_asc);
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(7, arr[i]);
    }
}

void test_sort_int_single(void) {
    int arr[] = {42};
    generic_sort(arr, 1, sizeof(int), cmp_int_asc);
    ASSERT_EQ(42, arr[0]);
}

void test_sort_int_empty(void) {
    int arr[] = {1};
    generic_sort(arr, 0, sizeof(int), cmp_int_asc);
    ASSERT_EQ(1, arr[0]);
}

void test_sort_int_two_elements(void) {
    int arr[] = {2, 1};
    generic_sort(arr, 2, sizeof(int), cmp_int_asc);
    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(2, arr[1]);
}

void test_sort_int_negative(void) {
    int arr[] = {-3, 5, -1, 0, -7, 2};
    generic_sort(arr, 6, sizeof(int), cmp_int_asc);
    ASSERT_EQ(-7, arr[0]);
    ASSERT_EQ(-3, arr[1]);
    ASSERT_EQ(-1, arr[2]);
    ASSERT_EQ(0, arr[3]);
    ASSERT_EQ(2, arr[4]);
    ASSERT_EQ(5, arr[5]);
}

/* ── String sort tests ───────────────────────────────── */

void test_sort_string_alphabetical(void) {
    const char *arr[] = {"banana", "apple", "cherry", "date", "elderberry"};
    generic_sort(arr, 5, sizeof(const char *), cmp_string_asc);
    ASSERT_STR_EQ("apple", arr[0]);
    ASSERT_STR_EQ("banana", arr[1]);
    ASSERT_STR_EQ("cherry", arr[2]);
    ASSERT_STR_EQ("date", arr[3]);
    ASSERT_STR_EQ("elderberry", arr[4]);
}

void test_sort_string_by_length(void) {
    const char *arr[] = {"cherry", "fig", "apple", "kiwi", "do"};
    generic_sort(arr, 5, sizeof(const char *), cmp_string_by_length);
    ASSERT_EQ(2, (int)strlen(arr[0]));
    ASSERT_EQ(3, (int)strlen(arr[1]));
    ASSERT_EQ(4, (int)strlen(arr[2]));
    ASSERT_EQ(5, (int)strlen(arr[3]));
    ASSERT_EQ(6, (int)strlen(arr[4]));
}

void test_sort_string_single(void) {
    const char *arr[] = {"only"};
    generic_sort(arr, 1, sizeof(const char *), cmp_string_asc);
    ASSERT_STR_EQ("only", arr[0]);
}

void test_sort_string_same_length_alpha(void) {
    const char *arr[] = {"cat", "bat", "ant", "dog"};
    generic_sort(arr, 4, sizeof(const char *), cmp_string_by_length);
    ASSERT_STR_EQ("ant", arr[0]);
    ASSERT_STR_EQ("bat", arr[1]);
    ASSERT_STR_EQ("cat", arr[2]);
    ASSERT_STR_EQ("dog", arr[3]);
}

/* ── Comparator correctness tests ────────────────────── */

void test_cmp_int_asc_correctness(void) {
    int a = 1, b = 2, c = 1;
    ASSERT_TRUE(cmp_int_asc(&a, &b) < 0);
    ASSERT_TRUE(cmp_int_asc(&b, &a) > 0);
    ASSERT_EQ(0, cmp_int_asc(&a, &c));
}

void test_cmp_int_desc_correctness(void) {
    int a = 1, b = 2, c = 1;
    ASSERT_TRUE(cmp_int_desc(&a, &b) > 0);
    ASSERT_TRUE(cmp_int_desc(&b, &a) < 0);
    ASSERT_EQ(0, cmp_int_desc(&a, &c));
}

void test_cmp_string_asc_correctness(void) {
    const char *a = "apple", *b = "banana", *c = "apple";
    ASSERT_TRUE(cmp_string_asc(&a, &b) < 0);
    ASSERT_TRUE(cmp_string_asc(&b, &a) > 0);
    ASSERT_EQ(0, cmp_string_asc(&a, &c));
}

void test_cmp_string_by_length_correctness(void) {
    const char *short_s = "hi", *long_s = "hello", *same_s = "lo";
    ASSERT_TRUE(cmp_string_by_length(&short_s, &long_s) < 0);
    ASSERT_TRUE(cmp_string_by_length(&long_s, &short_s) > 0);
    /* Same length: should fall back to alphabetical */
    ASSERT_TRUE(cmp_string_by_length(&short_s, &same_s) < 0);
}

/* ── Generic swap test ───────────────────────────────── */

void test_generic_swap(void) {
    int a = 10, b = 20;
    generic_swap(&a, &b, sizeof(int));
    ASSERT_EQ(20, a);
    ASSERT_EQ(10, b);

    double x = 3.14, y = 2.72;
    generic_swap(&x, &y, sizeof(double));
    ASSERT_TRUE(y > 3.0 && y < 3.2);
    ASSERT_TRUE(x > 2.7 && x < 2.8);
}

int main(void) {
    printf("\n=== Function Pointers: Generic Sort ===\n\n");

    TEST_BEGIN();

    RUN_TEST(test_generic_swap);
    RUN_TEST(test_cmp_int_asc_correctness);
    RUN_TEST(test_cmp_int_desc_correctness);
    RUN_TEST(test_cmp_string_asc_correctness);
    RUN_TEST(test_cmp_string_by_length_correctness);
    RUN_TEST(test_sort_int_ascending);
    RUN_TEST(test_sort_int_descending);
    RUN_TEST(test_sort_int_already_sorted);
    RUN_TEST(test_sort_int_reverse_sorted);
    RUN_TEST(test_sort_int_duplicates);
    RUN_TEST(test_sort_int_all_same);
    RUN_TEST(test_sort_int_single);
    RUN_TEST(test_sort_int_empty);
    RUN_TEST(test_sort_int_two_elements);
    RUN_TEST(test_sort_int_negative);
    RUN_TEST(test_sort_string_alphabetical);
    RUN_TEST(test_sort_string_by_length);
    RUN_TEST(test_sort_string_single);
    RUN_TEST(test_sort_string_same_length_alpha);

    TEST_END();
}
