#ifndef FIPS202_H
#define FIPS202_H

#include <stdint.h>

#define SHAKE128_RATE 168
#define SHA3_256_RATE 136
#define SHA3_512_RATE 72

void shake128(unsigned char *output, unsigned long long outlen, const unsigned char *input,  unsigned long long inlen);

void cshake128_simple_absorb(uint64_t *s, uint16_t cstm, const unsigned char *in, unsigned long long inlen);
void cshake128_simple_squeezeblocks(unsigned char *output, unsigned long long nblocks, uint64_t *s);
void cshake128_simple(unsigned char *output, unsigned long long outlen, uint16_t cstm, const unsigned char *in, unsigned long long inlen);

//int crypto_stream_shake128(unsigned char *output, unsigned long long outlen, const unsigned char *nonce, const unsigned char *key);

#endif
