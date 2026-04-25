#include "common.h"
#include "strings.h"
#include <stdlib.h>

/* --- my_strlen --- */

void test_strlen(void) {
    ASSERT_EQ(5, my_strlen("hello"));
    ASSERT_EQ(0, my_strlen(""));
    ASSERT_EQ(1, my_strlen("x"));
    ASSERT_EQ(11, my_strlen("hello world"));
}

/* --- my_strcpy --- */

void test_strcpy(void) {
    char dst[32];
    my_strcpy(dst, "hello");
    ASSERT_STR_EQ("hello", dst);

    my_strcpy(dst, "");
    ASSERT_STR_EQ("", dst);

    my_strcpy(dst, "a longer string here");
    ASSERT_STR_EQ("a longer string here", dst);
}

/* --- my_strcmp --- */

void test_strcmp(void) {
    ASSERT_EQ(0, my_strcmp("hello", "hello"));
    ASSERT_EQ(0, my_strcmp("", ""));
    ASSERT_TRUE(my_strcmp("apple", "banana") < 0);
    ASSERT_TRUE(my_strcmp("banana", "apple") > 0);
    ASSERT_TRUE(my_strcmp("abc", "abd") < 0);
    ASSERT_TRUE(my_strcmp("abc", "ab") > 0);
    ASSERT_TRUE(my_strcmp("ab", "abc") < 0);
}

/* --- my_strcat --- */

void test_strcat(void) {
    char dst[64] = "hello";
    my_strcat(dst, " world");
    ASSERT_STR_EQ("hello world", dst);

    char dst2[64] = "";
    my_strcat(dst2, "test");
    ASSERT_STR_EQ("test", dst2);

    char dst3[64] = "abc";
    my_strcat(dst3, "");
    ASSERT_STR_EQ("abc", dst3);
}

/* --- my_strdup --- */

void test_strdup(void) {
    char *dup = my_strdup("hello");
    ASSERT_NOT_NULL(dup);
    ASSERT_STR_EQ("hello", dup);
    free(dup);

    dup = my_strdup("");
    ASSERT_NOT_NULL(dup);
    ASSERT_STR_EQ("", dup);
    free(dup);

    const char *orig = "test string";
    dup = my_strdup(orig);
    ASSERT_NOT_NULL(dup);
    ASSERT_STR_EQ("test string", dup);
    ASSERT_TRUE(dup != orig);  // must be a different allocation
    free(dup);
}

/* --- str_join --- */

void test_str_join(void) {
    char *joined = str_join("hello", " world");
    ASSERT_NOT_NULL(joined);
    ASSERT_STR_EQ("hello world", joined);
    free(joined);

    joined = str_join("", "test");
    ASSERT_NOT_NULL(joined);
    ASSERT_STR_EQ("test", joined);
    free(joined);

    joined = str_join("test", "");
    ASSERT_NOT_NULL(joined);
    ASSERT_STR_EQ("test", joined);
    free(joined);

    joined = str_join("", "");
    ASSERT_NOT_NULL(joined);
    ASSERT_STR_EQ("", joined);
    free(joined);
}

/* --- str_reverse --- */

void test_str_reverse(void) {
    char s1[] = "hello";
    str_reverse(s1);
    ASSERT_STR_EQ("olleh", s1);

    char s2[] = "ab";
    str_reverse(s2);
    ASSERT_STR_EQ("ba", s2);

    char s3[] = "a";
    str_reverse(s3);
    ASSERT_STR_EQ("a", s3);

    char s4[] = "";
    str_reverse(s4);
    ASSERT_STR_EQ("", s4);

    char s5[] = "racecar";
    str_reverse(s5);
    ASSERT_STR_EQ("racecar", s5);
}

/* --- str_is_palindrome --- */

void test_str_is_palindrome(void) {
    ASSERT_EQ(1, str_is_palindrome("racecar"));
    ASSERT_EQ(1, str_is_palindrome("aba"));
    ASSERT_EQ(1, str_is_palindrome("a"));
    ASSERT_EQ(1, str_is_palindrome(""));
    ASSERT_EQ(1, str_is_palindrome("abba"));
    ASSERT_EQ(0, str_is_palindrome("hello"));
    ASSERT_EQ(0, str_is_palindrome("ab"));
    ASSERT_EQ(0, str_is_palindrome("abc"));
}

/* --- dynamic vector --- */

void test_vec_basic(void) {
    int_vec_t v;
    vec_init(&v, 4);
    ASSERT_EQ(0, (long long)v.length);
    ASSERT_EQ(4, (long long)v.capacity);
    ASSERT_NOT_NULL(v.data);

    vec_push(&v, 10);
    vec_push(&v, 20);
    vec_push(&v, 30);
    ASSERT_EQ(3, (long long)v.length);
    ASSERT_EQ(10, vec_get(&v, 0));
    ASSERT_EQ(20, vec_get(&v, 1));
    ASSERT_EQ(30, vec_get(&v, 2));

    int popped = vec_pop(&v);
    ASSERT_EQ(30, popped);
    ASSERT_EQ(2, (long long)v.length);

    vec_free(&v);
}

void test_vec_grow(void) {
    int_vec_t v;
    vec_init(&v, 2);

    for (int i = 0; i < 100; i++) {
        vec_push(&v, i * 10);
    }
    ASSERT_EQ(100, (long long)v.length);
    ASSERT_TRUE(v.capacity >= 100);

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(i * 10, vec_get(&v, (size_t)i));
    }

    vec_free(&v);
}

void test_vec_push_pop(void) {
    int_vec_t v;
    vec_init(&v, 1);

    vec_push(&v, 5);
    vec_push(&v, 10);
    vec_push(&v, 15);

    ASSERT_EQ(15, vec_pop(&v));
    ASSERT_EQ(10, vec_pop(&v));

    vec_push(&v, 99);
    ASSERT_EQ(99, vec_get(&v, 1));
    ASSERT_EQ(2, (long long)v.length);

    vec_free(&v);
}

int main(void) {
    printf("=== Strings and Arrays ===\n\n");
    TEST_BEGIN();

    RUN_TEST(test_strlen);
    RUN_TEST(test_strcpy);
    RUN_TEST(test_strcmp);
    RUN_TEST(test_strcat);
    RUN_TEST(test_strdup);
    RUN_TEST(test_str_join);
    RUN_TEST(test_str_reverse);
    RUN_TEST(test_str_is_palindrome);
    RUN_TEST(test_vec_basic);
    RUN_TEST(test_vec_grow);
    RUN_TEST(test_vec_push_pop);

    TEST_END();
}
