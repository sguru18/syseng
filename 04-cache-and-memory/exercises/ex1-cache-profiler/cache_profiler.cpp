/* TODO: add your #include lines */

// ─── Part 1: Pointer-chasing latency measurement ─────────────────────────────
//
// The idea: build a linked list through an array of `n` nodes.
// Shuffle the pointer chain so each access is unpredictable to the prefetcher.
// Time how long it takes to chase N pointers.
// ns_per_access = elapsed_ns / steps
//
// At small n: entire array fits in L1  → fast (~1 ns).
// At large n: array spills to RAM      → slow (~100 ns).

struct Node {
    Node* next;
    char  padding[64 - sizeof(Node*)];  // one node = one cache line
};

// TODO: Allocate n Node objects, set next pointers to form a random permutation
//       cycle (every node visited exactly once). Return a pointer to the head.
Node* build_random_chain(size_t n) {
    return nullptr;
}

// TODO: Follow the next pointer `steps` times starting from head.
//       Call bench::do_not_optimize(node) each iteration to prevent the compiler
//       from eliminating the loop. Return the final node pointer.
Node* chase_pointers(Node* head, size_t steps) {
    return head;
}

// ─── Part 2: False sharing demonstration ─────────────────────────────────────
//
// When two threads write to variables on the SAME cache line, the MESI protocol
// forces the line to bounce between cores on every write. This is false sharing:
// there is no logical sharing of data, but the hardware doesn't know that.

// Two counters that land on the same 64-byte cache line.
struct SharedCounters {
    volatile int64_t a;
    volatile int64_t b;
};

// Two counters each padded onto their own 64-byte cache line.
struct alignas(64) PaddedCounters {
    volatile int64_t a;
    char pad[64 - sizeof(int64_t)];
    volatile int64_t b;
};

// TODO: Spawn two threads. Thread 0 increments counters.a n_iters times;
//       thread 1 increments counters.b n_iters times (using SharedCounters).
//       Join both threads. Return total elapsed nanoseconds.
long long benchmark_false_sharing(size_t n_iters) {
    return 0LL;
}

// TODO: Same as above but using PaddedCounters.
long long benchmark_no_false_sharing(size_t n_iters) {
    return 0LL;
}

// ─── main ─────────────────────────────────────────────────────────────────────
int main() {
    printf("=== Cache Hierarchy Profiler ===\n\n");

    // Array sizes to profile (in bytes)
    const size_t sizes[] = {
        1ul << 10,   //   1 KB
        4ul << 10,   //   4 KB
        32ul << 10,  //  32 KB
        256ul << 10, // 256 KB
        4ul << 20,   //   4 MB
        64ul << 20   //  64 MB
    };
    const size_t steps = 1u << 22;  // pointer chases per measurement

    printf("Array size    | Latency per access | Likely location\n");
    printf("--------------|--------------------|-----------------\n");

    // TODO: For each size in `sizes`:
    //   1. Compute n = size / sizeof(Node)
    //   2. Build the random chain: Node* head = build_random_chain(n)
    //   3. Warm up: chase_pointers(head, n) once (not timed)
    //   4. Time `steps` pointer chases: auto t = bench::now(); chase_pointers(head, steps); auto ns = bench::elapsed_ns(t);
    //   5. ns_per_access = (double)ns / steps
    //   6. Classify: <4 ns="L1"  <15 ns="L2/L3"  <60 ns="L3"  else="RAM"
    //   7. Print: printf("  %7zu KB | %15.1f ns   | %s\n", size/1024, ns_per_access, location)
    //   8. Free the allocated nodes (delete[] the raw array from build_random_chain)

    // ─── Part 2 ───────────────────────────────────────────────────────────────
    const size_t iters = 50'000'000;
    printf("\n=== False Sharing ===\n");

    // TODO: long long shared_ns = benchmark_false_sharing(iters);
    // TODO: long long padded_ns = benchmark_no_false_sharing(iters);
    // TODO: double shared_ns_per  = (double)shared_ns / iters;
    // TODO: double padded_ns_per  = (double)padded_ns / iters;
    // TODO: double speedup        = shared_ns_per / padded_ns_per;
    // TODO: printf("  Shared:  %.1f ns per iteration\n", shared_ns_per);
    // TODO: printf("  Padded:  %.1f ns per iteration\n", padded_ns_per);
    // TODO: printf("  Speedup: %.1fx\n", speedup);

    return 0;
}
