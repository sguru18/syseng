#pragma once
/* TODO: add your #include lines */

// ─── Arena Allocator ──────────────────────────────────────────────────────────
//
// Strategy: bump-pointer through a pre-allocated backing buffer.
//
//   buffer: [ used bytes | alignment padding | next alloc | ... | free ]
//            ^                                                          ^
//            base                                               base + capacity
//
// allocate() uses std::align to find the next aligned address then advances the
// internal offset by the alignment padding plus the requested size.
// reset() rewinds offset to 0. No individual frees.

class Arena {
public:
    // Pre-allocate a backing buffer of `capacity` bytes.
    explicit Arena(size_t capacity) : buf_(nullptr), offset_(0), capacity_(capacity) {
        // TODO: allocate capacity_ bytes into buf_ (e.g. new char[capacity_])
    }

    ~Arena() {
        // TODO: free buf_
    }

    // Allocate `size` bytes aligned to `align`. Returns nullptr if out of space.
    // Hint: cast buf_ + offset_ to void*, call std::align(align, size, ptr, space),
    //       then advance offset_ by (space_before - space_after) + size.
    void* allocate(size_t size, size_t align = alignof(std::max_align_t)) {
        (void)size; (void)align;
        return nullptr; // TODO
    }

    // Construct a T in-place inside the arena via placement new.
    template<typename T, typename... Args>
    T* emplace(Args&&... args) {
        void* p = allocate(sizeof(T), alignof(T));
        if (!p) return nullptr;
        return new(p) T(std::forward<Args>(args)...);
    }

    // Rewind offset to 0. All previously returned pointers become invalid.
    void reset() {
        // TODO: set offset_ = 0
    }

    size_t used()     const { return offset_; } // TODO: return current byte offset
    size_t capacity() const { return capacity_; }

private:
    char*  buf_;
    size_t offset_;
    size_t capacity_;
};

// ─── Object Pool ──────────────────────────────────────────────────────────────
//
// Strategy: intrusive free list embedded in the object slots themselves.
//
//   slots: [ slot0 | slot1 | ... | slot_{n-1} ]
//
// Free slots store a FreeNode* in their first sizeof(void*) bytes.
// allocate(): pop head_, return it cast to T*.
// deallocate(): cast p to FreeNode*, push back onto head_.
//
// Requirement: sizeof(T) >= sizeof(void*). The static_assert below enforces it.

template<typename T>
class ObjectPool {
public:
    explicit ObjectPool(size_t n) : storage_(nullptr), head_(nullptr),
                                    capacity_(n), available_(n) {
        static_assert(sizeof(T) >= sizeof(void*),
            "ObjectPool: T must be at least pointer-sized for the intrusive free list");
        // TODO: allocate raw storage for n objects: operator new[](n * sizeof(T))
        //       or a char array of size n * sizeof(T) with alignof(T) alignment.
        // TODO: link all slots into the free list:
        //       for each slot i: cast storage+i to FreeNode*, set next to slot i+1
        //       (last node's next = nullptr). Set head_ = &slot[0].
    }

    ~ObjectPool() {
        // TODO: free the storage (operator delete[](storage_) or equivalent)
    }

    // Pop the head of the free list. Returns nullptr if the pool is exhausted.
    T* allocate() {
        // TODO: if head_ == nullptr return nullptr
        //       FreeNode* node = head_; head_ = head_->next; --available_;
        //       return reinterpret_cast<T*>(node);
        return nullptr;
    }

    // Push p back onto the free list.
    void deallocate(T* p) {
        (void)p;
        // TODO: reinterpret_cast<FreeNode*>(p)->next = head_; head_ = ...; ++available_;
    }

    size_t available() const { return available_; }
    size_t capacity()  const { return capacity_; }

private:
    struct FreeNode { FreeNode* next; };

    void*    storage_;
    FreeNode* head_;
    size_t   capacity_;
    size_t   available_;
};
