/*
 * IEEE 754 Float Bit Surgery
 *
 * Manipulate single-precision floats by operating directly on their
 * 32-bit unsigned integer representation.
 *
 * For special values (NaN, infinity), preserve them or return them unchanged
 * unless the operation specifically transforms them.
 *
 * INCLUDES: Figure out what you need.
 *   You'll need your header. No standard library functions are required --
 *   this is pure bit manipulation.
 */

/* TODO: add your #include lines here */

#define SIGN_MASK  0x80000000u
#define EXP_MASK   0x7F800000u
#define FRAC_MASK  0x007FFFFFu
#define EXP_SHIFT  23
#define BIAS       127

/*
 * Example: Negate a float (flip the sign bit).
 * NaN is returned unchanged.
 */
float_bits float_negate(float_bits f) {
    unsigned exp  = (f >> EXP_SHIFT) & 0xFF;
    unsigned frac = f & FRAC_MASK;
    if (exp == 0xFF && frac != 0) return f;
    return f ^ SIGN_MASK;
}

/* TODO: Return the absolute value of f (clear the sign bit). NaN unchanged. */
float_bits float_abs(float_bits f) {
    return 0;
}

/*
 * TODO: Return f * 2.0.
 * Think about what "multiply by 2" means for normalized, denormalized,
 * zero, infinity, and NaN.
 */
float_bits float_twice(float_bits f) {
    return 0;
}

/*
 * TODO: Return f / 2.0.
 * The reverse of float_twice, but watch the boundary where a normalized
 * number becomes denormalized (exponent goes from 1 to 0).
 */
float_bits float_half(float_bits f) {
    return 0;
}

/* TODO: Return 1 if f represents NaN, 0 otherwise. */
int float_is_nan(float_bits f) {
    return 0;
}

/*
 * TODO: Convert a signed 32-bit integer to its IEEE 754 float representation.
 *
 * This is the hardest function. Think about:
 *   - Special case: x == 0
 *   - Sign handling (watch out for INT_MIN: its abs doesn't fit in a signed int)
 *   - Finding the most significant 1-bit to determine the exponent
 *   - Extracting the 23-bit mantissa (minus the implicit leading 1)
 *   - Rounding: when you discard low bits, round to nearest even
 */
float_bits int_to_float(int x) {
    return 0;
}
