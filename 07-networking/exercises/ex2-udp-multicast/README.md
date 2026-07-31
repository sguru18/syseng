# Exercise 2 — UDP Multicast Market Data

Real equity market data (NYSE OpenBook, NASDAQ TotalView, CME MDP3) is delivered over UDP multicast. There's no TCP handshake, no retransmit — if you miss a packet, you request a recovery snapshot. This exercise builds both sides of a simple multicast feed.

## Your task

Implement in `multicast.hpp`:

1. `MulticastSender::init()` — create a UDP socket, set `IP_MULTICAST_TTL=1`, `IP_MULTICAST_LOOP=1` (so local receivers get packets), store the destination address.
2. `MulticastSender::send()` — `sendto` the packet to the multicast group.
3. `MulticastReceiver::init()` — create a UDP socket, `SO_REUSEADDR`+`SO_REUSEPORT`, bind to `INADDR_ANY:MULTICAST_PORT`, join the group via `IP_ADD_MEMBERSHIP`, set `O_NONBLOCK`.
4. `MulticastReceiver::recv_one()` — non-blocking `recvfrom`; on success, check `seq_no` for gaps and update `last_seq_`.
5. `PriceTable::update()` / `get()` — maintain the latest quote for each `symbol_id`.

## Build & test

```sh
make test
```
