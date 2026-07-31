/* TODO: add your #include lines */
#pragma once

// ─────────────────────────────────────────────────────────────────────────────
// Part 1: Strong typedefs -- prevent Price and Quantity from mixing implicitly
//
// Price wraps int64_t (price in ticks). Quantity wraps int64_t (share count).
//
// Rules:
//   - Explicit constructor only -- no implicit conversion from int64_t.
//   - Price + Price = Price, Price - Price = Price, Price comparisons.
//   - Quantity + Quantity = Quantity, Quantity * int64_t = Quantity (scaling).
//   - value() returns the raw int64_t.
//   - Price + Quantity must NOT compile (just don't define that operator).
// ─────────────────────────────────────────────────────────────────────────────
struct Price {
    explicit Price(int64_t v = 0) : v_(0) { /* TODO: store v */ }

    int64_t value() const { return 0; /* TODO: return v_ */ }

    Price operator+(Price other) const { return Price{}; /* TODO */ }
    Price operator-(Price other) const { return Price{}; /* TODO */ }

    bool operator==(Price other) const { return false; /* TODO */ }
    bool operator!=(Price other) const { return false; /* TODO */ }
    bool operator< (Price other) const { return false; /* TODO */ }
    bool operator> (Price other) const { return false; /* TODO */ }
    bool operator<=(Price other) const { return false; /* TODO */ }
    bool operator>=(Price other) const { return false; /* TODO */ }

private:
    int64_t v_;
};

struct Quantity {
    explicit Quantity(int64_t v = 0) : v_(0) { /* TODO: store v */ }

    int64_t value() const { return 0; /* TODO: return v_ */ }

    Quantity operator+(Quantity other)  const { return Quantity{}; /* TODO */ }
    Quantity operator-(Quantity other)  const { return Quantity{}; /* TODO */ }
    Quantity operator*(int64_t scale)   const { return Quantity{}; /* TODO */ }

    bool operator==(Quantity other) const { return false; /* TODO */ }
    bool operator!=(Quantity other) const { return false; /* TODO */ }
    bool operator< (Quantity other) const { return false; /* TODO */ }
    bool operator> (Quantity other) const { return false; /* TODO */ }
    bool operator<=(Quantity other) const { return false; /* TODO */ }
    bool operator>=(Quantity other) const { return false; /* TODO */ }

private:
    int64_t v_;
};

// Compile-time proof that the types are distinct:
//   Price p(100); Quantity q(10);
//   auto x = p + q;   // ERROR: no matching operator -- this is intentional

// ─────────────────────────────────────────────────────────────────────────────
// Part 2: Side enum class
//
// - opposite(): Buy -> Sell, Sell -> Buy.
// - side_str(): human-readable "Buy" / "Sell".
// ─────────────────────────────────────────────────────────────────────────────
enum class Side { Buy, Sell };

inline Side opposite(Side s) {
    return s; /* TODO: return the other side */
}

inline const char* side_str(Side s) {
    return ""; /* TODO: return "Buy" or "Sell" */
}

// ─────────────────────────────────────────────────────────────────────────────
// Part 3: Order -- a trading message using your strong types
// ─────────────────────────────────────────────────────────────────────────────
struct Order {
    uint64_t id;
    Price    price;
    Quantity qty;
    Side     side;

    // TODO: initialize all members from the arguments
    Order(uint64_t id_, Price p_, Quantity q_, Side s_)
        : id(0), price{}, qty{}, side(Side::Buy) {}
};

// ─────────────────────────────────────────────────────────────────────────────
// Part 4: Compile-time symbol table
//
// - SYMBOL_TABLE is a constexpr array of {name, id} pairs.
// - symbol_id(name) does a linear search and returns the id, or -1 if unknown.
// - Must be constexpr so the lookup can happen at compile time.
// ─────────────────────────────────────────────────────────────────────────────
struct Symbol { const char* name; int id; };

constexpr Symbol SYMBOL_TABLE[] = {
    {"AAPL", 0}, {"MSFT", 1}, {"GOOG", 2}, {"AMZN", 3},
    {"META", 4}, {"NVDA", 5}, {"TSLA", 6}, {"JPM",  7},
};
constexpr int NUM_SYMBOLS = static_cast<int>(sizeof(SYMBOL_TABLE) / sizeof(SYMBOL_TABLE[0]));

// TODO: linear search -- compare name against each SYMBOL_TABLE entry character
// by character (no strcmp in constexpr before C++20; write your own loop).
constexpr int symbol_id(const char* name) {
    return -1; /* TODO */
}
