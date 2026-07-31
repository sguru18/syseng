#pragma once

/* TODO: add your #include lines */
// Hint: <sys/event.h>, <sys/socket.h>, <netinet/in.h>, <arpa/inet.h>,
//       <unistd.h>, <fcntl.h>, <atomic>, <cstring>, <cerrno>

// Non-blocking TCP echo server using kqueue (macOS).
// Single-threaded, event-driven -- one thread handles all connections.
// Linux equivalent: replace kqueue/kevent with epoll_create1/epoll_ctl/epoll_wait.

class EchoServer {
public:
    explicit EchoServer(uint16_t port)
        : port_(port), kq_(-1), listen_fd_(-1),
          stop_flag_(false), accepted_(0), bytes_echoed_(0) {}

    ~EchoServer() {
        if (listen_fd_ >= 0) close(listen_fd_);
        if (kq_ >= 0)        close(kq_);
    }

    // Blocks until stop() is called.
    void run() {
        // TODO: kq_ = kqueue();
        //
        // TODO: create listen_fd_:
        //   listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        //   setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
        //   struct sockaddr_in addr{}; addr.sin_family=AF_INET; addr.sin_port=htons(port_);
        //   addr.sin_addr.s_addr=INADDR_ANY;
        //   bind(listen_fd_, (sockaddr*)&addr, sizeof(addr));
        //   listen(listen_fd_, 128);
        //
        // TODO: set_nonblocking(listen_fd_)
        //
        // TODO: register listen_fd_ with kqueue:
        //   struct kevent chg;
        //   EV_SET(&chg, listen_fd_, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, nullptr);
        //   kevent(kq_, &chg, 1, nullptr, 0, nullptr);
        while (!stop_flag_.load()) {
            run_once();
        }
    }

    // Wait up to timeout_ms for kqueue events and dispatch each one.
    // Returns number of events processed.
    int run_once(int timeout_ms = 100) {
        // TODO:
        //   struct kevent events[64];
        //   struct timespec ts{ timeout_ms / 1000, (timeout_ms % 1000) * 1000000LL };
        //   int n = kevent(kq_, nullptr, 0, events, 64, &ts);
        //   for (int i = 0; i < n; i++) {
        //       if ((int)events[i].ident == listen_fd_) accept_client();
        //       else                                    echo_client((int)events[i].ident);
        //   }
        //   return n;
        (void)timeout_ms;
        return 0;
    }

    // Signal the server to stop.
    void stop() { stop_flag_.store(true); }

    int connections_accepted() const { return accepted_; }
    int bytes_echoed()         const { return bytes_echoed_; }

private:
    uint16_t          port_;
    int               kq_;
    int               listen_fd_;
    std::atomic<bool> stop_flag_;
    int               accepted_;
    int               bytes_echoed_;

    // Set fd to O_NONBLOCK.
    void set_nonblocking(int fd) {
        // TODO: fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK)
        (void)fd;
    }

    // Accept a new connection and register it with kqueue for reads.
    void accept_client() {
        // TODO: sockaddr_in addr{}; socklen_t len = sizeof(addr);
        // TODO: int cfd = accept(listen_fd_, (sockaddr*)&addr, &len);
        // TODO: if (cfd < 0) return;
        // TODO: set_nonblocking(cfd)
        // TODO: struct kevent chg;
        //        EV_SET(&chg, cfd, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, nullptr);
        //        kevent(kq_, &chg, 1, nullptr, 0, nullptr);
        // TODO: accepted_++
    }

    // Read from a client and echo the bytes back. Close on EOF or error.
    void echo_client(int fd) {
        // TODO: char buf[4096];
        // TODO: ssize_t n = read(fd, buf, sizeof(buf));
        // TODO: if (n <= 0) {
        //           struct kevent chg;
        //           EV_SET(&chg, fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
        //           kevent(kq_, &chg, 1, nullptr, 0, nullptr);
        //           close(fd); return;
        //       }
        // TODO: write(fd, buf, n);
        // TODO: bytes_echoed_ += n;
        (void)fd;
    }
};
