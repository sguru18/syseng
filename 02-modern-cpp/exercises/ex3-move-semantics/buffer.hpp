/* TODO: add your #include lines */
#pragma once

// ─────────────────────────────────────────────────────────────────────────────
// Buffer: a heap-allocated byte array with full copy/move semantics.
//
// Instrument every copy and move via static counters so you can observe
// exactly what the compiler does in each scenario.
// ─────────────────────────────────────────────────────────────────────────────
class Buffer {
public:
    // Allocate `size` bytes, zero them.
    explicit Buffer(size_t size) : data_(nullptr), size_(0) { 
        this -> data_ = new uint8_t[size]{};
        this -> size_ = size;
    }

    // Deep copy: allocate new memory, copy bytes. Increment s_copies_.
    Buffer(const Buffer& other) : data_(nullptr), size_(0) { 
        this -> data_ = new uint8_t[other.size_]{};
        this -> size_ = other.size_;

        size_t num = 0;
        uint8_t* ptr = this -> data_;
        uint8_t* ptr2 = other.data_;
        while (num++ < other.size_) {
            *ptr = *ptr2;
            ptr++;
            ptr2++;
        }

        this -> s_copies_ += 1;
    }

    // Copy assign: release current, deep copy from other. Increment s_copies_.
    Buffer& operator=(const Buffer& other) { 
        delete[] this -> data_;

        this -> data_ = new uint8_t[other.size_]{};
        this -> size_ = other.size_;
        
        size_t num = 0;
        uint8_t* ptr = this -> data_;
        uint8_t* ptr2 = other.data_;
        while (num++ < other.size_) {
            *ptr = *ptr2;
            ptr++;
            ptr2++;
        }

        this -> s_copies_ += 1;

        return *this; 
    }

    // Move: steal data_ and size_ from other, set other to nullptr/0. Increment s_moves_.
    Buffer(Buffer&& other) noexcept : data_(nullptr), size_(0) { 
        this -> data_ = other.data_;
        this -> size_ = other.size_;

        other.data_ = nullptr;
        other.size_ = 0;

        this -> s_moves_ += 1;
    }

    // Move assign: release current, steal from other. Increment s_moves_.
    Buffer& operator=(Buffer&& other) noexcept { 
        delete[] this -> data_;

        this -> data_ = other.data_;
        this -> size_ = other.size_;

        other.data_ = nullptr;
        other.size_ = 0;

        this -> s_moves_ += 1;
        
        return *this; 
    }

    // Release owned memory.
    ~Buffer() { 
        delete[] this -> data_;
    }

    size_t         size()        const { return size_; }
    uint8_t*       data()              { return data_; }
    const uint8_t* data()        const { return data_; }
    uint8_t&       operator[](size_t i){ return data_[i]; }
    const uint8_t& operator[](size_t i) const { return data_[i]; }

    // Reset counters before a benchmark run.
    static void reset_counters() { s_copies_ = s_moves_ = 0; }
    static int  copy_count()     { return s_copies_; }
    static int  move_count()     { return s_moves_;  }

private:
    uint8_t* data_;
    size_t   size_;

    static inline int s_copies_ = 0;
    static inline int s_moves_  = 0;
};

// ─────────────────────────────────────────────────────────────────────────────
// make_buffer: perfect-forwarding factory.
//
// Forwards all arguments to Buffer's constructor without extra copies.
// Use a variadic template + std::forward.
// ─────────────────────────────────────────────────────────────────────────────
template<typename... Args>
Buffer make_buffer(Args&&... args) {
    return Buffer(std::forward<Args>(args)...); /* TODO: verify this is sufficient */
}
