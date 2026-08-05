#include <cstdio>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <deque>
#include <algorithm>
#include <random>
#include "common.hpp"
#include <memory>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_node_map.hpp>
#include <boost/container/flat_map.hpp>

// ─── Part 1: Generate test data ─────────────────────────────────────────────

// Return a vector of N distinct random int keys.
// Seed std::mt19937 with 42 for reproducibility.
std::vector<int> generate_keys(int N) {
    std::vector<int> nums(N); //vector of size N
    std::iota(nums.begin(), nums.end(), 0); //fills it sequentially starting with 0
    static std::mt19937 generator(42);
    std::shuffle(nums.begin(), nums.end(), generator); //shuffles the order
    return nums;
}

// Return a copy of keys in a random (shuffled) order.
// Used as the lookup query set so access pattern isn't sequential.
std::vector<int> generate_shuffled(const std::vector<int>& keys) {
    std::vector<int> shuffled_keys(keys);
    static std::mt19937 generator(42);
    std::shuffle(shuffled_keys.begin(), shuffled_keys.end(), generator);
    return shuffled_keys;
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
    std::vector<int> vec;
    auto start = std::chrono::steady_clock::now();
    for (auto key : keys) {
        vec.push_back(key);
    }
    std::sort(vec.begin(), vec.end());
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(vec);
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    return nanoseconds;
}

long long bench_insert_map(const std::vector<int>& keys) {
    std::map<int,int> map;
    auto start = std::chrono::steady_clock::now();
    for (auto key : keys) {
        map[key] = key;
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(map);
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    return nanoseconds;
}

long long bench_insert_umap(const std::vector<int>& keys) {
    std::unordered_map<int, int> map;
    auto start = std::chrono::steady_clock::now();
    for (auto key : keys) {
        map[key] = key;
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(map);
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    return nanoseconds;
}

long long bench_insert_list(const std::vector<int>& keys) {
    std::list<int> list;
    auto start = std::chrono::steady_clock::now();
    for (auto& key : keys) {
        list.push_back(key);
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(list);
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    return nanoseconds;
}

long long bench_insert_deque(const std::vector<int>& keys) {
    std::deque<int> deque;
    auto start = std::chrono::steady_clock::now();
    for (auto& key : keys) {
        deque.push_back(key);
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(deque);
    auto time  = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    return nanoseconds;
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
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto& val : q) {
        auto itr = std::lower_bound(sorted_v.begin(), sorted_v.end(), val);
        if (itr != sorted_v.end()) { sum += *itr; }
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(sum);
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    return nanoseconds;
}

long long bench_lookup_map(const std::map<int,int>& m, const std::vector<int>& q) {
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto& val : q) {
        auto itr = m.find(val);
        if (itr != m.end()) { sum += itr -> second; }
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(sum);
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    return nanoseconds;
}

long long bench_lookup_umap(const std::unordered_map<int,int>& h, const std::vector<int>& q) {
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto& val : q) {
        auto itr = h.find(val);
        if (itr != h.end()) { sum += itr -> second; }
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(sum);
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    return nanoseconds;
}

// List and deque use O(n) linear scan — cap queries to avoid O(n²) blowup at large N.
static constexpr int LINEAR_SCAN_CAP = 1000;

long long bench_lookup_list(const std::list<int>& l, const std::vector<int>& q) {
    int sum = 0;
    int limit = std::min((int)q.size(), LINEAR_SCAN_CAP);
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < limit; ++i) {
        auto itr = std::find(l.begin(), l.end(), q[i]);
        if (itr != l.end()) { sum += *itr; }
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(sum);
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    // Scale back up so the ns/op calculation in main() stays consistent
    return ns * ((int)q.size() / limit);
}

long long bench_lookup_deque(const std::deque<int>& d, const std::vector<int>& q) {
    int sum = 0;
    int limit = std::min((int)q.size(), LINEAR_SCAN_CAP);
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < limit; ++i) {
        auto itr = std::find(d.begin(), d.end(), q[i]);
        if (itr != d.end()) { sum += *itr; }
    }
    auto end = std::chrono::steady_clock::now();
    bench::do_not_optimize(sum);
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return ns * ((int)q.size() / limit);
}

// ─── Part 4: Iteration benchmarks ───────────────────────────────────────────
// Sum all values in the container. bench::do_not_optimize the sum.
// Return TOTAL elapsed nanoseconds.

long long bench_iter_vector(const std::vector<int>& v) {
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto& val : v) {
        sum += val;
    }
    auto end = std::chrono::steady_clock::now();
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    bench::do_not_optimize(sum);
    return nanoseconds; 
}

long long bench_iter_map(const std::map<int,int>& m) {
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto& [key, value] : m) {
        sum += value;
    }
    auto end = std::chrono::steady_clock::now();
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    bench::do_not_optimize(sum);
    return nanoseconds; 
}

long long bench_iter_umap(const std::unordered_map<int,int>& h) {
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto& [key, value] : h) {
        sum += value;
    }
    auto end = std::chrono::steady_clock::now();
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    bench::do_not_optimize(sum);
    return nanoseconds; 
}

