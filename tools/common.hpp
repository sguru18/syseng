#pragma once

/*
 * C++ test and benchmark utilities for syseng workbook.
 *
 * Drop-in companion to common.h for C++ exercises.
 * Include this instead of common.h in .cpp files.
 *
 * Usage:
 *   TEST_BEGIN();
 *   RUN_TEST(test_my_thing);
 *   TEST_END();
 *
 * For benchmarks:
 *   auto t = bench::now();
 *   // ... work ...
 *   auto ns = bench::elapsed_ns(t);
 *   bench::print_result("my operation", ns, iterations);
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <functional>

// ── Color codes ──────────────────────────────────────────────────────────────

#define GREEN  "\033[92m"
#define RED    "\033[91m"
#define YELLOW "\033[93m"
#define CYAN   "\033[96m"
#define BOLD   "\033[1m"
#define RESET  "\033[0m"

// ── Test framework ────────────────────────────────────────────────────────────

static int _tests_run    = 0;
static int _tests_passed = 0;
static int _tests_failed = 0;
static const char *_current_test = nullptr;

#define TEST_BEGIN() do { _tests_run = _tests_passed = _tests_failed = 0; } while(0)

#define RUN_TEST(fn) do { \
    _current_test = #fn; \
    _tests_run++; \
    int _prev = _tests_failed; \
    fn(); \
    if (_tests_failed == _prev) { \
        _tests_passed++; \
        std::printf(GREEN "  PASS" RESET " %s\n", #fn); \
    } \
} while(0)

#define TEST_END() do { \
    std::printf("\n%s%d/%d tests passed%s\n", \
        (_tests_failed == 0) ? GREEN BOLD : RED BOLD, \
        _tests_passed, _tests_run, RESET); \
    return (_tests_failed == 0) ? 0 : 1; \
} while(0)

#define ASSERT_EQ(expected, actual) do { \
    auto _exp = (expected); auto _act = (actual); \
    if (_exp != _act) { \
        std::printf(RED "  FAIL" RESET " %s: expected %lld, got %lld  (%s:%d)\n", \
            _current_test, (long long)_exp, (long long)_act, __FILE__, __LINE__); \
        _tests_failed++; return; \
    } \
} while(0)

#define ASSERT_EQ_F(expected, actual, tol) do { \
    double _exp = (expected), _act = (actual); \
    if (std::abs(_exp - _act) > (tol)) { \
        std::printf(RED "  FAIL" RESET " %s: expected %.6f, got %.6f  (%s:%d)\n", \
            _current_test, _exp, _act, __FILE__, __LINE__); \
        _tests_failed++; return; \
    } \
} while(0)

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        std::printf(RED "  FAIL" RESET " %s: assertion failed: %s  (%s:%d)\n", \
            _current_test, #cond, __FILE__, __LINE__); \
        _tests_failed++; return; \
    } \
} while(0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_NULL(ptr)     ASSERT_TRUE((ptr) == nullptr)
#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != nullptr)

#define ASSERT_STR_EQ(expected, actual) do { \
    std::string _exp(expected), _act(actual); \
    if (_exp != _act) { \
        std::printf(RED "  FAIL" RESET " %s: expected \"%s\", got \"%s\"  (%s:%d)\n", \
            _current_test, _exp.c_str(), _act.c_str(), __FILE__, __LINE__); \
        _tests_failed++; return; \
    } \
} while(0)

#define ASSERT_THROWS(expr, ExcType) do { \
    bool _caught = false; \
    try { (void)(expr); } catch (const ExcType &) { _caught = true; } \
    if (!_caught) { \
        std::printf(RED "  FAIL" RESET " %s: expected " #ExcType " not thrown  (%s:%d)\n", \
            _current_test, __FILE__, __LINE__); \
        _tests_failed++; return; \
    } \
} while(0)

// ── Timing and benchmarking ───────────────────────────────────────────────────

namespace bench {

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

inline TimePoint now() { return Clock::now(); }

inline long long elapsed_ns(TimePoint start) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now() - start).count();
}

inline long long elapsed_us(TimePoint start) {
    return std::chrono::duration_cast<std::chrono::microseconds>(now() - start).count();
}

inline long long elapsed_ms(TimePoint start) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(now() - start).count();
}

// Run fn() `iterations` times, return median nanoseconds per iteration.
inline double median_ns(std::function<void()> fn, int iterations = 100) {
    std::vector<long long> samples;
    samples.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        auto t = now();
        fn();
        samples.push_back(elapsed_ns(t));
    }
    std::sort(samples.begin(), samples.end());
    return static_cast<double>(samples[samples.size() / 2]);
}

// Print a benchmark result line.
inline void print_result(const char *label, double ns_per_iter, long long iters = 1) {
    if (ns_per_iter < 1000.0)
        std::printf(CYAN "  bench" RESET " %-40s  %6.1f ns  (%lld iters)\n", label, ns_per_iter, iters);
    else if (ns_per_iter < 1e6)
        std::printf(CYAN "  bench" RESET " %-40s  %6.1f µs  (%lld iters)\n", label, ns_per_iter / 1000.0, iters);
    else
        std::printf(CYAN "  bench" RESET " %-40s  %6.1f ms  (%lld iters)\n", label, ns_per_iter / 1e6, iters);
}

// Print throughput in millions of operations per second.
inline void print_throughput(const char *label, long long ops, double total_ns) {
    double mops = (ops / total_ns) * 1000.0;
    std::printf(CYAN "  bench" RESET " %-40s  %6.2f Mops/s\n", label, mops);
}

// Percentile from a sorted vector.
inline double percentile(const std::vector<long long> &sorted, double p) {
    size_t idx = static_cast<size_t>(p / 100.0 * sorted.size());
    if (idx >= sorted.size()) idx = sorted.size() - 1;
    return static_cast<double>(sorted[idx]);
}

// Print a latency distribution.
inline void print_latency_dist(const char *label, std::vector<long long> samples) {
    if (samples.empty()) return;
    std::sort(samples.begin(), samples.end());
    double p50 = percentile(samples, 50);
    double p99 = percentile(samples, 99);
    double p999 = percentile(samples, 99.9);
    double avg = static_cast<double>(
        std::accumulate(samples.begin(), samples.end(), 0LL)) / samples.size();
    std::printf(CYAN "  dist " RESET " %-30s  avg=%.0fns  p50=%.0fns  p99=%.0fns  p99.9=%.0fns\n",
        label, avg, p50, p99, p999);
}

} // namespace bench

// ── Prevent dead-code elimination for benchmarks ──────────────────────────────
// Use: bench::do_not_optimize(x) to keep a computed value alive.
namespace bench {
template <typename T>
inline void do_not_optimize(T const &val) {
    asm volatile("" : : "r,m"(val) : "memory");
}
}
