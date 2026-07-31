#pragma once
/* TODO: add your #include lines */
// You will need: <cstddef>, <cstdint>, <new>

// ─────────────────────────────────────────────────────────────────────────────
// PoolAllocator<T>
//
// A fixed-pool bump allocator that satisfies the C++ Allocator concept.
// Backed by pool_detail::g_pool — a flat 4 MB array.
//
// Can be used as: std::vector<T, PoolAllocator<T>>
//
// Allocator concept requirements:
//   - value_type typedef
//   - allocate(n)  -> T*    (bump-pointer from pool; throw std::bad_alloc on OOM)
//   - deallocate(p, n)      (no-op for a bump allocator)
//   - rebind<U>::other      (required by some STL internals)
//   - operator== / !=
//
// The allocator is stateless — all instances share the same pool.
// Call pool_detail::reset() between tests to reclaim the pool.
// ─────────────────────────────────────────────────────────────────────────────

// ── Backing pool ──────────────────────────────────────────────────────────────
// A flat slab of memory with a bump pointer.
// inline variables (C++17) ensure a single definition across translation units.

namespace pool_detail {
    constexpr std::size_t POOL_SIZE = 4 * 1024 * 1024; // 4 MB
    inline std::uint8_t   g_pool[POOL_SIZE] = {};
    inline std::size_t    g_offset = 0;

    // Reset the pool (call between independent tests to avoid exhaustion).
    inline void reset() noexcept { g_offset = 0; }
}

// ── Allocator ─────────────────────────────────────────────────────────────────

template<typename T>
class PoolAllocator {
public:
    using value_type = T;

    // Rebind: allows std::list<T, PoolAllocator<T>> to allocate internal nodes.
    template<typename U>
    struct rebind { using other = PoolAllocator<U>; };

    PoolAllocator()  noexcept = default;
    template<typename U>
    PoolAllocator(const PoolAllocator<U>&) noexcept {}

    // Allocate storage for n objects of type T.
    // - Round pool_detail::g_offset UP to the next 64-byte boundary.
    // - Throw std::bad_alloc if the pool would overflow.
    // - Advance g_offset by n * sizeof(T).
    // - Return a pointer to the aligned region.
    T* allocate(std::size_t n) {
        // TODO: 64-byte aligned bump allocation from pool_detail::g_pool
        std::size_t bytes   = n * sizeof(T);
        // Alignment placeholder (TODO: replace with 64-byte alignment)
        std::size_t offset  = pool_detail::g_offset;
        if (offset + bytes > pool_detail::POOL_SIZE) throw std::bad_alloc{};
        pool_detail::g_offset = offset + bytes;
        return reinterpret_cast<T*>(pool_detail::g_pool + offset);
    }

    // Return storage to the pool.
    // For a bump allocator this is a deliberate no-op.
    void deallocate(T* p, std::size_t n) noexcept {
        // TODO: (no-op is correct for a bump allocator)
        (void)p; (void)n;
    }

    bool operator==(const PoolAllocator&) const noexcept { return true; }
    bool operator!=(const PoolAllocator&) const noexcept { return false; }
};
