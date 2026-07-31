# Exercise 3 — FIX Message Framer

FIX protocol is used for order entry at virtually every broker and exchange. Despite being text-based, parsing it correctly at TCP speeds requires handling partial messages (TCP fragmentation), SOH delimiters, and checksum validation. Real trading systems parse millions of FIX messages per second.

## Message format

```
8=FIX.4.2 SOH  9=<bodylen> SOH  <body fields>  10=<checksum> SOH
```

- Each field: `<tag>=<value>` followed by `SOH` (byte `0x01`)
- `BodyLength` (tag 9) = byte count from tag 35 to delimiter of last field before tag 10
- `CheckSum` (tag 10) = sum of all preceding bytes % 256, formatted as 3-digit decimal

## Your task

Implement in `fix_parser.hpp`:

1. `fix_checksum()` — sum all bytes in the view mod 256.
2. `parse_fields()` — split by SOH, parse `tag=value` pairs, validate checksum via tag 10; return empty on failure.
3. `FixParser::feed()` — append bytes to internal buffer; use tag 9 body length to detect complete messages and fire the callback.

## Build & test

```sh
make test
```
