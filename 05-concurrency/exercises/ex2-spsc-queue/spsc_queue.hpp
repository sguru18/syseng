#pragma once

/* TODO: add your #include lines */

// Single-Producer Single-Consumer lock-free ring buffer.
//
// Key insight: only the producer writes head_, only the consumer writes tail_.
// Each thread only reads the other's pointer (never writes it), so no CAS needed.
// Use memory_order_release on writes, memory_order_acquire on reads.
//
// The queue is full when (head_ + 1) % N == tail_.
// The queue is empty when head_ == tail_.

template<typename T, std::size_t N>
class SPSCQueue {
    static_assert((N & (N - 1)) == 0, "N must be a power of 2");

public:
    SPSCQueue() : head_(0), tail_(0) {}

    // TODO: push val. Returns false if queue is full.
    // Called from producer thread only.
    bool push(const T& val) { (void)val; return false; /* TODO */ }

    // TODO: push with move semantics
    bool push(T&& val) { (void)val; return false; /* TODO */ }

    // TODO: pop. Returns std::nullopt if empty.
    // Called from consumer thread only.
    std::optional<T> pop() { return std::nullopt; /* TODO */ }

    // TODO: try_pop into out. Returns false if empty. (Avoids optional overhead)
    bool try_pop(T& out) { (void)out; return false; /* TODO */ }

    std::size_t size() const {
        // Approximate (head/tail may be stale, but safe to read individually)
        std::size_t h = head_.load(std::memory_order_relaxed);
        std::size_t t = tail_.load(std::memory_order_relaxed);
        return (h - t) & (N - 1);
    }

    bool empty() const {
        return head_.load(std::memory_order_relaxed) ==
               tail_.load(std::memory_order_relaxed);
    }

    bool full() const {
        std::size_t h = head_.load(std::memory_order_relaxed);
        std::size_t t = tail_.load(std::memory_order_relaxed);
        return ((h + 1) & (N - 1)) == (t & (N - 1));
    }

private:
    // IMPORTANT: head_ and tail_ on separate cache lines to avoid false sharing
    alignas(64) std::atomic<std::size_t> head_;
    alignas(64) std::atomic<std::size_t> tail_;
    std::array<T, N> buffer_;
};
