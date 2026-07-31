# Exercise 2 — Type-Safe Trading Primitives

A mismatched price and quantity is a billion-dollar bug. Strong types catch these at compile time, not at 3am during a post-mortem. Every modern trading system wraps raw integers in explicit typedef structs to prevent silent unit confusion.

## Parts

1. **Price / Quantity** — Strong typedefs over `int64_t`. Prevent implicit mixing. Arithmetic within each type works; mixing types doesn't compile.
2. **Side** — `enum class` with helpers. Scoped enums prevent the classic `if (side == 0)` footgun.
3. **Order** — Aggregate using your strong types. The vocabulary of a trading message.
4. **Symbol table** — `constexpr` linear search: compile-time symbol-to-id mapping, zero overhead at runtime.

## By the end of this exercise...

You'll have a compile-time firewall against the most common class of trading bugs. You'll be able to answer: _"How do you prevent a price from being passed where a quantity is expected?"_ -- and show real C++ code that enforces it.

Answer: Use strongly typed wrappers, ie. structs. always use explicit for the constructors, and define new operators.

## Build & Test

```
make test
```
