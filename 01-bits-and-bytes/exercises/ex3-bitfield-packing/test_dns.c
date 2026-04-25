#include <string.h>
#include "common.h"
#include "dns.h"

/* ── Pack a standard query header ─────────────────────────────── */

void test_pack_standard_query(void) {
    /*
     * Standard query for google.com:
     * ID=0xABCD, QR=0 (query), Opcode=0, AA=0, TC=0, RD=1,
     * RA=0, Z=0, RCODE=0, QDCOUNT=1, rest=0
     *
     * Expected bytes:
     *   0xAB 0xCD  (ID)
     *   0x01       (QR=0, Opcode=0000, AA=0, TC=0, RD=1 → 00000001)
     *   0x00       (RA=0, Z=000, RCODE=0000 → 00000000)
     *   0x00 0x01  (QDCOUNT=1)
     *   0x00 0x00  (ANCOUNT=0)
     *   0x00 0x00  (NSCOUNT=0)
     *   0x00 0x00  (ARCOUNT=0)
     */
    dns_header_t hdr = {0};
    hdr.id = 0xABCD;
    hdr.rd = 1;
    hdr.qdcount = 1;

    uint8_t out[12] = {0};
    dns_pack(&hdr, out);

    ASSERT_EQ(0xAB, out[0]);
    ASSERT_EQ(0xCD, out[1]);
    ASSERT_EQ(0x01, out[2]);
    ASSERT_EQ(0x00, out[3]);
    ASSERT_EQ(0x00, out[4]);
    ASSERT_EQ(0x01, out[5]);
    ASSERT_EQ(0x00, out[6]);
    ASSERT_EQ(0x00, out[7]);
    ASSERT_EQ(0x00, out[8]);
    ASSERT_EQ(0x00, out[9]);
    ASSERT_EQ(0x00, out[10]);
    ASSERT_EQ(0x00, out[11]);
}

/* ── Pack a response header ───────────────────────────────────── */

void test_pack_response(void) {
    /*
     * DNS response: QR=1, AA=1, RD=1, RA=1, RCODE=0
     * ID=0x1234, QDCOUNT=1, ANCOUNT=2
     *
     * Byte 2: QR=1, Opcode=0000, AA=1, TC=0, RD=1 → 10000101 = 0x85
     * Byte 3: RA=1, Z=000, RCODE=0000 → 10000000 = 0x80
     */
    dns_header_t hdr = {0};
    hdr.id = 0x1234;
    hdr.qr = 1;
    hdr.aa = 1;
    hdr.rd = 1;
    hdr.ra = 1;
    hdr.qdcount = 1;
    hdr.ancount = 2;

    uint8_t out[12] = {0};
    dns_pack(&hdr, out);

    ASSERT_EQ(0x12, out[0]);
    ASSERT_EQ(0x34, out[1]);
    ASSERT_EQ(0x85, out[2]);
    ASSERT_EQ(0x80, out[3]);
    ASSERT_EQ(0x00, out[4]);
    ASSERT_EQ(0x01, out[5]);
    ASSERT_EQ(0x00, out[6]);
    ASSERT_EQ(0x02, out[7]);
}

/* ── Unpack known bytes into a header ─────────────────────────── */

