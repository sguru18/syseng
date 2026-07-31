# Module 07 — Networking

HFT systems receive market data over UDP multicast and send orders over TCP. The difference between a blocking and non-blocking network stack can be microseconds per message. These exercises build real network components you can run and test.

## Exercises

| # | Directory | What you build |
|---|-----------|----------------|
| 1 | `ex1-epoll-server` | Non-blocking TCP echo server (kqueue on macOS, epoll on Linux) |
| 2 | `ex2-udp-multicast` | UDP multicast market data feed — sender and receiver |
| 3 | `ex3-fix-framer` | Streaming FIX protocol parser with checksum validation |

## Platform note

These exercises target **macOS ARM64**. Exercise 1 uses `kqueue` / `kevent` (BSD/macOS). On Linux, replace with `epoll_create1` / `epoll_ctl` / `epoll_wait` — the event-loop pattern is identical.

## Build

```sh
make          # build all exercises
make test     # build and run all tests
make clean
```
