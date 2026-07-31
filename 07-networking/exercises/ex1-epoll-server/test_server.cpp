#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include "server.hpp"
#include "common.hpp"

static constexpr uint16_t PORT = 19876;

// ── helpers ───────────────────────────────────────────────────────────────────

static int connect_client() {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return -1;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(PORT);
    ::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        ::close(fd); return -1;
    }
    return fd;
}

static bool send_all(int fd, const void* buf, size_t len) {
    const char* p = static_cast<const char*>(buf);
    size_t sent = 0;
    while (sent < len) {
        ssize_t n = ::send(fd, p + sent, len - sent, 0);
        if (n <= 0) return false;
        sent += n;
    }
    return true;
}

static bool recv_exact(int fd, void* buf, size_t len) {
    char* p = static_cast<char*>(buf);
    size_t got = 0;
    while (got < len) {
        ssize_t n = ::recv(fd, p + got, len - got, 0);
        if (n <= 0) return false;
        got += n;
    }
    return true;
}

// ── test functions ────────────────────────────────────────────────────────────

static void test_single_client_echo() {
    int fd = connect_client();
    ASSERT_TRUE(fd >= 0);

    const char msg[] = "hello\n";
    ASSERT_TRUE(send_all(fd, msg, sizeof(msg) - 1));

    char buf[8] = {};
    ASSERT_TRUE(recv_exact(fd, buf, sizeof(msg) - 1));
    ASSERT_STR_EQ("hello\n", std::string(buf, sizeof(msg) - 1));

    ::close(fd);
}

static void test_five_clients_echo() {
    static const char* msgs[] = {"alpha\n", "bravo\n", "charlie\n", "delta\n", "echo\n"};
    int fds[5];

    for (int i = 0; i < 5; i++) {
        fds[i] = connect_client();
        ASSERT_TRUE(fds[i] >= 0);
    }
    for (int i = 0; i < 5; i++) {
        ASSERT_TRUE(send_all(fds[i], msgs[i], strlen(msgs[i])));
    }
    for (int i = 0; i < 5; i++) {
        char buf[16] = {};
        size_t len = strlen(msgs[i]);
        ASSERT_TRUE(recv_exact(fds[i], buf, len));
        ASSERT_STR_EQ(std::string(msgs[i], len), std::string(buf, len));
        ::close(fds[i]);
    }
}

static void test_stress_20_clients() {
    static constexpr int N_CLIENTS = 20;
    static constexpr int N_MSGS    = 100;

    int fds[N_CLIENTS];
    for (int i = 0; i < N_CLIENTS; i++) {
        fds[i] = connect_client();
        ASSERT_TRUE(fds[i] >= 0);
    }

    for (int i = 0; i < N_CLIENTS; i++) {
        for (int m = 0; m < N_MSGS; m++) {
            char msg[16];
            int len = snprintf(msg, sizeof(msg), "c%dm%d\n", i, m);
            ASSERT_TRUE(send_all(fds[i], msg, len));
        }
    }

    for (int i = 0; i < N_CLIENTS; i++) {
        for (int m = 0; m < N_MSGS; m++) {
            char expected[16], got[16] = {};
            int len = snprintf(expected, sizeof(expected), "c%dm%d\n", i, m);
            ASSERT_TRUE(recv_exact(fds[i], got, len));
            ASSERT_STR_EQ(std::string(expected, len), std::string(got, len));
        }
        ::close(fds[i]);
    }
}

static void test_echo_rtt() {
    int fd = connect_client();
    ASSERT_TRUE(fd >= 0);

    const char msg[] = "ping\n";
    char buf[8] = {};

    for (int i = 0; i < 5; i++) {  // warm-up
        send_all(fd, msg, 5);
        recv_exact(fd, buf, 5);
    }

    auto t = bench::now();
    for (int i = 0; i < 100; i++) {
        send_all(fd, msg, 5);
        recv_exact(fd, buf, 5);
    }
    double avg_ns = bench::elapsed_ns(t) / 100.0;
    bench::print_result("echo RTT (loopback, 100 pings)", avg_ns);

    ASSERT_TRUE(avg_ns < 1'000'000);  // < 1 ms on loopback

    ::close(fd);
}

static EchoServer* g_server = nullptr;

static void test_server_stats() {
    // 1 + 5 + 20 + 1 connections opened across the four tests above
    ASSERT_TRUE(g_server->connections_accepted() >= 27);
    ASSERT_TRUE(g_server->bytes_echoed() > 0);
}

// ── main ──────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    EchoServer server(PORT);
    g_server = &server;
    std::thread srv_thread([&server]() { server.run(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    RUN_TEST(test_single_client_echo);
    RUN_TEST(test_five_clients_echo);
    RUN_TEST(test_stress_20_clients);
    RUN_TEST(test_echo_rtt);

    server.stop();
    srv_thread.join();

    RUN_TEST(test_server_stats);

    TEST_END();
}
