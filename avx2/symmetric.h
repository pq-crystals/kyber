#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "params.h"

#ifdef KYBER_90S

#include "aes256ctr.h"
#include <openssl/sha.h>

#if (KYBER_SSBYTES != 32)
#error "90s variant of Kyber can only generate keys of length 256 bits"
#endif

#define hash_h(OUT, IN, INBYTES) SHA256(IN, INBYTES, OUT)
#define hash_g(OUT, IN, INBYTES) SHA512(IN, INBYTES, OUT)
#define xof_absorb(STATE, IN, X, Y) PQCLEAN_NAMESPACE_aes256ctr_init(STATE, IN, Y + ((uint16_t)X << 8))
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_NAMESPACE_aes256ctr_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) PQCLEAN_NAMESPACE_aes256ctr_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) SHA256(IN, INBYTES, OUT)

#define XOF_BLOCKBYTES 128

typedef aes256ctr_ctx xof_state;

#else

#include "fips202.h"
#include "fips202x4.h"

typedef shake128ctx keccak_state;

void PQCLEAN_NAMESPACE_kyber_shake128_absorb(keccak_state *s, const uint8_t *input, uint8_t x, uint8_t y);
void PQCLEAN_NAMESPACE_kyber_shake128_squeezeblocks(uint8_t *output, size_t nblocks, keccak_state *s);
void PQCLEAN_NAMESPACE_shake256_prf(uint8_t *output, size_t outlen, const uint8_t *key, uint8_t nonce);

#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha3_512(OUT, IN, INBYTES)
#define xof_absorb(STATE, IN, X, Y) PQCLEAN_NAMESPACE_kyber_shake128_absorb(STATE, IN, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) PQCLEAN_NAMESPACE_shake256_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) shake256(OUT, KYBER_SSBYTES, IN, INBYTES)

#define XOF_BLOCKBYTES SHAKE128_RATE

typedef keccak_state xof_state;

#endif /* KYBER_90S */

#endif /* SYMMETRIC_H */
