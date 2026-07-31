#pragma once

/* TODO: add your #include lines */

// Multi-Producer Multi-Consumer lock-free queue.
// Based on Dmitry Vyukov's MPMC queue design.
//
// Each slot has a sequence number:
//   - slot is ready to write when seq == position
//   - slot is ready to read when seq == position + 1
//
// Producer: CAS on enqueue_pos_ to claim a slot, write data, increment seq.
// Consumer: CAS on dequeue_pos_ to claim a slot, read data, increment seq.

template<typename T, std::size_t N>
class MPMCQueue {
    static_assert((N & (N - 1)) == 0, "N must be a power of 2");

    struct Slot {
        std::atomic<std::size_t> seq;
        T data;
    };

public:
    MPMCQueue() : enqueue_pos_(0), dequeue_pos_(0) {
        for (std::size_t i = 0; i < N; ++i) buffer_[i].seq.store(i, std::memory_order_relaxed);
        /* TODO: initialize slot sequence numbers to their index */
    }

    // TODO: push val. Spins if full (blocking push).
    void push(const T& val) { (void)val; /* TODO: CAS enqueue_pos_, write, advance seq */ }

    // TODO: try_push. Returns false if full (non-blocking).
    bool try_push(const T& val) { (void)val; return false; /* TODO */ }

    // TODO: pop into out. Spins if empty (blocking pop).
    void pop(T& out) { (void)out; /* TODO: CAS dequeue_pos_, read, advance seq */ }

    // TODO: try_pop. Returns false if empty.
    bool try_pop(T& out) { (void)out; return false; /* TODO */ }

private:
    alignas(64) std::atomic<std::size_t> enqueue_pos_;
    alignas(64) std::atomic<std::size_t> dequeue_pos_;
    std::array<Slot, N> buffer_;
};

// ─── False sharing demonstration ─────────────────────────────────────────────

// BAD: both counters on same cache line
struct BadCounters {
    std::atomic<int64_t> producer_count{0};
    std::atomic<int64_t> consumer_count{0};
};

// GOOD: counters on separate cache lines
struct GoodCounters {
    alignas(64) std::atomic<int64_t> producer_count{0};
    char _pad[64 - sizeof(std::atomic<int64_t>)];
    alignas(64) std::atomic<int64_t> consumer_count{0};
};

// TODO: benchmark_false_sharing(n_iters) -- two threads increment BadCounters, return ns
inline long long benchmark_false_sharing(std::size_t n_iters) { (void)n_iters; return 0; /* TODO */ }

// TODO: benchmark_no_false_sharing(n_iters) -- same but GoodCounters
inline long long benchmark_no_false_sharing(std::size_t n_iters) { (void)n_iters; return 1; /* TODO */ }
