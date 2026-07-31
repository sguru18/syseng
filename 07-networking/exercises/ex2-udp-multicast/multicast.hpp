#pragma once

/* TODO: add your #include lines */
// Hint: <sys/socket.h>, <netinet/in.h>, <arpa/inet.h>, <net/if.h>,
//       <unistd.h>, <fcntl.h>, <cstring>, <map>, <cstdint>

// Wire format for a single quote update (24 bytes, packed).
struct __attribute__((packed)) QuoteMsg {
    uint32_t symbol_id;  // symbol index (0=AAPL, 1=MSFT, ...)
    int32_t  price;      // last price in ticks  (price * 100 = cents)
    int32_t  bid_qty;    // bid quantity
    int32_t  ask_qty;    // ask quantity
    uint64_t seq_no;     // sequence number for gap detection
};  // total: 4+4+4+4+8 = 24 bytes

static const char*     MULTICAST_GROUP = "239.1.1.1";
static const uint16_t  MULTICAST_PORT  = 9901;

// ── Sender ────────────────────────────────────────────────────────────────────

// Publishes QuoteMsg packets to the multicast group.
class MulticastSender {
public:
    MulticastSender() : fd_(-1) {}
    ~MulticastSender() { if (fd_ >= 0) close(fd_); }

    // Create a UDP socket and configure it for multicast send.
    // Set IP_MULTICAST_TTL=1 and IP_MULTICAST_LOOP=1.
    bool init() {
        // TODO: fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        // TODO: int ttl = 1; setsockopt(fd_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
        // TODO: int loop = 1; setsockopt(fd_, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
        // TODO: memset(&dest_, 0, sizeof(dest_));
        //        dest_.sin_family = AF_INET;
        //        dest_.sin_port   = htons(MULTICAST_PORT);
        //        inet_pton(AF_INET, MULTICAST_GROUP, &dest_.sin_addr);
        return false;
    }

    // Send one QuoteMsg to the multicast group.
    bool send(const QuoteMsg& msg) {
        // TODO: sendto(fd_, &msg, sizeof(msg), 0, (sockaddr*)&dest_, sizeof(dest_))
        (void)msg;
        return false;
    }

private:
    int             fd_;
    struct sockaddr_in dest_{};  // destination multicast address
};

// ── Receiver ──────────────────────────────────────────────────────────────────

// Joins the multicast group and receives QuoteMsg packets.
class MulticastReceiver {
public:
    MulticastReceiver() : fd_(-1), packets_received_(0), gaps_detected_(0) {}
    ~MulticastReceiver() { if (fd_ >= 0) close(fd_); }

    // Create a UDP socket, bind to INADDR_ANY:MULTICAST_PORT,
    // join the multicast group, and set O_NONBLOCK.
    bool init() {
        // TODO: fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        // TODO: int yes = 1;
        //        setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        //        setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
        // TODO: struct sockaddr_in bind_addr{};
        //        bind_addr.sin_family = AF_INET; bind_addr.sin_port = htons(MULTICAST_PORT);
        //        bind_addr.sin_addr.s_addr = INADDR_ANY;
        //        bind(fd_, (sockaddr*)&bind_addr, sizeof(bind_addr));
        // TODO: struct ip_mreq mreq{};
        //        inet_pton(AF_INET, MULTICAST_GROUP, &mreq.imr_multiaddr);
        //        mreq.imr_interface.s_addr = INADDR_ANY;
        //        setsockopt(fd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
        // TODO: fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL, 0) | O_NONBLOCK)
        return false;
    }

    // Non-blocking receive. Returns true if a packet was received.
    // Updates gap detection state: if seq_no skips, increment gaps_detected_.
    bool recv_one(QuoteMsg& out) {
        // TODO: ssize_t n = recv(fd_, &out, sizeof(out), 0);
        // TODO: if (n != sizeof(out)) return false;
        // TODO: packets_received_++
        // TODO: gap detection:
        //   auto it = last_seq_.find(out.symbol_id);
        //   if (it != last_seq_.end() && out.seq_no != it->second + 1)
        //       gaps_detected_++;
        //   last_seq_[out.symbol_id] = out.seq_no;
        (void)out;
        return false;
    }

    uint64_t packets_received() const { return packets_received_; }
    uint64_t gaps_detected()    const { return gaps_detected_; }

private:
    int      fd_;
    uint64_t packets_received_;
    uint64_t gaps_detected_;
    // TODO: std::map<uint32_t, uint64_t> last_seq_;
};

// ── PriceTable ────────────────────────────────────────────────────────────────

// Maintains the latest bid/ask for each symbol.
class PriceTable {
public:
    struct Quote { int32_t bid, ask, bid_qty, ask_qty; };

    // Update the price table from a received QuoteMsg.
    // Use msg.price as both bid and ask (single-price wire format).
    void update(const QuoteMsg& msg) {
        // TODO: table_[msg.symbol_id] = { msg.price, msg.price, msg.bid_qty, msg.ask_qty };
        (void)msg;
    }

    // Return the current quote for symbol_id.
    // Returns {0,0,0,0} if the symbol has not been seen.
    Quote get(uint32_t symbol_id) const {
        // TODO: auto it = table_.find(symbol_id);
        //        if (it == table_.end()) return {0, 0, 0, 0};
        //        return it->second;
        (void)symbol_id;
        return {0, 0, 0, 0};
    }

    size_t symbol_count() const {
        // TODO: return table_.size();
        return 0;
    }

private:
    // TODO: std::map<uint32_t, Quote> table_;
};
