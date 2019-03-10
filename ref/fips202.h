#ifndef FIPS202_H
#define FIPS202_H

#include <stdint.h>

#define SHAKE128_RATE 168
#define SHAKE256_RATE 136
#define SHA3_256_RATE 136
#define SHA3_512_RATE  72

typedef struct {
  uint64_t s[25];
} keccak_state;

void kyber_shake128_absorb(keccak_state *s, const unsigned char *input, unsigned char i, unsigned char j);
void shake128_squeezeblocks(unsigned char *output, unsigned long long nblocks, keccak_state *s);

void shake256(unsigned char *output, unsigned long long outlen, const unsigned char *input,  unsigned long long inlen);
void shake256_prf(unsigned char *output, unsigned long long outlen, const unsigned char *key, unsigned long long keylen, const unsigned char nonce);

void sha3_256(unsigned char *output, const unsigned char *input,  unsigned long long inlen);
void sha3_512(unsigned char *output, const unsigned char *input,  unsigned long long inlen);

#endif
