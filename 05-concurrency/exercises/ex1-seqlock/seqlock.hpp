#pragma once

/* TODO: add your #include lines */

// Seqlock: a synchronization primitive for read-mostly data.
//
// Writer protocol:
//   1. Increment sequence counter (must be odd = "write in progress")
//   2. Write data
//   3. Increment sequence counter (now even = "data consistent")
//
// Reader protocol:
//   1. Read seq. If odd, a write is in progress -- spin.
//   2. Read data
//   3. Read seq again. If changed, retry from step 1.
//
// Memory ordering:
//   The seq counter must use acquire/release semantics to ensure
//   the data writes/reads are properly ordered relative to the counter updates.

template<typename T>
class SeqLock {
public:
    SeqLock() : seq_(0), data_{} {}

    // TODO: Write data_ = val atomically under the sequence protocol.
    void write(const T& val) { (void)val; /* TODO: odd seq, store data, even seq */ }

    // TODO: Read data_ safely. Retry if a write is in progress or data was torn.
    // Returns the consistent value.
    T read() const { return data_; /* TODO: load seq, read data_, verify seq unchanged */ }

    // Convenience: get the current sequence number (for testing)
    uint64_t seq() const { return seq_.load(std::memory_order_acquire); }

private:
    std::atomic<uint64_t> seq_;
    T data_;
    // Pad to avoid false sharing between seq_ and data_
    // (T might be small, seq_ might be on same line as other things)
};
