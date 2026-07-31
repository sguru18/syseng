#include <algorithm>
#include <deque>
#include <iterator>
#include <numeric>
#include <vector>
#include "common.hpp"
#include "circular_buffer.hpp"

// ─── Correctness tests ───────────────────────────────────────────────────────

void test_size_and_full() {
    CircularBuffer<int, 4> buf;
    ASSERT_EQ(0, (int)buf.size());
    ASSERT_FALSE(buf.full());
    ASSERT_EQ(4, (int)buf.capacity());

    buf.push_back(1);
    ASSERT_EQ(1, (int)buf.size());
    ASSERT_FALSE(buf.full());

    buf.push_back(2); buf.push_back(3); buf.push_back(4);
    ASSERT_EQ(4, (int)buf.size());
    ASSERT_TRUE(buf.full());
}

void test_operator_index_order() {
    // operator[](0) = oldest, operator[](size-1) = newest
    CircularBuffer<int, 3> buf;
    buf.push_back(10); buf.push_back(20); buf.push_back(30);
    ASSERT_EQ(10, buf[0]);
    ASSERT_EQ(20, buf[1]);
    ASSERT_EQ(30, buf[2]);
}

void test_ring_overwrite_one() {
    CircularBuffer<int, 3> buf;
    buf.push_back(1); buf.push_back(2); buf.push_back(3);
    buf.push_back(4);  // overwrites 1
    ASSERT_EQ(3, (int)buf.size());
    ASSERT_EQ(2, buf[0]);  // oldest is now 2
    ASSERT_EQ(3, buf[1]);
    ASSERT_EQ(4, buf[2]);  // newest
}

void test_ring_wraps_multiple_times() {
    CircularBuffer<int, 3> buf;
    for (int i = 0; i < 10; ++i) buf.push_back(i);
    // last 3 values pushed: 7, 8, 9
    ASSERT_EQ(3, (int)buf.size());
    ASSERT_EQ(7, buf[0]);
    ASSERT_EQ(8, buf[1]);
    ASSERT_EQ(9, buf[2]);
}

void test_clear_and_reuse() {
    CircularBuffer<int, 4> buf;
    buf.push_back(1); buf.push_back(2); buf.push_back(3);
    buf.clear();
    ASSERT_EQ(0, (int)buf.size());
    ASSERT_FALSE(buf.full());
    buf.push_back(99);
    ASSERT_EQ(1, (int)buf.size());
    ASSERT_EQ(99, buf[0]);
}

void test_single_element() {
    CircularBuffer<int, 4> buf;
    buf.push_back(42);
    ASSERT_EQ(42, buf[0]);
    ASSERT_EQ(42, *buf.begin());
    ASSERT_EQ(1, (int)(buf.end() - buf.begin()));
}

void test_partial_fill() {
    CircularBuffer<int, 10> buf;
    buf.push_back(5); buf.push_back(6); buf.push_back(7);
    ASSERT_EQ(3, (int)buf.size());
    ASSERT_FALSE(buf.full());
    // iteration over partial fill
    int sum = 0;
    for (int v : buf) sum += v;
    ASSERT_EQ(18, sum);
}

// ─── Iterator tests ──────────────────────────────────────────────────────────

void test_iterator_pre_increment() {
    CircularBuffer<int, 4> buf;
    for (int i = 1; i <= 4; ++i) buf.push_back(i);
    auto it = buf.begin();
    ASSERT_EQ(1, *it);
    ++it; ASSERT_EQ(2, *it);
    ++it; ASSERT_EQ(3, *it);
    ++it; ASSERT_EQ(4, *it);
    ++it; ASSERT_TRUE(it == buf.end());
}

void test_iterator_post_increment() {
    CircularBuffer<int, 3> buf;
    buf.push_back(10); buf.push_back(20); buf.push_back(30);
    auto it = buf.begin();
    ASSERT_EQ(10, *it++);
    ASSERT_EQ(20, *it++);
    ASSERT_EQ(30, *it);
}

