#pragma once
/* TODO: add your #include lines */
// You will need: <cstddef>, <iterator>

// ─────────────────────────────────────────────────────────────────────────────
// CircularBuffer<T, N>
//
// Fixed-size ring buffer backed by a stack-allocated array.
// - push_back(val): add element; if full, silently overwrites the oldest.
// - operator[](i):  0 = oldest element, size()-1 = newest.
// - Iterator:       satisfies LegacyRandomAccessIterator (std::sort etc. work).
//
// Real use: inter-thread event queues, rolling VWAP windows, market-data
// tick history where you only care about the last N events.
// ─────────────────────────────────────────────────────────────────────────────

template<typename T, std::size_t N>
class CircularBuffer {
public:

    // ── Iterator ─────────────────────────────────────────────────────────────
    // Stores a pointer to the parent buffer and a logical index (0 = oldest).
    struct Iterator {
        // Required typedefs for STL algorithms
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using iterator_category = std::random_access_iterator_tag;

        CircularBuffer* buf_;
        std::ptrdiff_t  pos_;   // logical index: 0 = oldest element

        // TODO: implement operator* and operator[].
        // The mapping from logical index pos_ to physical array index is:
        //   physical = (buf_->head_ - buf_->count_ + pos_ + N) % N
        // Use buf_->operator[](pos_) once you implement it.

        reference operator*() const {
            // TODO: return the element at logical index pos_
            return buf_->data_[0];  // placeholder -- replace with correct mapping
        }

        // Iterator arithmetic -- advance/retreat pos_.
        // These are provided correct in the stub so STL algorithms don't crash.
        // TODO: once you implement operator*, verify the traversal is correct.
        Iterator& operator++()   { ++pos_; return *this; }
        Iterator  operator++(int){ auto t = *this; ++pos_; return t; }
        Iterator& operator--()   { --pos_; return *this; }
        Iterator  operator--(int){ auto t = *this; --pos_; return t; }

        Iterator& operator+=(difference_type n) { pos_ += n; return *this; }
        Iterator& operator-=(difference_type n) { pos_ -= n; return *this; }

        Iterator operator+(difference_type n) const { return {buf_, pos_ + n}; }
        Iterator operator-(difference_type n) const { return {buf_, pos_ - n}; }
        difference_type operator-(const Iterator& o) const { return pos_ - o.pos_; }

        reference operator[](difference_type n) const {
            // TODO: equivalent to *(*this + n)
            (void)n;
            return buf_->data_[0];  // placeholder
        }

        bool operator==(const Iterator& o) const { return pos_ == o.pos_; }
        bool operator!=(const Iterator& o) const { return pos_ != o.pos_; }
        bool operator< (const Iterator& o) const { return pos_ <  o.pos_; }
        bool operator<=(const Iterator& o) const { return pos_ <= o.pos_; }
        bool operator> (const Iterator& o) const { return pos_ >  o.pos_; }
        bool operator>=(const Iterator& o) const { return pos_ >= o.pos_; }
    };

    // Free function so Iterator + n also works (n + iter)
    friend Iterator operator+(typename Iterator::difference_type n, const Iterator& it) {
        return it + n;
    }

    // ── Buffer API ───────────────────────────────────────────────────────────

    // Add val to back. Overwrites oldest element when full.
    void push_back(const T& val) {
        // TODO:
        //   data_[head_] = val;
        //   head_ = (head_ + 1) % N;
        //   if (count_ < N) ++count_; (or advance head_ unconditionally)
        (void)val;
    }

    // Logical index: 0 = oldest, size()-1 = newest.
    T& operator[](std::size_t i) {
        // TODO: return data_[(head_ - count_ + i + N) % N]
        (void)i;
        return data_[0];  // placeholder
    }
    const T& operator[](std::size_t i) const {
        // TODO: same mapping as above
        (void)i;
        return data_[0];  // placeholder
    }

    std::size_t size()     const { return count_; }
    bool        full()     const { return count_ == N; }
    std::size_t capacity() const { return N; }

    void clear() {
        head_  = 0;
        count_ = 0;
    }

    // ── Iterator factory ─────────────────────────────────────────────────────
    Iterator begin()       { return {this,  0}; }
    Iterator end()         { return {this,  static_cast<std::ptrdiff_t>(count_)}; }
    // const versions: const_cast is safe for read-only algorithms
    Iterator begin() const { return {const_cast<CircularBuffer*>(this), 0}; }
    Iterator end()   const { return {const_cast<CircularBuffer*>(this),
                                     static_cast<std::ptrdiff_t>(count_)}; }

private:
    // Fixed backing store and ring state.
    // head_  = index of the NEXT write slot (one past newest)
    // count_ = number of valid elements in [0, N]
    T           data_[N]{};
    std::size_t head_  = 0;
    std::size_t count_ = 0;
};