long long bench_iter_list(const std::list<int>& l) {
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto& val : l) {
        sum += val;
    }
    auto end = std::chrono::steady_clock::now();
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    bench::do_not_optimize(sum);
    return nanoseconds; 
}

long long bench_iter_deque(const std::deque<int>& d) {
    int sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto& val : d) {
        sum += val;
    }
    auto end = std::chrono::steady_clock::now();
    auto time = end - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    bench::do_not_optimize(sum);
    return nanoseconds; 
}

// ─── boost::unordered_flat_map (open addressing, cache-friendly) ─────────────
// Measures only what the label says: insert times the full fill (including any
// internal rehash), lookup and iter operate on a pre-built map passed in.

long long bench_insert_uflat(const std::vector<int>& keys) {
    boost::unordered_flat_map<int,int> h;
    h.reserve(keys.size());          // pre-size to avoid rehash noise mid-insert
    auto t = bench::now();
    for (int k : keys) h.emplace(k, k);
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(h);
    return ns;
}

long long bench_lookup_uflat(const boost::unordered_flat_map<int,int>& h,
                              const std::vector<int>& q) {
    long long sum = 0;
    auto t = bench::now();
    for (int k : q) {
        auto it = h.find(k);
        if (it != h.end()) sum += it->second;
    }
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(sum);
    return ns;
}

long long bench_iter_uflat(const boost::unordered_flat_map<int,int>& h) {
    long long sum = 0;
    auto t = bench::now();
    for (auto& [k, v] : h) sum += v;
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(sum);
    return ns;
}

// ─── boost::container::flat_map (sorted contiguous array, O(log n)) ──────────
// Same layout guarantee as a sorted vector but with std::map-compatible API.
// Lookup is O(log n) binary search like std::map, but data is contiguous so
// cache prefetcher can help — compare against std::map (pointer-chased tree).

long long bench_insert_flatmap(const std::vector<int>& keys) {
    boost::container::flat_map<int,int> m;
    m.reserve(keys.size());          // pre-size backing vector
    auto t = bench::now();
    for (int k : keys) m.emplace(k, k);
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(m);
    return ns;
}

long long bench_lookup_flatmap(const boost::container::flat_map<int,int>& m,
                                const std::vector<int>& q) {
    long long sum = 0;
    auto t = bench::now();
    for (int k : q) {
        auto it = m.find(k);
        if (it != m.end()) sum += it->second;
    }
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(sum);
    return ns;
}

long long bench_iter_flatmap(const boost::container::flat_map<int,int>& m) {
    long long sum = 0;
    auto t = bench::now();
    for (auto& [k, v] : m) sum += v;
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(sum);
    return ns;
}

// ─── boost::unordered_flat_map<int, unique_ptr<int>> ─────────────────────────
// Values are heap-allocated via unique_ptr. The flat array stores the
// unique_ptr objects inline (they move on rehash, which is fine), but the
// pointed-to ints never move — giving stable int* across rehashes.
// Cost: one extra heap alloc per insert, one pointer dereference per lookup.

using FlatPtrMap = boost::unordered_flat_map<int, std::unique_ptr<int>>;