void test_iterator_decrement() {
    CircularBuffer<int, 4> buf;
    for (int i = 1; i <= 4; ++i) buf.push_back(i);
    auto it = buf.end();
    --it; ASSERT_EQ(4, *it);
    --it; ASSERT_EQ(3, *it);
    --it; ASSERT_EQ(2, *it);
    --it; ASSERT_EQ(1, *it);
    ASSERT_TRUE(it == buf.begin());
}

void test_iterator_advance() {
    CircularBuffer<int, 5> buf;
    for (int i = 0; i < 5; ++i) buf.push_back(i * 10);
    auto it = buf.begin() + 3;
    ASSERT_EQ(30, *it);
    it = it - 2;
    ASSERT_EQ(10, *it);
}

void test_iterator_difference() {
    CircularBuffer<int, 4> buf;
    for (int i = 0; i < 4; ++i) buf.push_back(i);
    ASSERT_EQ(4, (int)(buf.end() - buf.begin()));
    ASSERT_EQ(2, (int)((buf.begin() + 2) - buf.begin()));
}

void test_iterator_subscript() {
    CircularBuffer<int, 4> buf;
    for (int i = 10; i <= 40; i += 10) buf.push_back(i);
    auto it = buf.begin();
    ASSERT_EQ(10, it[0]);
    ASSERT_EQ(20, it[1]);
    ASSERT_EQ(30, it[2]);
    ASSERT_EQ(40, it[3]);
}

void test_iterator_comparison() {
    CircularBuffer<int, 4> buf;
    for (int i = 0; i < 4; ++i) buf.push_back(i);
    auto a = buf.begin();
    auto b = buf.begin() + 2;
    ASSERT_TRUE (a <  b);
    ASSERT_FALSE(b <  a);
    ASSERT_TRUE (a <= b);
    ASSERT_TRUE (b >  a);
    ASSERT_TRUE (a != b);
    ASSERT_FALSE(a == b);
}

void test_iterator_after_wrap() {
    // Iterator must work correctly after ring has wrapped
    CircularBuffer<int, 3> buf;
    buf.push_back(1); buf.push_back(2); buf.push_back(3);
    buf.push_back(4);  // wraps: oldest=2,3,4
    int vals[3] = {-1, -1, -1};
    int i = 0;
    for (int v : buf) vals[i++] = v;
    ASSERT_EQ(2, vals[0]);
    ASSERT_EQ(3, vals[1]);
    ASSERT_EQ(4, vals[2]);
}

// ─── STL algorithm integration ───────────────────────────────────────────────

void test_std_accumulate() {
    CircularBuffer<int, 5> buf;
    for (int i = 1; i <= 5; ++i) buf.push_back(i);
    int sum = std::accumulate(buf.begin(), buf.end(), 0);
    ASSERT_EQ(15, sum);
}

void test_std_find_if() {
    CircularBuffer<int, 5> buf;
    for (int i = 1; i <= 5; ++i) buf.push_back(i);
    auto it = std::find_if(buf.begin(), buf.end(), [](int v){ return v > 3; });
    ASSERT_TRUE(it != buf.end());
    ASSERT_EQ(4, *it);
}

void test_std_sort() {
    CircularBuffer<int, 5> buf;
    buf.push_back(3); buf.push_back(1); buf.push_back(4);
    buf.push_back(1); buf.push_back(5);
    std::sort(buf.begin(), buf.end());
    ASSERT_EQ(1, buf[0]);
    ASSERT_EQ(1, buf[1]);
    ASSERT_EQ(3, buf[2]);
    ASSERT_EQ(4, buf[3]);
    ASSERT_EQ(5, buf[4]);
    ASSERT_TRUE(std::is_sorted(buf.begin(), buf.end()));
}

void test_std_copy_n() {
    CircularBuffer<int, 5> buf;
    for (int i = 10; i <= 50; i += 10) buf.push_back(i);
    std::vector<int> v(3);
    std::copy_n(buf.begin(), 3, v.begin());
    ASSERT_EQ(10, v[0]);
    ASSERT_EQ(20, v[1]);
    ASSERT_EQ(30, v[2]);
}

