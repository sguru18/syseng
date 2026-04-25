#include "common.h"
#include "hello.h"

/* --- max_of / min_of / clamp --- */

void test_max_of(void) {
    ASSERT_EQ(5, max_of(3, 5));
    ASSERT_EQ(5, max_of(5, 3));
    ASSERT_EQ(7, max_of(7, 7));
    ASSERT_EQ(0, max_of(-1, 0));
    ASSERT_EQ(-3, max_of(-3, -10));
}

void test_min_of(void) {
    ASSERT_EQ(3, min_of(3, 5));
    ASSERT_EQ(3, min_of(5, 3));
    ASSERT_EQ(7, min_of(7, 7));
    ASSERT_EQ(-1, min_of(-1, 0));
    ASSERT_EQ(-10, min_of(-3, -10));
}

void test_clamp(void) {
    ASSERT_EQ(5, clamp(5, 0, 10));
    ASSERT_EQ(0, clamp(-5, 0, 10));
    ASSERT_EQ(10, clamp(15, 0, 10));
    ASSERT_EQ(0, clamp(0, 0, 10));
    ASSERT_EQ(10, clamp(10, 0, 10));
    ASSERT_EQ(-3, clamp(-3, -5, -1));
}

/* --- array functions --- */

void test_array_sum(void) {
    int a[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(15, array_sum(a, 5));

    int b[] = {-1, 1, -1, 1};
    ASSERT_EQ(0, array_sum(b, 4));

    ASSERT_EQ(0, array_sum(a, 0));

    int c[] = {42};
    ASSERT_EQ(42, array_sum(c, 1));
}

void test_array_max_index(void) {
    int a[] = {1, 5, 3, 9, 2};
    ASSERT_EQ(3, array_max_index(a, 5));

    int b[] = {10, 1, 2, 3};
    ASSERT_EQ(0, array_max_index(b, 4));

    int c[] = {1, 2, 3, 10};
    ASSERT_EQ(3, array_max_index(c, 4));

    ASSERT_EQ(-1, array_max_index(a, 0));
    ASSERT_EQ(-1, array_max_index(a, -1));

    int d[] = {7};
    ASSERT_EQ(0, array_max_index(d, 1));

    int e[] = {-5, -1, -10};
    ASSERT_EQ(1, array_max_index(e, 3));
}

void test_array_reverse(void) {
    int a[] = {1, 2, 3, 4, 5};
    array_reverse(a, 5);
    ASSERT_EQ(5, a[0]);
    ASSERT_EQ(4, a[1]);
    ASSERT_EQ(3, a[2]);
    ASSERT_EQ(2, a[3]);
    ASSERT_EQ(1, a[4]);

    int b[] = {1, 2, 3, 4};
    array_reverse(b, 4);
    ASSERT_EQ(4, b[0]);
    ASSERT_EQ(3, b[1]);
    ASSERT_EQ(2, b[2]);
    ASSERT_EQ(1, b[3]);

    int c[] = {42};
    array_reverse(c, 1);
    ASSERT_EQ(42, c[0]);

    int d[] = {1, 2};
    array_reverse(d, 0);
    ASSERT_EQ(1, d[0]);
}

void test_array_contains(void) {
    int a[] = {10, 20, 30, 40, 50};
    ASSERT_EQ(1, array_contains(a, 5, 30));
    ASSERT_EQ(1, array_contains(a, 5, 10));
    ASSERT_EQ(1, array_contains(a, 5, 50));
    ASSERT_EQ(0, array_contains(a, 5, 25));
    ASSERT_EQ(0, array_contains(a, 5, 0));
    ASSERT_EQ(0, array_contains(a, 0, 10));
}

/* --- fizzbuzz --- */

void test_fizzbuzz(void) {
    char buf[32];

    fizzbuzz(1, buf);
    ASSERT_STR_EQ("1", buf);

    fizzbuzz(3, buf);
    ASSERT_STR_EQ("Fizz", buf);

    fizzbuzz(5, buf);
    ASSERT_STR_EQ("Buzz", buf);

    fizzbuzz(15, buf);
    ASSERT_STR_EQ("FizzBuzz", buf);

    fizzbuzz(7, buf);
    ASSERT_STR_EQ("7", buf);

    fizzbuzz(30, buf);
    ASSERT_STR_EQ("FizzBuzz", buf);

    fizzbuzz(9, buf);
    ASSERT_STR_EQ("Fizz", buf);

    fizzbuzz(10, buf);
    ASSERT_STR_EQ("Buzz", buf);

    fizzbuzz(100, buf);
    ASSERT_STR_EQ("Buzz", buf);
}

int main(void) {
    printf("=== Hello C ===\n\n");
    TEST_BEGIN();

    RUN_TEST(test_max_of);
    RUN_TEST(test_min_of);
    RUN_TEST(test_clamp);
    RUN_TEST(test_array_sum);
    RUN_TEST(test_array_max_index);
    RUN_TEST(test_array_reverse);
    RUN_TEST(test_array_contains);
    RUN_TEST(test_fizzbuzz);

    TEST_END();
}
