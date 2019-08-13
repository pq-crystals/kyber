#ifndef AES256CTR_H
#define AES256CTR_H

#include <stdint.h>
#include <immintrin.h>

typedef struct {
  __m128i rkeys[16];
  __m128i n;
} aes256ctr_ctx;

void PQCLEAN_NAMESPACE_aes256ctr_init(aes256ctr_ctx *state,
                    const uint8_t *key,
                    uint16_t nonce);
void PQCLEAN_NAMESPACE_aes256ctr_select(aes256ctr_ctx *state, uint16_t nonce);
void PQCLEAN_NAMESPACE_aes256ctr_squeezeblocks(uint8_t *out,
                             unsigned long long nblocks,
                             aes256ctr_ctx *state);

void PQCLEAN_NAMESPACE_aes256ctr_prf(uint8_t *out,
                   unsigned long long outlen,
                   const uint8_t *seed,
                   uint8_t nonce);

#endif