void test_std_min_max_element() {
    CircularBuffer<int, 6> buf;
    int vals[] = {7, 2, 9, 1, 5, 3};
    for (int v : vals) buf.push_back(v);
    auto mn = std::min_element(buf.begin(), buf.end());
    auto mx = std::max_element(buf.begin(), buf.end());
    ASSERT_EQ(1, *mn);
    ASSERT_EQ(9, *mx);
}

void test_std_count_if() {
    CircularBuffer<int, 6> buf;
    for (int v : {1, 2, 3, 4, 5, 6}) buf.push_back(v);
    int evens = (int)std::count_if(buf.begin(), buf.end(), [](int v){ return v % 2 == 0; });
    ASSERT_EQ(3, evens);
}

void test_std_distance() {
    CircularBuffer<int, 5> buf;
    for (int i = 0; i < 5; ++i) buf.push_back(i);
    ASSERT_EQ(5, (int)std::distance(buf.begin(), buf.end()));
    ASSERT_EQ(3, (int)std::distance(buf.begin(), buf.begin() + 3));
}

void test_std_sort_after_wrap() {
    CircularBuffer<int, 4> buf;
    // Push 6 elements into capacity-4 buffer to exercise the wrap path
    for (int v : {5, 3, 8, 1, 9, 2}) buf.push_back(v);
    // oldest 4: 8, 1, 9, 2
    std::sort(buf.begin(), buf.end());
    ASSERT_TRUE(std::is_sorted(buf.begin(), buf.end()));
    ASSERT_EQ(1, buf[0]);
    ASSERT_EQ(2, buf[1]);
    ASSERT_EQ(8, buf[2]);
    ASSERT_EQ(9, buf[3]);
}

// ─── Benchmark ───────────────────────────────────────────────────────────────

void bench_circular_vs_deque() {
    const int M = 1'000'000;

    // CircularBuffer: fixed-size window, constant time push (no heap)
    CircularBuffer<int, 1024> cb;
    auto t1 = bench::now();
    for (int i = 0; i < M; ++i) cb.push_back(i);
    bench::do_not_optimize(cb[0]);
    long long cb_ns = bench::elapsed_ns(t1);

    // std::deque: cap at 1024 by popping front (heap-backed, more overhead)
    std::deque<int> dq;
    auto t2 = bench::now();
    for (int i = 0; i < M; ++i) {
        if ((int)dq.size() >= 1024) dq.pop_front();
        dq.push_back(i);
    }
    bench::do_not_optimize(dq.front());
    long long dq_ns = bench::elapsed_ns(t2);

    std::printf("\n");
    bench::print_result("CircularBuffer<int,1024> 1M push_back",
                        (double)cb_ns / M, M);
    bench::print_result("deque (pop_front+push_back, cap 1024)",
                        (double)dq_ns / M, M);
    if (cb_ns > 0)
        std::printf("  Speedup: %.1fx\n", (double)dq_ns / cb_ns);
}

// ─── main ────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    RUN_TEST(test_size_and_full);
    RUN_TEST(test_operator_index_order);
    RUN_TEST(test_ring_overwrite_one);
    RUN_TEST(test_ring_wraps_multiple_times);
    RUN_TEST(test_clear_and_reuse);
    RUN_TEST(test_single_element);
    RUN_TEST(test_partial_fill);

    RUN_TEST(test_iterator_pre_increment);
    RUN_TEST(test_iterator_post_increment);
    RUN_TEST(test_iterator_decrement);
    RUN_TEST(test_iterator_advance);
    RUN_TEST(test_iterator_difference);
    RUN_TEST(test_iterator_subscript);
    RUN_TEST(test_iterator_comparison);
    RUN_TEST(test_iterator_after_wrap);

    RUN_TEST(test_std_accumulate);
    RUN_TEST(test_std_find_if);
    RUN_TEST(test_std_sort);
    RUN_TEST(test_std_copy_n);
    RUN_TEST(test_std_min_max_element);
    RUN_TEST(test_std_count_if);
    RUN_TEST(test_std_distance);
    RUN_TEST(test_std_sort_after_wrap);

    bench_circular_vs_deque();

    TEST_END();
}
