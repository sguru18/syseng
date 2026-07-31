/* TODO: add your #include lines */
// You will need: <cstdio>, <vector>, <map>, <unordered_map>, <list>, <deque>,
//               <algorithm>, <random>, and "common.hpp"

// ─── Part 1: Generate test data ─────────────────────────────────────────────

// Return a vector of N distinct random int keys.
// Seed std::mt19937 with 42 for reproducibility.
std::vector<int> generate_keys(int N) {
    // TODO
    (void)N;
    return {};
}

// Return a copy of keys in a random (shuffled) order.
// Used as the lookup query set so access pattern isn't sequential.
std::vector<int> generate_shuffled(const std::vector<int>& keys) {
    // TODO
    return {};
}

// ─── Part 2: Insert benchmarks ──────────────────────────────────────────────
// Each function creates a fresh container, inserts all keys, and returns
// the TOTAL elapsed nanoseconds.
// Use bench::do_not_optimize() on the container before returning to prevent DCE.
//
// For vector:        push_back all keys, then std::sort.
// For map/umap:      insert as (key → key) value pairs.
// For list/deque:    push_back all keys.

long long bench_insert_vector(const std::vector<int>& keys) {
    // TODO
    (void)keys;
    return 0;
}

long long bench_insert_map(const std::vector<int>& keys) {
    // TODO
    (void)keys;
    return 0;
}

long long bench_insert_umap(const std::vector<int>& keys) {
    // TODO
    (void)keys;
    return 0;
}

long long bench_insert_list(const std::vector<int>& keys) {
    // TODO
    (void)keys;
    return 0;
}

long long bench_insert_deque(const std::vector<int>& keys) {
    // TODO
    (void)keys;
    return 0;
}

// ─── Part 3: Lookup benchmarks ──────────────────────────────────────────────
// Look up each element of q in the pre-built container.
// Accumulate found values into a running sum; bench::do_not_optimize the sum.
// Return TOTAL elapsed nanoseconds.
//
// For vector:        std::lower_bound on the sorted vector.
// For map/umap:      container.find().
// For list/deque:    std::find (linear scan).

long long bench_lookup_vector(const std::vector<int>& sorted_v, const std::vector<int>& q) {
    // TODO
    (void)sorted_v; (void)q;
    return 0;
}

long long bench_lookup_map(const std::map<int,int>& m, const std::vector<int>& q) {
    // TODO
    (void)m; (void)q;
    return 0;
}

long long bench_lookup_umap(const std::unordered_map<int,int>& h, const std::vector<int>& q) {
    // TODO
    (void)h; (void)q;
    return 0;
}

long long bench_lookup_list(const std::list<int>& l, const std::vector<int>& q) {
    // TODO
    (void)l; (void)q;
    return 0;
}

long long bench_lookup_deque(const std::deque<int>& d, const std::vector<int>& q) {
    // TODO
    (void)d; (void)q;
    return 0;
}

// ─── Part 4: Iteration benchmarks ───────────────────────────────────────────
// Sum all values in the container. bench::do_not_optimize the sum.
// Return TOTAL elapsed nanoseconds.

long long bench_iter_vector(const std::vector<int>& v) {
    // TODO
    (void)v;
    return 0;
}

long long bench_iter_map(const std::map<int,int>& m) {
    // TODO
    (void)m;
    return 0;
}

long long bench_iter_umap(const std::unordered_map<int,int>& h) {
    // TODO
    (void)h;
    return 0;
}

long long bench_iter_list(const std::list<int>& l) {
    // TODO
    (void)l;
    return 0;
}

long long bench_iter_deque(const std::deque<int>& d) {
    // TODO
    (void)d;
    return 0;
}

// ─── main() ─────────────────────────────────────────────────────────────────
int main() {
    const int N = 100'000;

    const auto keys    = generate_keys(N);
    const auto queries = generate_shuffled(keys);

    // ── vector (sorted + lower_bound) ────────────────────────────────────────
    long long v_ins  = bench_insert_vector(keys);
    // TODO: build sorted_v (a sorted copy of keys, not timed)
    std::vector<int> sorted_v;
    long long v_look = bench_lookup_vector(sorted_v, queries);
    long long v_iter = bench_iter_vector(sorted_v);

    // ── std::map ─────────────────────────────────────────────────────────────
    long long m_ins  = bench_insert_map(keys);
    // TODO: build map_m (a std::map<int,int> from keys, not timed)
    std::map<int,int> map_m;
    long long m_look = bench_lookup_map(map_m, queries);
    long long m_iter = bench_iter_map(map_m);

    // ── std::unordered_map ───────────────────────────────────────────────────
    long long h_ins  = bench_insert_umap(keys);
    // TODO: build umap_h (a std::unordered_map<int,int> from keys, not timed)
    std::unordered_map<int,int> umap_h;
    long long h_look = bench_lookup_umap(umap_h, queries);
    long long h_iter = bench_iter_umap(umap_h);

    // ── std::list ────────────────────────────────────────────────────────────
    long long l_ins  = bench_insert_list(keys);
    // TODO: build list_l (a std::list<int> from keys, not timed)
    std::list<int> list_l;
    long long l_look = bench_lookup_list(list_l, queries);
    long long l_iter = bench_iter_list(list_l);

    // ── std::deque ───────────────────────────────────────────────────────────
    long long d_ins  = bench_insert_deque(keys);
    // TODO: build deque_d (a std::deque<int> from keys, not timed)
    std::deque<int> deque_d;
    long long d_look = bench_lookup_deque(deque_d, queries);
    long long d_iter = bench_iter_deque(deque_d);

    // ── Sanity check: sorted vector and unordered_map must agree ─────────────
    // For the first 100 queries: verify that lower_bound(sorted_v, q) and
    // umap_h.find(q) return the same value. Print "Sanity OK" or abort.
    // TODO
    std::printf("(sanity check not yet implemented)\n");

    // ── Results table ─────────────────────────────────────────────────────────
    std::printf("\nN = %d\n", N);
    std::printf("%-22s %15s %15s %15s\n",
        "Container", "Insert (ns/op)", "Lookup (ns/op)", "Iter (ns/elem)");
    std::printf("%-22s %15s %15s %15s\n",
        "──────────────────────",
        "───────────────", "───────────────", "───────────────");

    auto row = [&](const char* name, long long ins, long long look, long long iter) {
        std::printf("%-22s %15.1f %15.1f %15.1f\n",
            name,
            (double)ins  / N,
            (double)look / N,
            (double)iter / N);
    };

    row("vector+lower_bound",   v_ins, v_look, v_iter);
    row("std::map",             m_ins, m_look, m_iter);
    row("unordered_map",        h_ins, h_look, h_iter);
    row("std::list",            l_ins, l_look, l_iter);
    row("std::deque",           d_ins, d_look, d_iter);

    // ── Answer these questions based on YOUR output ───────────────────────────
    //
    // Q1: Which container has the fastest lookup at N=100,000? Why?
    //
    //
    // Q2: Why is vector+lower_bound lookup slower than unordered_map
    //     but faster than map? Both lower_bound and map::find are O(log n).
    //
    //
    // Q3: For an order book's price level → quantity mapping (frequent
    //     best-price touch, occasional insert at arbitrary price levels),
    //     which container would you use in production? Why?
    //
    //
    // Q4: Why does std::list iterate slower than std::vector even though
    //     both are O(n)? What does this tell you about CPU cache behavior?
    //

    return 0;
}
