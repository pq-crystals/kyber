#ifndef KEX_H
#define KEX_H

#include "api.h"
#include "params.h"

#include <stdint.h>

#define KEX_UAKE_SENDABYTES (CRYPTO_PUBLICKEYBYTES + CRYPTO_CIPHERTEXTBYTES)
#define KEX_UAKE_SENDBBYTES (CRYPTO_CIPHERTEXTBYTES)

#define KEX_AKE_SENDABYTES (CRYPTO_PUBLICKEYBYTES + CRYPTO_CIPHERTEXTBYTES)
#define KEX_AKE_SENDBBYTES (2*CRYPTO_CIPHERTEXTBYTES)

#define KEX_SSBYTES 32


void PQCLEAN_NAMESPACE_kex_uake_initA(uint8_t *send, uint8_t* tk, uint8_t *sk, const uint8_t *pkb);

void PQCLEAN_NAMESPACE_kex_uake_sharedB(uint8_t *send, uint8_t *k, const uint8_t* recv, const uint8_t *skb);

void PQCLEAN_NAMESPACE_kex_uake_sharedA(uint8_t *k, const uint8_t *recv, const uint8_t *tk, const uint8_t *sk);


void PQCLEAN_NAMESPACE_kex_ake_initA(uint8_t *send, uint8_t* tk, uint8_t *sk, const uint8_t *pkb);

void PQCLEAN_NAMESPACE_kex_ake_sharedB(uint8_t *send, uint8_t *k, const uint8_t* recv, const uint8_t *skb, const uint8_t *pka);

void PQCLEAN_NAMESPACE_kex_ake_sharedA(uint8_t *k, const uint8_t *recv, const uint8_t *tk, const uint8_t *sk, const uint8_t *ska);


#endif
