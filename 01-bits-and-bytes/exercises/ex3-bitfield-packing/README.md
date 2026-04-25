# Exercise 3: Bitfield Packing — DNS Header

## The Problem

You need to pack a DNS header (RFC 1035) into exactly 12 bytes in network byte order (big-endian), and unpack it back. This is the same operation every DNS resolver, web browser, and network stack performs billions of times a day.

## Why Not Just Use C Bitfields?

C's `: n` bitfield syntax seems perfect for this. Don't use it.

The C standard leaves bit layout **implementation-defined**: compilers choose padding, ordering, and alignment however they want. Code using struct bitfields that works on your Mac will silently produce corrupt packets on an ARM chip. Every serious network protocol parser uses manual shifts and masks instead.

## DNS Header Format (RFC 1035)

```
 0                   1
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
┌─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┐
│                ID                 │  Bytes 0–1
├─┼─┬─┬─┬─┬─┼─┼─┼─┼─┼─┬─┬─┼─┬─┬─┤
│QR│ Opcode  │AA│TC│RD│RA│  Z  │RCODE│  Bytes 2–3
├─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┤
│             QDCOUNT               │  Bytes 4–5
├─────────────────────────────────-─┤
│             ANCOUNT               │  Bytes 6–7
├───────────────────────────────────┤
│             NSCOUNT               │  Bytes 8–9
├───────────────────────────────────┤
│             ARCOUNT               │  Bytes 10–11
└───────────────────────────────────┘
```

**Byte 2 breakdown** (flags, high byte):
| Bit 7 | Bits 6–3 | Bit 2 | Bit 1 | Bit 0 |
|-------|----------|-------|-------|-------|
| QR    | Opcode   | AA    | TC    | RD    |

**Byte 3 breakdown** (flags, low byte):
| Bit 7 | Bits 6–4 | Bits 3–0 |
|-------|----------|----------|
| RA    | Z (reserved) | RCODE |

## Warm-Up

How do you extract bits 4–7 from a 16-bit value `x`?

```
Step 1: Shift right by 4 to move them to position 0–3
Step 2: Mask with 0x0F to clear everything above

result = (x >> 4) & 0x0F
```

How do you pack a 4-bit value `v` into bits 4–7 of a 16-bit field?

```
result |= (v & 0x0F) << 4
```

That's it. The entire exercise is systematic application of this pattern.

## Functions to Implement

- **`dns_pack(hdr, out)`**: Take a `dns_header_t` struct and write exactly 12 bytes to `out` in network byte order (big-endian).
- **`dns_unpack(in, hdr)`**: Read 12 bytes from `in` and populate all fields of a `dns_header_t`.

`dns_print` is provided for debugging.

## Building & Testing

```bash
make test
```

Tests verify your packing against known byte sequences, including real-world DNS query and response headers.

## Real-World Connections

- **Every network protocol**: TCP, IP, UDP, HTTP/2, TLS — all have headers that must be packed/unpacked with exact bit layouts
- **Pixel format conversion**: RGBA ↔ BGRA ↔ RGB565 for display hardware
- **Hardware registers**: Memory-mapped I/O registers on embedded systems have fields at exact bit positions
- **File formats**: PNG chunks, ELF headers, PE executables — all require bit-exact parsing