long long bench_insert_uflat_ptr(const std::vector<int>& keys) {
    FlatPtrMap h;
    h.reserve(keys.size());
    auto t = bench::now();
    for (int k : keys) h.emplace(k, std::make_unique<int>(k));
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(h);
    return ns;
}

long long bench_lookup_uflat_ptr(const FlatPtrMap& h, const std::vector<int>& q) {
    long long sum = 0;
    auto t = bench::now();
    for (int k : q) {
        auto it = h.find(k);
        if (it != h.end()) sum += *it->second;   // dereference unique_ptr
    }
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(sum);
    return ns;
}

long long bench_iter_uflat_ptr(const FlatPtrMap& h) {
    long long sum = 0;
    auto t = bench::now();
    for (auto& [k, v] : h) sum += *v;            // dereference unique_ptr
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(sum);
    return ns;
}

// ─── boost::unordered_node_map ────────────────────────────────────────────────
// Each key-value pair lives in its own heap node. The flat array holds pointers
// to those nodes. On rehash only the pointer array is rebuilt — nodes stay put,
// so int& references and int* pointers into the map are never invalidated.
// This is the clean built-in alternative to manually wrapping values in unique_ptr.

using NodeMap = boost::unordered_node_map<int, int>;

long long bench_insert_node_map(const std::vector<int>& keys) {
    NodeMap h;
    h.reserve(keys.size());
    auto t = bench::now();
    for (int k : keys) h.emplace(k, k);
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(h);
    return ns;
}

long long bench_lookup_node_map(const NodeMap& h, const std::vector<int>& q) {
    long long sum = 0;
    auto t = bench::now();
    for (int k : q) {
        auto it = h.find(k);
        if (it != h.end()) sum += it->second;
    }
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(sum);
    return ns;
}

long long bench_iter_node_map(const NodeMap& h) {
    long long sum = 0;
    auto t = bench::now();
    for (auto& [k, v] : h) sum += v;
    long long ns = bench::elapsed_ns(t);
    bench::do_not_optimize(sum);
    return ns;
}

