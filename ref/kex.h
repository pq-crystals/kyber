#ifndef KEX_H
#define KEX_H

#include <stdint.h>
#include "api.h"

#define KEX_UAKE_SENDABYTES (CRYPTO_PUBLICKEYBYTES + CRYPTO_CIPHERTEXTBYTES)
#define KEX_UAKE_SENDBBYTES (CRYPTO_CIPHERTEXTBYTES)

#define KEX_AKE_SENDABYTES (CRYPTO_PUBLICKEYBYTES + CRYPTO_CIPHERTEXTBYTES)
#define KEX_AKE_SENDBBYTES (2*CRYPTO_CIPHERTEXTBYTES)

#define KEX_SSBYTES CRYPTO_BYTES

void kex_uake_initA(uint8_t *send, uint8_t *tk, uint8_t *sk, const uint8_t *pkb);

void kex_uake_sharedB(uint8_t *send, uint8_t *k, const uint8_t *recv, const uint8_t *skb);

void kex_uake_sharedA(uint8_t *k, const uint8_t *recv, const uint8_t *tk, const uint8_t *sk);

void kex_ake_initA(uint8_t *send, uint8_t *tk, uint8_t *sk, const uint8_t *pkb);
void kex_ake_sharedB(uint8_t *send, uint8_t *k, const uint8_t *recv, const uint8_t *skb, const uint8_t *pka);

void kex_ake_sharedA(uint8_t *k, const uint8_t *recv, const uint8_t *tk, const uint8_t *sk, const uint8_t *ska);

#endif
