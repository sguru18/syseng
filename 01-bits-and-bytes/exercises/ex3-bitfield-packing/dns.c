/*
 * DNS Header Packing/Unpacking
 *
 * Wire format (12 bytes, big-endian / network byte order):
 *
 *   Byte  0: ID high byte
 *   Byte  1: ID low byte
 *   Byte  2: QR(1) | Opcode(4) | AA(1) | TC(1) | RD(1)
 *   Byte  3: RA(1) | Z(3) | RCODE(4)
 *   Byte  4: QDCOUNT high byte
 *   Byte  5: QDCOUNT low byte
 *   Byte  6: ANCOUNT high byte
 *   Byte  7: ANCOUNT low byte
 *   Byte  8: NSCOUNT high byte
 *   Byte  9: NSCOUNT low byte
 *   Byte 10: ARCOUNT high byte
 *   Byte 11: ARCOUNT low byte
 *
 * Byte 2 bit layout (MSB first):
 *   [7]     QR
 *   [6:3]   Opcode
 *   [2]     AA
 *   [1]     TC
 *   [0]     RD
 *
 * Byte 3 bit layout (MSB first):
 *   [7]     RA
 *   [6:4]   Z
 *   [3:0]   RCODE
 */

#include <stdio.h>
#include "dns.h"

/*
 * TODO: Pack a dns_header_t into exactly 12 bytes in network byte order.
 *
 * For each 16-bit field (id, qdcount, etc.), the high byte goes first (big-endian).
 * For the flags byte (byte 2), combine QR, Opcode, AA, TC, RD into a single byte
 * using shifts and OR. Same idea for byte 3 with RA, Z, RCODE.
 *
 * Example for byte 2:
 *   byte2 = (qr << 7) | (opcode << 3) | (aa << 2) | (tc << 1) | rd
 */
void dns_pack(const dns_header_t *hdr, uint8_t out[12]) {
    (void)hdr;
    (void)out;
    /* TODO: implement packing */
}

/*
 * TODO: Unpack 12 bytes (network byte order) into a dns_header_t.
 *
 * This is the reverse of dns_pack. For each 16-bit field, combine two bytes:
 *   field = (in[high] << 8) | in[low]
 *
 * For the flags bytes, extract each field using shifts and masks:
 *   qr = (byte2 >> 7) & 0x01
 *   opcode = (byte2 >> 3) & 0x0F
 *   ... etc.
 */
void dns_unpack(const uint8_t in[12], dns_header_t *hdr) {
    (void)in;
    (void)hdr;
    /* TODO: implement unpacking */
}

void dns_print(const dns_header_t *hdr) {
    printf("DNS Header:\n");
    printf("  ID:      0x%04X\n", hdr->id);
    printf("  QR:      %d (%s)\n", hdr->qr, hdr->qr ? "response" : "query");
    printf("  Opcode:  %d\n", hdr->opcode);
    printf("  AA:      %d\n", hdr->aa);
    printf("  TC:      %d\n", hdr->tc);
    printf("  RD:      %d\n", hdr->rd);
    printf("  RA:      %d\n", hdr->ra);
    printf("  Z:       %d\n", hdr->z);
    printf("  RCODE:   %d\n", hdr->rcode);
    printf("  QDCOUNT: %u\n", hdr->qdcount);
    printf("  ANCOUNT: %u\n", hdr->ancount);
    printf("  NSCOUNT: %u\n", hdr->nscount);
    printf("  ARCOUNT: %u\n", hdr->arcount);
}
