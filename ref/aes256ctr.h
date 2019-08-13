#ifndef AES256CTR_H
#define AES256CTR_H

#include <stdint.h>

typedef struct {
  uint64_t sk_exp[120];
	uint32_t ivw[16];
} aes256xof_ctx;

void PQCLEAN_NAMESPACE_aes256_prf(unsigned char *output, unsigned long long outlen, const unsigned char *key, const unsigned char nonce);
void PQCLEAN_NAMESPACE_aes256xof_absorb(aes256xof_ctx *s, const unsigned char *key, unsigned char x, unsigned char y);
void PQCLEAN_NAMESPACE_aes256xof_squeezeblocks(unsigned char *out, unsigned long long nblocks, aes256xof_ctx *s);

#endif
