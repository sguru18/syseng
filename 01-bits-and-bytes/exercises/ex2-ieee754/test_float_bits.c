#include <limits.h>
#include <math.h>
#include "common.h"
#include "float_bits.h"

/*
 * Helper: get the IEEE 754 bit pattern of a float via type punning.
 * Using a union is well-defined in C (but not C++).
 */
static float_bits to_bits(float f) {
    union { float f; unsigned int u; } u;
    u.f = f;
    return u.u;
}

/* ── float_negate (example, should pass) ──────────────────────── */

void test_float_negate(void) {
    ASSERT_EQ_UINT(to_bits(-1.0f),  float_negate(to_bits(1.0f)));
    ASSERT_EQ_UINT(to_bits(1.0f),   float_negate(to_bits(-1.0f)));
    ASSERT_EQ_UINT(to_bits(-0.0f),  float_negate(to_bits(0.0f)));
    ASSERT_EQ_UINT(to_bits(0.0f),   float_negate(to_bits(-0.0f)));
    /* NaN stays NaN (unchanged) */
    float_bits nan_bits = to_bits(NAN);
    ASSERT_EQ_UINT(nan_bits, float_negate(nan_bits));
}

/* ── float_abs ────────────────────────────────────────────────── */

void test_float_abs(void) {
    ASSERT_EQ_UINT(to_bits(1.0f),       float_abs(to_bits(1.0f)));
    ASSERT_EQ_UINT(to_bits(1.0f),       float_abs(to_bits(-1.0f)));
    ASSERT_EQ_UINT(to_bits(0.0f),       float_abs(to_bits(0.0f)));
    ASSERT_EQ_UINT(to_bits(0.0f),       float_abs(to_bits(-0.0f)));
    ASSERT_EQ_UINT(to_bits(3.14f),      float_abs(to_bits(-3.14f)));
    ASSERT_EQ_UINT(to_bits(INFINITY),   float_abs(to_bits(-INFINITY)));
    /* NaN stays NaN (unchanged) */
    float_bits nan_bits = to_bits(NAN);
    ASSERT_EQ_UINT(nan_bits, float_abs(nan_bits));
}

/* ── float_twice ──────────────────────────────────────────────── */

void test_float_twice(void) {
    ASSERT_EQ_UINT(to_bits(2.0f),       float_twice(to_bits(1.0f)));
    ASSERT_EQ_UINT(to_bits(1.0f),       float_twice(to_bits(0.5f)));
    ASSERT_EQ_UINT(to_bits(-2.0f),      float_twice(to_bits(-1.0f)));
    ASSERT_EQ_UINT(to_bits(0.0f),       float_twice(to_bits(0.0f)));
    ASSERT_EQ_UINT(to_bits(-0.0f),      float_twice(to_bits(-0.0f)));
    /* Infinity stays infinity */
    ASSERT_EQ_UINT(to_bits(INFINITY),   float_twice(to_bits(INFINITY)));
    ASSERT_EQ_UINT(to_bits(-INFINITY),  float_twice(to_bits(-INFINITY)));
    /* NaN stays NaN */
    float_bits nan_bits = to_bits(NAN);
    ASSERT_EQ_UINT(nan_bits, float_twice(nan_bits));
    /* Denormalized: smallest positive denorm × 2 */
    float_bits smallest_denorm = 0x00000001u;
    ASSERT_EQ_UINT(0x00000002u, float_twice(smallest_denorm));
    /* Denorm that becomes normalized when doubled */
    float_bits half_min_norm = 0x00400000u; /* largest denorm mantissa bit */
    ASSERT_EQ_UINT(0x00800000u, float_twice(half_min_norm));
}

/* ── float_half ───────────────────────────────────────────────── */

