#include <limits.h>
#include "common.h"
#include "bits.h"

/* ── bit_and (example, should pass) ────────────────────────────── */

void test_bit_and(void) {
    ASSERT_EQ(0x00, bit_and(0x0F, 0xF0));
    ASSERT_EQ(0x0F, bit_and(0x0F, 0xFF));
    ASSERT_EQ(0,    bit_and(0, INT_MAX));
    ASSERT_EQ(-1,   bit_and(-1, -1));
}

/* ── bit_negate ────────────────────────────────────────────────── */

void test_bit_negate(void) {
    ASSERT_EQ(0,  bit_negate(0));
    ASSERT_EQ(-1, bit_negate(1));
    ASSERT_EQ(1,  bit_negate(-1));
    ASSERT_EQ(-42, bit_negate(42));
    ASSERT_EQ(42,  bit_negate(-42));
    ASSERT_EQ(-INT_MAX, bit_negate(INT_MAX));
    /* Note: -INT_MIN overflows in two's complement, but ~INT_MIN + 1 == INT_MIN */
    ASSERT_EQ(INT_MIN, bit_negate(INT_MIN));
}

/* ── bit_abs ──────────────────────────────────────────────────── */

void test_bit_abs(void) {
    ASSERT_EQ(0,  bit_abs(0));
    ASSERT_EQ(1,  bit_abs(1));
    ASSERT_EQ(1,  bit_abs(-1));
    ASSERT_EQ(42, bit_abs(42));
    ASSERT_EQ(42, bit_abs(-42));
    ASSERT_EQ(INT_MAX, bit_abs(INT_MAX));
    /* abs(INT_MIN) overflows — two's complement can't represent it.
       Most implementations return INT_MIN; accept that. */
    ASSERT_EQ(INT_MIN, bit_abs(INT_MIN));
}

/* ── is_negative ──────────────────────────────────────────────── */

void test_is_negative(void) {
    ASSERT_EQ(0, is_negative(0));
    ASSERT_EQ(0, is_negative(1));
    ASSERT_EQ(0, is_negative(INT_MAX));
    ASSERT_EQ(1, is_negative(-1));
    ASSERT_EQ(1, is_negative(-42));
    ASSERT_EQ(1, is_negative(INT_MIN));
}

/* ── is_power_of_2 ────────────────────────────────────────────── */

void test_is_power_of_2(void) {
    ASSERT_EQ(1, is_power_of_2(1));
    ASSERT_EQ(1, is_power_of_2(2));
    ASSERT_EQ(1, is_power_of_2(4));
    ASSERT_EQ(1, is_power_of_2(1 << 30));
    ASSERT_EQ(0, is_power_of_2(0));
    ASSERT_EQ(0, is_power_of_2(3));
    ASSERT_EQ(0, is_power_of_2(6));
    ASSERT_EQ(0, is_power_of_2(-1));
    ASSERT_EQ(0, is_power_of_2(INT_MIN));  /* 0x80000000 has one bit set but is negative */
    ASSERT_EQ(0, is_power_of_2(-16));
}

/* ── add_ok ───────────────────────────────────────────────────── */

void test_add_ok(void) {
    ASSERT_EQ(1, add_ok(0, 0));
    ASSERT_EQ(1, add_ok(1, 2));
    ASSERT_EQ(1, add_ok(-1, 1));
    ASSERT_EQ(1, add_ok(-100, 100));
    /* Positive overflow */
    ASSERT_EQ(0, add_ok(INT_MAX, 1));
    ASSERT_EQ(0, add_ok(INT_MAX, INT_MAX));
    /* Negative overflow */
    ASSERT_EQ(0, add_ok(INT_MIN, -1));
    ASSERT_EQ(0, add_ok(INT_MIN, INT_MIN));
    /* Just barely no overflow */
    ASSERT_EQ(1, add_ok(INT_MAX, 0));
    ASSERT_EQ(1, add_ok(INT_MIN, 0));
    ASSERT_EQ(1, add_ok(INT_MAX, INT_MIN));
}

/* ── conditional ──────────────────────────────────────────────── */

void test_conditional(void) {
    ASSERT_EQ(10, conditional(1, 10, 20));
    ASSERT_EQ(20, conditional(0, 10, 20));
    ASSERT_EQ(10, conditional(-1, 10, 20));
    ASSERT_EQ(10, conditional(42, 10, 20));
    ASSERT_EQ(0,  conditional(1, 0, 99));
    ASSERT_EQ(99, conditional(0, 0, 99));
    ASSERT_EQ(-5, conditional(INT_MIN, -5, 5));
}

/* ── bit_count ────────────────────────────────────────────────── */

void test_bit_count(void) {
    ASSERT_EQ(0,  bit_count(0));
    ASSERT_EQ(1,  bit_count(1));
    ASSERT_EQ(1,  bit_count(2));
    ASSERT_EQ(1,  bit_count(1 << 30));
    ASSERT_EQ(32, bit_count(-1));         /* all bits set */
    ASSERT_EQ(31, bit_count(INT_MAX));    /* all but sign bit */
    ASSERT_EQ(1,  bit_count(INT_MIN));    /* only sign bit */
    ASSERT_EQ(16, bit_count(0x55555555)); /* alternating: 0101... */
    ASSERT_EQ(16, bit_count((int)0xAAAAAAAA)); /* alternating: 1010... */
    ASSERT_EQ(8,  bit_count(0xFF));
}

/* ── main ─────────────────────────────────────────────────────── */

int main(void) {
    TEST_BEGIN();
    RUN_TEST(test_bit_and);
    RUN_TEST(test_bit_negate);
    RUN_TEST(test_bit_abs);
    RUN_TEST(test_is_negative);
    RUN_TEST(test_is_power_of_2);
    RUN_TEST(test_add_ok);
    RUN_TEST(test_conditional);
    RUN_TEST(test_bit_count);
    TEST_END();
}
