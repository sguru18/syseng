#ifndef SYSENG_COMMON_H
#define SYSENG_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Lightweight test framework for syseng workbook.
 *
 * Usage:
 *   TEST_BEGIN();
 *   RUN_TEST(test_my_function);
 *   RUN_TEST(test_edge_cases);
 *   TEST_END();
 */

static int _tests_run = 0;
static int _tests_passed = 0;
static int _tests_failed = 0;
static const char *_current_test = NULL;

#define GREEN   "\033[92m"
#define RED     "\033[91m"
#define YELLOW  "\033[93m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

#define TEST_BEGIN() do { \
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0; \
} while(0)

#define RUN_TEST(fn) do { \
    _current_test = #fn; \
    _tests_run++; \
    int _prev_failed = _tests_failed; \
    fn(); \
    if (_tests_failed == _prev_failed) { \
        _tests_passed++; \
        printf(GREEN "  PASS" RESET " %s\n", #fn); \
    } \
} while(0)

#define TEST_END() do { \
    printf("\n%s%d/%d tests passed%s\n", \
        (_tests_failed == 0) ? GREEN BOLD : RED BOLD, \
        _tests_passed, _tests_run, RESET); \
    return (_tests_failed == 0) ? 0 : 1; \
} while(0)

#define ASSERT_EQ(expected, actual) do { \
    long long _exp = (long long)(expected); \
    long long _act = (long long)(actual); \
    if (_exp != _act) { \
        printf(RED "  FAIL" RESET " %s: " \
               "expected %lld, got %lld  (%s:%d)\n", \
               _current_test, _exp, _act, __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_EQ_UINT(expected, actual) do { \
    unsigned long long _exp = (unsigned long long)(expected); \
    unsigned long long _act = (unsigned long long)(actual); \
    if (_exp != _act) { \
        printf(RED "  FAIL" RESET " %s: " \
               "expected 0x%llx, got 0x%llx  (%s:%d)\n", \
               _current_test, _exp, _act, __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf(RED "  FAIL" RESET " %s: " \
               "assertion failed: %s  (%s:%d)\n", \
               _current_test, #cond, __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_NULL(ptr) do { \
    if ((ptr) != NULL) { \
        printf(RED "  FAIL" RESET " %s: " \
               "expected NULL, got %p  (%s:%d)\n", \
               _current_test, (void*)(ptr), __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_NOT_NULL(ptr) do { \
    if ((ptr) == NULL) { \
        printf(RED "  FAIL" RESET " %s: " \
               "expected non-NULL, got NULL  (%s:%d)\n", \
               _current_test, __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_STR_EQ(expected, actual) do { \
    const char *_exp = (expected); \
    const char *_act = (actual); \
    if (_exp == NULL || _act == NULL || strcmp(_exp, _act) != 0) { \
        printf(RED "  FAIL" RESET " %s: " \
               "expected \"%s\", got \"%s\"  (%s:%d)\n", \
               _current_test, _exp ? _exp : "(null)", \
               _act ? _act : "(null)", __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_MEM_EQ(expected, actual, len) do { \
    if (memcmp((expected), (actual), (len)) != 0) { \
        printf(RED "  FAIL" RESET " %s: " \
               "memory mismatch (%zu bytes)  (%s:%d)\n", \
               _current_test, (size_t)(len), __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#endif /* SYSENG_COMMON_H */
