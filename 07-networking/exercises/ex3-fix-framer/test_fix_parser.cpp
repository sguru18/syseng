#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <functional>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "fix_parser.hpp"
#include "common.hpp"

// ── build_fix_message (provided glue) ────────────────────────────────────────
// Constructs a valid FIX message from tag/value pairs.
// Skips tags 8, 9, 10 (those are auto-generated).
// Computes and appends the correct checksum.

std::string build_fix_message(const std::vector<std::pair<int, std::string>>& fields) {
    std::string body;
    for (auto& [tag, val] : fields) {
        if (tag == 8 || tag == 9 || tag == 10) continue;
        body += std::to_string(tag) + "=" + val + SOH;
    }
    std::string msg =
        "8=FIX.4.2" + std::string(1, SOH) +
        "9=" + std::to_string(body.size()) + SOH +
        body;
    uint8_t cksum = fix_checksum(msg);
    char buf[8];
    snprintf(buf, sizeof(buf), "%03d", static_cast<int>(cksum));
    msg += "10=" + std::string(buf) + SOH;
    return msg;
}

// ── helper ────────────────────────────────────────────────────────────────────

static std::string find_field(const FixMessage& msg, int tag) {
    for (auto& f : msg)
        if (f.tag == tag) return f.value;
    return "";
}

// ── tests ─────────────────────────────────────────────────────────────────────

static void test_fix_checksum_known() {
    // "8=FIX.4.2\x01" "35=D\x01"
    // Bytes: 56 61 70 73 88 46 52 46 50 1  51 53 61 68 1 = sum 777, 777%256 = 9
    std::string s = "8=FIX.4.2\x01" "35=D\x01";
    ASSERT_EQ(9, static_cast<int>(fix_checksum(s)));

    // Empty string
    ASSERT_EQ(0, static_cast<int>(fix_checksum("")));
}

static void test_parse_fields_valid() {
    auto raw = build_fix_message({
        {35, "D"},
        {49, "SENDER"},
        {56, "TARGET"},
        {11, "ORDER1"},
        {55, "AAPL"},
        {54, "1"},
        {38, "100"},
        {44, "150.00"},
        {40, "2"}
    });

    auto fields = parse_fields(raw);
    ASSERT_FALSE(fields.empty());
    ASSERT_STR_EQ("FIX.4.2", find_field(fields, 8));
    ASSERT_STR_EQ("D",       find_field(fields, 35));
    ASSERT_STR_EQ("SENDER",  find_field(fields, 49));
    ASSERT_STR_EQ("TARGET",  find_field(fields, 56));
    ASSERT_STR_EQ("AAPL",    find_field(fields, 55));
    ASSERT_STR_EQ("100",     find_field(fields, 38));
    ASSERT_STR_EQ("150.00",  find_field(fields, 44));
    ASSERT_STR_EQ("1",       find_field(fields, 54));
    ASSERT_STR_EQ("2",       find_field(fields, 40));
}

static void test_parse_fields_bad_checksum() {
    // Body "35=D\x01" = 5 bytes.  Correct checksum is 181, not 000.
    std::string raw = "8=FIX.4.2\x01" "9=5\x01" "35=D\x01" "10=000\x01";
    auto fields = parse_fields(raw);
    ASSERT_TRUE(fields.empty());
}

static void test_streaming_complete_message() {
    int count = 0;
    std::string captured;
    FixParser parser([&](std::string_view raw) {
        count++;
        captured = std::string(raw);
    });

    auto msg = build_fix_message({{35, "D"}, {55, "AAPL"}});
    parser.feed(msg.data(), msg.size());

    ASSERT_EQ(1, count);
    ASSERT_EQ(1u, parser.messages_parsed());
    ASSERT_EQ(msg.size(), static_cast<size_t>(parser.bytes_consumed()));
    ASSERT_STR_EQ(msg, captured);
}