void test_unpack_query(void) {
    /* Same standard query bytes from test_pack_standard_query */
    uint8_t in[12] = {0xAB, 0xCD, 0x01, 0x00, 0x00, 0x01,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    dns_header_t hdr = {0};
    dns_unpack(in, &hdr);

    ASSERT_EQ(0xABCD, hdr.id);
    ASSERT_EQ(0,      hdr.qr);
    ASSERT_EQ(0,      hdr.opcode);
    ASSERT_EQ(0,      hdr.aa);
    ASSERT_EQ(0,      hdr.tc);
    ASSERT_EQ(1,      hdr.rd);
    ASSERT_EQ(0,      hdr.ra);
    ASSERT_EQ(0,      hdr.z);
    ASSERT_EQ(0,      hdr.rcode);
    ASSERT_EQ(1,      hdr.qdcount);
    ASSERT_EQ(0,      hdr.ancount);
    ASSERT_EQ(0,      hdr.nscount);
    ASSERT_EQ(0,      hdr.arcount);
}

/* ── Unpack a response with all flags set ─────────────────────── */

void test_unpack_all_flags(void) {
    /*
     * Byte 2: QR=1, Opcode=1111, AA=1, TC=1, RD=1 → 11111111 = 0xFF
     * Byte 3: RA=1, Z=111, RCODE=1111 → 11111111 = 0xFF
     * ID=0xFFFF, all counts=0xFFFF
     */
    uint8_t in[12] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    dns_header_t hdr = {0};
    dns_unpack(in, &hdr);

    ASSERT_EQ(0xFFFF, hdr.id);
    ASSERT_EQ(1,      hdr.qr);
    ASSERT_EQ(0x0F,   hdr.opcode);
    ASSERT_EQ(1,      hdr.aa);
    ASSERT_EQ(1,      hdr.tc);
    ASSERT_EQ(1,      hdr.rd);
    ASSERT_EQ(1,      hdr.ra);
    ASSERT_EQ(7,      hdr.z);
    ASSERT_EQ(0x0F,   hdr.rcode);
    ASSERT_EQ(0xFFFF, hdr.qdcount);
    ASSERT_EQ(0xFFFF, hdr.ancount);
    ASSERT_EQ(0xFFFF, hdr.nscount);
    ASSERT_EQ(0xFFFF, hdr.arcount);
}

/* ── All flags clear ──────────────────────────────────────────── */

void test_unpack_all_clear(void) {
    uint8_t in[12] = {0};
    dns_header_t hdr;
    memset(&hdr, 0xFF, sizeof(hdr)); /* fill with garbage first */
    dns_unpack(in, &hdr);

    ASSERT_EQ(0, hdr.id);
    ASSERT_EQ(0, hdr.qr);
    ASSERT_EQ(0, hdr.opcode);
    ASSERT_EQ(0, hdr.aa);
    ASSERT_EQ(0, hdr.tc);
    ASSERT_EQ(0, hdr.rd);
    ASSERT_EQ(0, hdr.ra);
    ASSERT_EQ(0, hdr.z);
    ASSERT_EQ(0, hdr.rcode);
    ASSERT_EQ(0, hdr.qdcount);
    ASSERT_EQ(0, hdr.ancount);
    ASSERT_EQ(0, hdr.nscount);
    ASSERT_EQ(0, hdr.arcount);
}

/* ── Round-trip: pack then unpack ─────────────────────────────── */

void test_roundtrip(void) {
    dns_header_t original = {
        .id = 0xBEEF,
        .qr = 1,
        .opcode = 5,
        .aa = 0,
        .tc = 1,
        .rd = 1,
        .ra = 1,
        .z = 2,
        .rcode = 3,
        .qdcount = 100,
        .ancount = 200,
        .nscount = 300,
        .arcount = 400
    };

    uint8_t wire[12];
    dns_pack(&original, wire);

    dns_header_t recovered = {0};
    dns_unpack(wire, &recovered);

    ASSERT_EQ(original.id,      recovered.id);
    ASSERT_EQ(original.qr,      recovered.qr);
    ASSERT_EQ(original.opcode,  recovered.opcode);
    ASSERT_EQ(original.aa,      recovered.aa);
    ASSERT_EQ(original.tc,      recovered.tc);
    ASSERT_EQ(original.rd,      recovered.rd);
    ASSERT_EQ(original.ra,      recovered.ra);
    ASSERT_EQ(original.z,       recovered.z);
    ASSERT_EQ(original.rcode,   recovered.rcode);
    ASSERT_EQ(original.qdcount, recovered.qdcount);
    ASSERT_EQ(original.ancount, recovered.ancount);
    ASSERT_EQ(original.nscount, recovered.nscount);
    ASSERT_EQ(original.arcount, recovered.arcount);
}

/* ── Pack with specific opcode and rcode ──────────────────────── */

void test_pack_opcode_rcode(void) {
    /*
     * ID=0x0001, QR=0, Opcode=2 (STATUS), RD=0, RCODE=5 (REFUSED)
     * Byte 2: QR=0, Opcode=0010, AA=0, TC=0, RD=0 → 00010000 = 0x10
     * Byte 3: RA=0, Z=000, RCODE=0101 → 00000101 = 0x05
     */
    dns_header_t hdr = {0};
    hdr.id = 0x0001;
    hdr.opcode = 2;
    hdr.rcode = 5;
    hdr.qdcount = 1;

    uint8_t out[12] = {0};
    dns_pack(&hdr, out);

    ASSERT_EQ(0x00, out[0]);
    ASSERT_EQ(0x01, out[1]);
    ASSERT_EQ(0x10, out[2]);
    ASSERT_EQ(0x05, out[3]);
}

/* ── main ─────────────────────────────────────────────────────── */

int main(void) {
    TEST_BEGIN();
    RUN_TEST(test_pack_standard_query);
    RUN_TEST(test_pack_response);
    RUN_TEST(test_unpack_query);
    RUN_TEST(test_unpack_all_flags);
    RUN_TEST(test_unpack_all_clear);
    RUN_TEST(test_roundtrip);
    RUN_TEST(test_pack_opcode_rcode);
    TEST_END();
}
