#ifndef FIPS202_H
#define FIPS202_H

#include <stddef.h>
#include <stdint.h>

#define SHAKE128_RATE 168
#define SHAKE256_RATE 136
#define SHA3_256_RATE 136
#define SHA3_512_RATE  72

void shake128_absorb(uint64_t *s, const uint8_t *input, unsigned int inputByteLen);
void shake128_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *s);

void shake256(uint8_t *output, size_t outlen, const uint8_t *input,  size_t inlen);

void sha3_256(uint8_t *output, const uint8_t *input,  size_t inlen);
void sha3_512(uint8_t *output, const uint8_t *input,  size_t inlen);

#endif