static void test_streaming_split_3_pieces() {
    int count = 0;
    FixParser parser([&count](std::string_view) { count++; });

    auto msg = build_fix_message({{35, "D"}, {55, "MSFT"}, {38, "500"}});
    size_t third = msg.size() / 3;

    parser.feed(msg.data(), third);
    ASSERT_EQ(0, count);  // incomplete

    parser.feed(msg.data() + third, third);
    ASSERT_EQ(0, count);  // still incomplete

    parser.feed(msg.data() + 2 * third, msg.size() - 2 * third);
    ASSERT_EQ(1, count);  // complete
}

static void test_streaming_two_back_to_back() {
    int count = 0;
    FixParser parser([&count](std::string_view) { count++; });

    auto m1 = build_fix_message({{35, "D"}, {55, "AAPL"}});
    auto m2 = build_fix_message({{35, "8"}, {55, "GOOG"}});
    std::string both = m1 + m2;

    parser.feed(both.data(), both.size());
    ASSERT_EQ(2, count);
}

static void test_streaming_half_and_half() {
    int count = 0;
    FixParser parser([&count](std::string_view) { count++; });

    auto msg = build_fix_message({{35, "D"}, {55, "TSLA"}, {38, "1000"}, {44, "250.00"}});
    size_t half = msg.size() / 2;

    parser.feed(msg.data(), half);
    ASSERT_EQ(0, count);

    parser.feed(msg.data() + half, msg.size() - half);
    ASSERT_EQ(1, count);
}

static void test_streaming_10_messages() {
    int count = 0;
    FixParser parser([&count](std::string_view) { count++; });

    for (int i = 0; i < 10; i++) {
        auto msg = build_fix_message({{35, "D"}, {11, std::to_string(i)}, {55, "AAPL"}});
        parser.feed(msg.data(), msg.size());
    }

    ASSERT_EQ(10, count);
    ASSERT_EQ(10u, parser.messages_parsed());
}

static void test_streaming_reset() {
    int count = 0;
    FixParser parser([&count](std::string_view) { count++; });

    auto msg = build_fix_message({{35, "D"}, {55, "AAPL"}});
    size_t half = msg.size() / 2;

    parser.feed(msg.data(), half);  // partial message buffered
    ASSERT_EQ(0, count);

    parser.reset();                 // discard partial

    parser.feed(msg.data(), msg.size());  // feed complete message again
    ASSERT_EQ(1, count);
}

static void test_benchmark_100k() {
    int count = 0;
    FixParser parser([&count](std::string_view) { count++; });

    auto msg = build_fix_message({
        {35, "D"}, {49, "SENDER"}, {56, "TARGET"},
        {11, "ORDER1"}, {55, "AAPL"}, {54, "1"}, {38, "100"}, {44, "150.00"}
    });

    static constexpr int N = 100000;
    auto t = bench::now();
    for (int i = 0; i < N; i++) {
        parser.feed(msg.data(), msg.size());
    }
    double total_ns = static_cast<double>(bench::elapsed_ns(t));

    ASSERT_EQ(N, count);
    bench::print_throughput("FIX parse throughput (100k msgs)", N, total_ns);
    printf("  msg size: %zu bytes\n", msg.size());
}

// ── main ──────────────────────────────────────────────────────────────────────

int main() {
    TEST_BEGIN();

    RUN_TEST(test_fix_checksum_known);
    RUN_TEST(test_parse_fields_valid);
    RUN_TEST(test_parse_fields_bad_checksum);
    RUN_TEST(test_streaming_complete_message);
    RUN_TEST(test_streaming_split_3_pieces);
    RUN_TEST(test_streaming_two_back_to_back);
    RUN_TEST(test_streaming_half_and_half);
    RUN_TEST(test_streaming_10_messages);
    RUN_TEST(test_streaming_reset);
    RUN_TEST(test_benchmark_100k);

    TEST_END();
}
