# Exercise 1 — Non-Blocking TCP Server (kqueue / epoll)

Every HFT network layer is event-driven — a single thread handles thousands of connections without blocking. On Linux this is `epoll`; on macOS it's `kqueue`. The pattern is identical: register interest, wait for events, process them. This is also how nginx, Node.js, and Redis work.

## Your task

Implement `EchoServer` in `server.hpp`:

1. `run()` — create a kqueue, create/bind/listen the socket (non-blocking), register it, then loop calling `run_once()`.
2. `run_once()` — call `kevent()` to wait; for each event call `accept_client()` or `echo_client()`.
3. `accept_client()` — `accept()`, set non-blocking, register the new fd with kqueue.
4. `echo_client()` — read bytes and write them back; close on EOF or error.

## kqueue vs epoll cheat-sheet

| | macOS (kqueue) | Linux (epoll) |
|---|---|---|
| Create | `kqueue()` | `epoll_create1(0)` |
| Register | `kevent(kq, &chg, 1, NULL, 0, NULL)` | `epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)` |
| Wait | `kevent(kq, NULL, 0, evs, N, &ts)` | `epoll_wait(epfd, evs, N, timeout_ms)` |
| Event filter | `EVFILT_READ` | `EPOLLIN` |

## Build & test

```sh
make test
```
