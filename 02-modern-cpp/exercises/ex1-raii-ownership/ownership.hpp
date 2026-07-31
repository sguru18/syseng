#include <unistd.h>   // close()
#include <fcntl.h>    // open(), O_RDONLY, O_WRONLY, O_CREAT etc.
#pragma once

// Notes

// a posix file descriptor is a slot number that tells you where a file or socket is
// you need open and close on that file descriptor to use the resource
// this file wraps that functionality in an RAII class. so there is no manual cleanup

// the scope guard at the end handles automatic cleanup of variables when they go out of scope
// regardless of execution result. .dismiss means you can cancel the cleanup/reversal if everything
// happened correctly. so we can make a scopeguard, try a database transaction for example, and if 
// anything errors in the middle, the scopeguard automatically runs to clean up. else we dismiss on
// a successful transaction. this can be true of database connection sockets or std::unique_lock too

// this is why garbage collected languages take a major hit for efficiency, bc memory clean up
// is not predictable. 

// the point of this exercise is to understand that you need to encode anything
// that must happen on exit directly in the type system. no with, finally, etc like python or any
// manual cleanups


// ─────────────────────────────────────────────────────────────────────────────
// Part 1: ScopedFd -- RAII wrapper for a POSIX file descriptor
//
// - Constructor takes an fd (int). Destructor calls close(fd_) if valid.
// - Non-copyable: delete copy ctor and copy assign.
// - Movable: move ctor transfers ownership and sets source fd_ to -1.
// - get() returns the raw fd. valid() returns fd_ != -1.
// - release() relinquishes ownership without closing; returns the fd.
// ─────────────────────────────────────────────────────────────────────────────
class ScopedFd {
public:
    explicit ScopedFd(int fd) : fd_(fd) {}
    ~ScopedFd() {  
        if (valid()) { 
            int fd = release(); 
            close(fd);
        }
    }

    ScopedFd(const ScopedFd&)            = delete;
    ScopedFd& operator=(const ScopedFd&) = delete;

    // move constructor
    ScopedFd(ScopedFd&& other) noexcept : fd_(-1) { 
        this -> fd_ = other.fd_;
        other.fd_ = -1;
    }
    
    // move assignment operator, has the extra responsibility of cleaning up the current fd before stealing the other
    ScopedFd& operator=(ScopedFd&& other) noexcept { 
        if (valid()) { 
            close(this->fd_); 
        }
        this -> fd_ = other.fd_;
        other.fd_ = -1;
        return *this; 
    }

    int  get()   const { return fd_; }
    bool valid() const { return fd_ != -1; }
    int  release()     { 
        // DOES NOT close the fd here, just hands ownership off to the caller and marks this wrapper as empty
        // useful for handing off to a library function or something
        if (valid()) {
            // close(this->fd_);
            int ret = this -> fd_;
            this -> fd_ = -1;
            return ret;
        }
        return -1;  
    }

private:
    int fd_ = -1;
};

// ─────────────────────────────────────────────────────────────────────────────
// Part 2: UniqueBuffer -- move-only heap buffer (simplified unique_ptr<T[]>)
//
// - Constructor allocates n elements with new[].
// - Destructor: delete[].
// - Non-copyable, movable. Move transfers data_ and size_, nulls the source.
// - data() returns raw pointer. size() returns element count.
// - operator[] provides element access.
// ─────────────────────────────────────────────────────────────────────────────
template<typename T>
class UniqueBuffer {
public:
    explicit UniqueBuffer(size_t n) : data_(nullptr), size_(0) { 
        this -> size_ = n;
        this -> data_ = new T[n];
    }
    ~UniqueBuffer() { 
        delete[] this -> data_;
    }

    UniqueBuffer(const UniqueBuffer&)            = delete;
    UniqueBuffer& operator=(const UniqueBuffer&) = delete;

    UniqueBuffer(UniqueBuffer&& other) noexcept : data_(nullptr), size_(0) { 
        // steal, null source
        this -> size_ = other.size_;
        this -> data_ = other.data_;

        other.size_ = 0;
        other.data_ = nullptr;
    }

    UniqueBuffer& operator=(UniqueBuffer&& other) noexcept { 
        // free, steal, null source
        delete[] this -> data_;

        this -> size_ = other.size_;
        this -> data_ = other.data_;

        other.size_ = 0;
        other.data_ = nullptr;

        return *this; 
    }

    T*       data()              { return data_; }
    const T* data()        const { return data_; }
    size_t   size()        const { return size_; }
    T&       operator[](size_t i){ return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

private:
    T*     data_;
    size_t size_;
};

// ─────────────────────────────────────────────────────────────────────────────
// Part 3: ScopeGuard -- run a callable when the guard goes out of scope
//
// Usage:  auto g = make_scope_guard([&]{ cleanup(); });
//
// - Non-copyable, non-movable.
// - Destructor calls fn_() if active_.
// - dismiss() sets active_ = false so the cleanup is skipped.
// ─────────────────────────────────────────────────────────────────────────────
template<typename F>
class ScopeGuard {
public:
    // std::forward<F> is perfect forwarding, essentially keeps fn as a lambda, function pointer,
    // or whatever else it was when it was passed in without copying
    explicit ScopeGuard(F&& fn) : fn_(std::forward<F>(fn)), active_(true) {}
    ~ScopeGuard() { if (this -> active_) fn_(); }

    ScopeGuard(const ScopeGuard&)            = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard(ScopeGuard&&)                 = delete;
    ScopeGuard& operator=(ScopeGuard&&)      = delete;

    void dismiss() { this -> active_ = false; }

private:
    F    fn_;
    bool active_;
};

// Factory: C++17 mandatory copy elision means this works even though ScopeGuard
// is non-movable -- the prvalue is constructed directly in the caller.

// this is needed because you can't call the constructor above because you don't know what type F is because of the lambda
// in C++17 and newer you can just omit the type and compiler will deduce but this is explicit
// the scopeguard is actually extremely simple implementation but such a powerful idea

// cool that the scopeguard's scope and the object's scope are different, the callback runs when the guard goes out of scope
// not the object itself. so you can have a db object, set up a scopeguard to do some risky work, and then have the db outlive
// the scopeguard for more work afterward. very cool.
template<typename F>
ScopeGuard<F> make_scope_guard(F&& fn) {
    return ScopeGuard<F>(std::forward<F>(fn));
}
