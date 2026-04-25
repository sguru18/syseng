#ifndef DNS_H
#define DNS_H

#include <stdint.h>

typedef struct {
    uint16_t id;
    int qr;          /* 1 bit: 0 = query, 1 = response */
    int opcode;      /* 4 bits: 0 = standard query */
    int aa;           /* 1 bit: authoritative answer */
    int tc;           /* 1 bit: truncated */
    int rd;           /* 1 bit: recursion desired */
    int ra;           /* 1 bit: recursion available */
    int z;            /* 3 bits: reserved (must be zero) */
    int rcode;        /* 4 bits: response code */
    uint16_t qdcount; /* number of questions */
    uint16_t ancount; /* number of answer RRs */
    uint16_t nscount; /* number of authority RRs */
    uint16_t arcount; /* number of additional RRs */
} dns_header_t;

void dns_pack(const dns_header_t *hdr, uint8_t out[12]);
void dns_unpack(const uint8_t in[12], dns_header_t *hdr);
void dns_print(const dns_header_t *hdr);

#endif /* DNS_H */
