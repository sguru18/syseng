#include <common.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <type_traits>
#include "ownership.hpp"

// ── ScopedFd ─────────────────────────────────────────────────────────────────

void test_scoped_fd_valid() {
    int raw = open("/tmp/test_scoped_fd.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_TRUE(raw >= 0);
    ScopedFd fd(raw);
    ASSERT_TRUE(fd.valid());
    ASSERT_EQ(raw, fd.get());
}

void test_scoped_fd_destructor_closes() {
    int raw = open("/tmp/test_scoped_fd.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_TRUE(raw >= 0);
    {
        ScopedFd fd(raw);
        ASSERT_TRUE(fd.valid());
    }
    // Destructor must have closed raw. Any syscall on a closed fd returns EBADF.
    char buf[1];
    ssize_t r = read(raw, buf, 1);
    ASSERT_EQ(-1, (int)r);
    ASSERT_EQ(EBADF, errno);
}

void test_scoped_fd_move() {
    int raw = open("/tmp/test_scoped_fd.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_TRUE(raw >= 0);
    ScopedFd fd1(raw);
    ScopedFd fd2(std::move(fd1));
    ASSERT_FALSE(fd1.valid());  // source must be invalidated
    ASSERT_TRUE(fd2.valid());
    ASSERT_EQ(raw, fd2.get());
}

void test_scoped_fd_not_copyable() {
    static_assert(!std::is_copy_constructible_v<ScopedFd>,
        "ScopedFd must not be copy constructible");
    static_assert(!std::is_copy_assignable_v<ScopedFd>,
        "ScopedFd must not be copy assignable");
}

void test_scoped_fd_release() {
    int raw = open("/tmp/test_scoped_fd.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_TRUE(raw >= 0);
    int released;
    {
        ScopedFd fd(raw);
        released = fd.release();
        ASSERT_EQ(raw, released);
        ASSERT_FALSE(fd.valid());
        // fd goes out of scope here; because we released, destructor must NOT close it
    }
    // released fd is still open -- we can use it
    ssize_t w = write(released, "x", 1);
    ASSERT_EQ(1, (int)w);
    close(released);
}

// ── UniqueBuffer ─────────────────────────────────────────────────────────────

void test_unique_buffer_basic() {
    UniqueBuffer<int> buf(10);
    ASSERT_NOT_NULL(buf.data());
    ASSERT_EQ(10, (int)buf.size());
    buf[0] = 42;
    buf[9] = 99;
    ASSERT_EQ(42, buf[0]);
    ASSERT_EQ(99, buf[9]);
}

void test_unique_buffer_move() {
    UniqueBuffer<int> src(5);
    ASSERT_NOT_NULL(src.data());
    src[0] = 7;
    int* original_ptr = src.data();

    UniqueBuffer<int> dst(std::move(src));

    ASSERT_NULL(src.data());          // source must be nulled
    ASSERT_EQ(0, (int)src.size());
    ASSERT_EQ(original_ptr, dst.data()); // same allocation, no copy
    ASSERT_EQ(7, dst[0]);
}

void test_unique_buffer_not_copyable() {
    static_assert(!std::is_copy_constructible_v<UniqueBuffer<int>>,
        "UniqueBuffer must not be copy constructible");
    static_assert(!std::is_copy_assignable_v<UniqueBuffer<int>>,
        "UniqueBuffer must not be copy assignable");
}

// ── ScopeGuard ───────────────────────────────────────────────────────────────

void test_scope_guard_runs() {
    bool ran = false;
    {
        auto g = make_scope_guard([&]{ ran = true; });
        ASSERT_FALSE(ran); // not yet
    }
    ASSERT_TRUE(ran); // must have fired on scope exit
}

void test_scope_guard_dismiss() {
    bool ran = false;
    {
        auto g = make_scope_guard([&]{ ran = true; });
        g.dismiss();
    }
    ASSERT_FALSE(ran); // dismissed -- must NOT have fired
}

void test_scope_guard_exception_path() {
    bool cleaned_up = false;
    try {
        auto g = make_scope_guard([&]{ cleaned_up = true; });
        throw std::runtime_error("simulated error");
    } catch (...) {}
    ASSERT_TRUE(cleaned_up); // guard must fire even when an exception unwinds the stack
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    std::printf(BOLD "\n── ScopedFd ──────────────────────────────────────\n" RESET);
    RUN_TEST(test_scoped_fd_valid);
    RUN_TEST(test_scoped_fd_destructor_closes);
    RUN_TEST(test_scoped_fd_move);
    RUN_TEST(test_scoped_fd_not_copyable);
    RUN_TEST(test_scoped_fd_release);

    std::printf(BOLD "\n── UniqueBuffer ──────────────────────────────────\n" RESET);
    RUN_TEST(test_unique_buffer_basic);
    RUN_TEST(test_unique_buffer_move);
    RUN_TEST(test_unique_buffer_not_copyable);

    std::printf(BOLD "\n── ScopeGuard ────────────────────────────────────\n" RESET);
    RUN_TEST(test_scope_guard_runs);
    RUN_TEST(test_scope_guard_dismiss);
    RUN_TEST(test_scope_guard_exception_path);

    TEST_END();
}
