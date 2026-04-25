#include "common.h"
#include "puzzles.h"
#include <stddef.h>

#define CHECK_PREDICTION(desc, predicted, actual) do { \
    int _p = (predicted); \
    int _a = (int)(actual); \
    if (_p == -1) { \
        printf(YELLOW "  SKIP" RESET " %s: still -1 (fill in your prediction!)\n", desc); \
        _tests_failed++; \
        return; \
    } \
    if (_p != _a) { \
        printf(RED "  FAIL" RESET " %s: you predicted %d, actual is %d\n", desc, _p, _a); \
        _tests_failed++; \
        return; \
    } \
} while(0)

/* ── struct simple ─────────────────────────────────────── */

void test_sizeof_simple(void) {
    CHECK_PREDICTION(
        "sizeof(struct simple)",
        predict_sizeof_simple(),
        sizeof(struct simple)
    );
}

void test_offsetof_simple_b(void) {
    CHECK_PREDICTION(
        "offsetof(struct simple, b)",
        predict_offsetof_simple_b(),
        offsetof(struct simple, b)
    );
}

void test_offsetof_simple_c(void) {
    CHECK_PREDICTION(
        "offsetof(struct simple, c) -- remember trailing padding for struct alignment",
        predict_offsetof_simple_c(),
        offsetof(struct simple, c)
    );
}

/* ── struct packed_attempt ─────────────────────────────── */

void test_sizeof_packed_attempt(void) {
    CHECK_PREDICTION(
        "sizeof(struct packed_attempt) -- chars pack together, less padding!",
        predict_sizeof_packed_attempt(),
        sizeof(struct packed_attempt)
    );
}

void test_offsetof_packed_attempt_c(void) {
    CHECK_PREDICTION(
        "offsetof(struct packed_attempt, c)",
        predict_offsetof_packed_attempt_c(),
        offsetof(struct packed_attempt, c)
    );
}

/* ── struct nested ─────────────────────────────────────── */

void test_sizeof_nested(void) {
    CHECK_PREDICTION(
        "sizeof(struct nested) -- a struct's alignment = its largest member's alignment",
        predict_sizeof_nested(),
        sizeof(struct nested)
    );
}

void test_offsetof_nested_inner(void) {
    CHECK_PREDICTION(
        "offsetof(struct nested, inner) -- inner needs alignment of struct simple",
        predict_offsetof_nested_inner(),
        offsetof(struct nested, inner)
    );
}

void test_offsetof_nested_b(void) {
    CHECK_PREDICTION(
        "offsetof(struct nested, b) -- right after inner ends",
        predict_offsetof_nested_b(),
        offsetof(struct nested, b)
    );
}

/* ── struct pointer_mix ────────────────────────────────── */

void test_sizeof_pointer_mix(void) {
    CHECK_PREDICTION(
        "sizeof(struct pointer_mix) -- pointers are 8 bytes on 64-bit!",
        predict_sizeof_pointer_mix(),
        sizeof(struct pointer_mix)
    );
}

void test_offsetof_pointer_mix_p(void) {
    CHECK_PREDICTION(
        "offsetof(struct pointer_mix, p) -- pointer needs 8-byte alignment",
        predict_offsetof_pointer_mix_p(),
        offsetof(struct pointer_mix, p)
    );
}

void test_offsetof_pointer_mix_b(void) {
    CHECK_PREDICTION(
        "offsetof(struct pointer_mix, b)",
        predict_offsetof_pointer_mix_b(),
        offsetof(struct pointer_mix, b)
    );
}

/* ── struct bitfield_struct ────────────────────────────── */

void test_sizeof_bitfield_struct(void) {
    CHECK_PREDICTION(
        "sizeof(struct bitfield_struct) -- 3+5+7+17=32 bits. How many bytes?",
        predict_sizeof_bitfield_struct(),
        sizeof(struct bitfield_struct)
    );
}

/* ── struct tricky ─────────────────────────────────────── */

void test_sizeof_tricky(void) {
    CHECK_PREDICTION(
        "sizeof(struct tricky) -- double forces 8-byte alignment",
        predict_sizeof_tricky(),
        sizeof(struct tricky)
    );
}

void test_offsetof_tricky_d(void) {
    CHECK_PREDICTION(
        "offsetof(struct tricky, d) -- double needs 8-byte alignment",
        predict_offsetof_tricky_d(),
        offsetof(struct tricky, d)
    );
}

void test_offsetof_tricky_b(void) {
    CHECK_PREDICTION(
        "offsetof(struct tricky, b)",
        predict_offsetof_tricky_b(),
        offsetof(struct tricky, b)
    );
}

void test_offsetof_tricky_c(void) {
    CHECK_PREDICTION(
        "offsetof(struct tricky, c) -- int after char, needs 4-byte alignment",
        predict_offsetof_tricky_c(),
        offsetof(struct tricky, c)
    );
}

int main(void) {
    printf("\n=== Pointer Puzzles: Struct Layout Predictions ===\n\n");

    printf("Platform info: sizeof(char)=%zu, sizeof(int)=%zu, "
           "sizeof(double)=%zu, sizeof(char*)=%zu\n\n",
           sizeof(char), sizeof(int), sizeof(double), sizeof(char*));

    TEST_BEGIN();

    RUN_TEST(test_sizeof_simple);
    RUN_TEST(test_offsetof_simple_b);
    RUN_TEST(test_offsetof_simple_c);
    RUN_TEST(test_sizeof_packed_attempt);
    RUN_TEST(test_offsetof_packed_attempt_c);
    RUN_TEST(test_sizeof_nested);
    RUN_TEST(test_offsetof_nested_inner);
    RUN_TEST(test_offsetof_nested_b);
    RUN_TEST(test_sizeof_pointer_mix);
    RUN_TEST(test_offsetof_pointer_mix_p);
    RUN_TEST(test_offsetof_pointer_mix_b);
    RUN_TEST(test_sizeof_bitfield_struct);
    RUN_TEST(test_sizeof_tricky);
    RUN_TEST(test_offsetof_tricky_d);
    RUN_TEST(test_offsetof_tricky_b);
    RUN_TEST(test_offsetof_tricky_c);

    TEST_END();
}
