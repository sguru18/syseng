#pragma once

/* TODO: add your #include lines */
// Hint: <string>, <string_view>, <vector>, <unordered_map>,
//       <functional>, <cstdint>, <cstdlib>

// FIX message structure:
//   8=FIX.4.2 SOH  9=<bodylen> SOH  <fields>  10=<checksum> SOH
//
// Each field:  <tag>=<value> SOH
// SOH = '\x01'
// BodyLength (tag 9) = byte count from start of field 35 to delimiter of last
//                      field before tag 10.
// CheckSum   (tag 10) = sum of all bytes up to (not including) "10=...SOH" % 256,
//                       formatted as 3-digit decimal.

static constexpr char SOH = '\x01';

// A parsed FIX field.
struct FixField {
    int         tag;
    std::string value;
};

// A complete parsed FIX message (all fields including 8=, 9=, 10=).
using FixMessage = std::vector<FixField>;

// ── FixParser ─────────────────────────────────────────────────────────────────

// Streaming FIX parser. Buffers incoming bytes and fires a callback for each
// complete message.  Handles TCP fragmentation (partial messages across calls).
class FixParser {
public:
    // Callback receives the raw bytes of one complete FIX message.
    using MessageCallback = std::function<void(std::string_view raw_msg)>;

    explicit FixParser(MessageCallback cb)
        : cb_(std::move(cb)), msgs_(0), bytes_(0) {}

    // Feed raw bytes from the TCP stream.
    // Algorithm:
    //   1. Append data[0..len) to buffer_.
    //   2. Loop: find "9=<N>" in buffer_ to get body length.
    //   3. total = (end of "9=N\x01") + N + 7  (7 = len("10=XXX\x01"))
    //   4. If buffer_.size() >= total: call cb_, advance buffer_, repeat.
    void feed(const char* data, size_t len) {
        // TODO: buffer_.append(data, len);
        // TODO: while (true) {
        //           auto pos9 = buffer_.find("\x019=");
        //           if (pos9 == std::string::npos) break;
        //           size_t eq9  = pos9 + 3;           // skip "\x019="
        //           size_t soh9 = buffer_.find('\x01', eq9);
        //           if (soh9 == std::string::npos) break;
        //           int body_len = std::stoi(buffer_.substr(eq9, soh9 - eq9));
        //           size_t total = soh9 + 1 + body_len + 7;
        //           if (buffer_.size() < total) break;
        //           cb_(std::string_view(buffer_.data(), total));
        //           msgs_++; bytes_ += total;
        //           buffer_.erase(0, total);
        //       }
        (void)data; (void)len;
    }

    // Discard buffered state after an error.
    void reset() {
        // TODO: buffer_.clear();
    }

    uint64_t messages_parsed() const { return msgs_; }
    uint64_t bytes_consumed()  const { return bytes_; }

private:
    MessageCallback cb_;
    std::string     buffer_;  // partial message accumulator
    uint64_t        msgs_;
    uint64_t        bytes_;
};

// ── Free functions ────────────────────────────────────────────────────────────

// Compute FIX checksum: sum of all bytes in msg % 256.
inline uint8_t fix_checksum(std::string_view msg) {
    // TODO: uint32_t sum = 0; for (unsigned char c : msg) sum += c; return sum % 256;
    (void)msg;
    return 0;
}

// Parse all tag=value fields from a raw FIX message string.
// Validates that tag 10 checksum matches the computed checksum.
// Returns empty vector if checksum fails or message is malformed.
inline FixMessage parse_fields(std::string_view raw_msg) {
    // TODO:
    //   FixMessage fields;
    //   Split raw_msg by SOH to get individual "tag=value" tokens.
    //   For each token: find '=', parse tag (int) and value (string).
    //   Append to fields.
    //   Validate: find tag 10, parse its value as int, compare against
    //     fix_checksum(raw_msg.substr(0, raw_msg.rfind("10=")))
    //   Return fields on success, {} on failure.
    (void)raw_msg;
    return {};
}

// Build a raw FIX message string from tag/value pairs.
// Automatically computes and appends the checksum (tag 10).
// PROVIDED FOR YOU in test_fix_parser.cpp.
std::string build_fix_message(const std::vector<std::pair<int, std::string>>& fields);