// ─── main() ─────────────────────────────────────────────────────────────────
int main() {
    const int N = 1'000'000;

    const auto keys    = generate_keys(N);
    const auto queries = generate_shuffled(keys);

    // ── vector (sorted + lower_bound) ────────────────────────────────────────
    long long v_ins  = bench_insert_vector(keys);
    std::vector<int> sorted_v(keys);
    std::sort(sorted_v.begin(), sorted_v.end());

    long long v_look = bench_lookup_vector(sorted_v, queries);
    long long v_iter = bench_iter_vector(sorted_v);

    // ── std::map ─────────────────────────────────────────────────────────────
    long long m_ins  = bench_insert_map(keys);
    std::map<int,int> map_m;
    for (auto& key : keys) {
        map_m[key] = key;
    }

    long long m_look = bench_lookup_map(map_m, queries);
    long long m_iter = bench_iter_map(map_m);

    // ── std::unordered_map ───────────────────────────────────────────────────
    long long h_ins  = bench_insert_umap(keys);
    std::unordered_map<int,int> umap_h;
    for (auto& key : keys) {
        umap_h[key] = key;
    }
    long long h_look = bench_lookup_umap(umap_h, queries);
    long long h_iter = bench_iter_umap(umap_h);

    // ── std::list ────────────────────────────────────────────────────────────
    long long l_ins  = bench_insert_list(keys);
    std::list<int> list_l;
    for (auto& key : keys) {
        list_l.push_back(key);
    }
    long long l_look = bench_lookup_list(list_l, queries);
    long long l_iter = bench_iter_list(list_l);

    // ── std::deque ───────────────────────────────────────────────────────────
    long long d_ins  = bench_insert_deque(keys);
    std::deque<int> deque_d;
    for (auto& key : keys) {
        deque_d.push_back(key);
    }
    long long d_look = bench_lookup_deque(deque_d, queries);
    long long d_iter = bench_iter_deque(deque_d);

    // ── boost::unordered_flat_map ─────────────────────────────────────────────
    long long uf_ins  = bench_insert_uflat(keys);
    boost::unordered_flat_map<int,int> uflat_h;
    uflat_h.reserve(keys.size());
    for (int k : keys) uflat_h.emplace(k, k);
    long long uf_look = bench_lookup_uflat(uflat_h, queries);
    long long uf_iter = bench_iter_uflat(uflat_h);

    // ── boost::container::flat_map ────────────────────────────────────────────
    long long fm_ins  = bench_insert_flatmap(keys);
    boost::container::flat_map<int,int> flat_m;
    flat_m.reserve(keys.size());
    for (int k : keys) flat_m.emplace(k, k);
    long long fm_look = bench_lookup_flatmap(flat_m, queries);
    long long fm_iter = bench_iter_flatmap(flat_m);

    // ── boost::unordered_flat_map<int, unique_ptr<int>> ───────────────────────
    long long ufp_ins  = bench_insert_uflat_ptr(keys);
    FlatPtrMap uflat_ptr_h;
    uflat_ptr_h.reserve(keys.size());
    for (int k : keys) uflat_ptr_h.emplace(k, std::make_unique<int>(k));
    long long ufp_look = bench_lookup_uflat_ptr(uflat_ptr_h, queries);
    long long ufp_iter = bench_iter_uflat_ptr(uflat_ptr_h);

    // ── boost::unordered_node_map ─────────────────────────────────────────────
    long long nm_ins  = bench_insert_node_map(keys);
    NodeMap node_h;
    node_h.reserve(keys.size());
    for (int k : keys) node_h.emplace(k, k);
    long long nm_look = bench_lookup_node_map(node_h, queries);
    long long nm_iter = bench_iter_node_map(node_h);

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

    row("std::list",            l_ins, l_look, l_iter);
    row("std::deque",           d_ins, d_look, d_iter);
    row("vector+lower_bound",   v_ins, v_look, v_iter);
    row("std::map",             m_ins, m_look, m_iter);
    row("unordered_map",        h_ins, h_look, h_iter);
    row("boost::unord_flat_map",uf_ins,  uf_look,  uf_iter);
    row("boost::flat_map",      fm_ins,  fm_look,  fm_iter);
    // row("flat_map+unique_ptr",  ufp_ins, ufp_look, ufp_iter);
    row("boost::node_map (heap allocation)",      nm_ins,  nm_look,  nm_iter);

    // ── Answer these questions based on YOUR output ───────────────────────────
    //
    // Q1: Which container has the fastest lookup at N=100,000? Why?
    //  boost::unord_flat_map has the fastest lookup at 1.8 ns/op, followed closely by std::unordered_map
    //  this is because boost::unord_flat_map uses flat hashing, storing all data inside a linear block of memory, 
    //  which is more cache friendly. The slight difference in std::unordered_map's lookup time is probably due to 
    //  a few cache misses, but this is still hashing so extremely fast compared to the others, which are iterative 
    //  lookups.
    //
    // Q2: Why is vector+lower_bound lookup slower than unordered_map
    //     but faster than map? Both lower_bound and map::find are O(log n).
    //  vector+lower_bound is slower than unordered_map because unordered_map uses hashing wherease lower_bound is binary search,
    //  O(1) vs O(log(N)). it is actually slower than map as well, likely because of cache behavior. since map
    //  is a tree, the top few levels get traversed every single time and will sit hot in cache. the binary
    //  search jump pattern is not as predictable, touching more cache lines. unless the whole vector can fit in cache,
    //  the map will be faster to lookup in.
    //
    // Q3: For an order book's price level → quantity mapping (frequent
    //     best-price touch, occasional insert at arbitrary price levels),
    //     which container would you use in production? Why?
    //  probably a boost::unord_flat_map. fastest lookup, which we'll do frequently, and pretty good insert as well
    //  for when we do that. 
    //
    // Q4: Why does std::list iterate slower than std::vector even though
    //     both are O(n)? What does this tell you about CPU cache behavior?
    //  std::list uses node-based elements so there's a lot of pointer chasing, whereas vector uses a contiguous
    //  block of memory, allowing prefetching to be more effective

    return 0;
}