void test_float_half(void) {
    ASSERT_EQ_UINT(to_bits(1.0f),       float_half(to_bits(2.0f)));
    ASSERT_EQ_UINT(to_bits(0.5f),       float_half(to_bits(1.0f)));
    ASSERT_EQ_UINT(to_bits(-0.5f),      float_half(to_bits(-1.0f)));
    ASSERT_EQ_UINT(to_bits(0.0f),       float_half(to_bits(0.0f)));
    ASSERT_EQ_UINT(to_bits(-0.0f),      float_half(to_bits(-0.0f)));
    /* Infinity stays infinity */
    ASSERT_EQ_UINT(to_bits(INFINITY),   float_half(to_bits(INFINITY)));
    ASSERT_EQ_UINT(to_bits(-INFINITY),  float_half(to_bits(-INFINITY)));
    /* NaN stays NaN */
    float_bits nan_bits = to_bits(NAN);
    ASSERT_EQ_UINT(nan_bits, float_half(nan_bits));
    /* Denorm halved */
    ASSERT_EQ_UINT(0x00000001u, float_half(0x00000002u));
    /* Smallest denorm halved → zero (rounds to zero) */
    ASSERT_EQ_UINT(0x00000000u, float_half(0x00000001u));
}

/* ── float_is_nan ─────────────────────────────────────────────── */

void test_float_is_nan(void) {
    ASSERT_EQ(1, float_is_nan(to_bits(NAN)));
    ASSERT_EQ(1, float_is_nan(0x7F800001u)); /* a specific NaN pattern */
    ASSERT_EQ(1, float_is_nan(0x7FC00000u)); /* quiet NaN */
    ASSERT_EQ(1, float_is_nan(0xFFC00001u)); /* negative NaN */
    ASSERT_EQ(0, float_is_nan(to_bits(0.0f)));
    ASSERT_EQ(0, float_is_nan(to_bits(1.0f)));
    ASSERT_EQ(0, float_is_nan(to_bits(-1.0f)));
    ASSERT_EQ(0, float_is_nan(to_bits(INFINITY)));
    ASSERT_EQ(0, float_is_nan(to_bits(-INFINITY)));
}

/* ── int_to_float ─────────────────────────────────────────────── */

void test_int_to_float(void) {
    ASSERT_EQ_UINT(to_bits(0.0f),           int_to_float(0));
    ASSERT_EQ_UINT(to_bits(1.0f),           int_to_float(1));
    ASSERT_EQ_UINT(to_bits(-1.0f),          int_to_float(-1));
    ASSERT_EQ_UINT(to_bits(2.0f),           int_to_float(2));
    ASSERT_EQ_UINT(to_bits(-2.0f),          int_to_float(-2));
    ASSERT_EQ_UINT(to_bits(42.0f),          int_to_float(42));
    ASSERT_EQ_UINT(to_bits(256.0f),         int_to_float(256));
    ASSERT_EQ_UINT(to_bits(1024.0f),        int_to_float(1024));
    ASSERT_EQ_UINT(to_bits((float)INT_MAX), int_to_float(INT_MAX));
    ASSERT_EQ_UINT(to_bits((float)INT_MIN), int_to_float(INT_MIN));
    ASSERT_EQ_UINT(to_bits(65536.0f),       int_to_float(65536));
    /* Powers of 2 should be exact */
    ASSERT_EQ_UINT(to_bits(1048576.0f),     int_to_float(1 << 20));
    /* Test rounding: 16777217 has 25 bits, can't be represented exactly in float (23-bit mantissa) */
    ASSERT_EQ_UINT(to_bits((float)16777217), int_to_float(16777217));
}

/* ── main ─────────────────────────────────────────────────────── */

int main(void) {
    TEST_BEGIN();
    RUN_TEST(test_float_negate);
    RUN_TEST(test_float_abs);
    RUN_TEST(test_float_twice);
    RUN_TEST(test_float_half);
    RUN_TEST(test_float_is_nan);
    RUN_TEST(test_int_to_float);
    TEST_END();
}
