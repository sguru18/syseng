/*
 * DNS Header Packing/Unpacking
 *
 * Wire format (12 bytes, big-endian / network byte order):
 *
 *   Byte  0-1:  ID  (high byte first)
 *   Byte  2:    QR(1) | Opcode(4) | AA(1) | TC(1) | RD(1)
 *   Byte  3:    RA(1) | Z(3) | RCODE(4)
 *   Byte  4-5:  QDCOUNT
 *   Byte  6-7:  ANCOUNT
 *   Byte  8-9:  NSCOUNT
 *   Byte 10-11: ARCOUNT
 *
 * INCLUDES: Figure out what you need.
 *   You'll need your header and something for printf (used by dns_print).
 */

/* TODO: add your #include lines here */

/* TODO: Implement dns_pack -- see dns.h for the interface. */
void dns_pack(const dns_header_t *hdr, uint8_t out[12]) {
    (void)hdr;
    (void)out;
}

/* TODO: Implement dns_unpack -- see dns.h for the interface. */
void dns_unpack(const uint8_t in[12], dns_header_t *hdr) {
    (void)in;
    (void)hdr;
}

/* Provided: print a dns header for debugging. */
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
