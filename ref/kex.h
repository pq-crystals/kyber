#ifndef KEX_H
#define KEX_H

#include "api.h"
#include "params.h"

#define KEX_UAKE_SENDABYTES (CRYPTO_PUBLICKEYBYTES + CRYPTO_CIPHERTEXTBYTES)
#define KEX_UAKE_SENDBBYTES (CRYPTO_CIPHERTEXTBYTES)

#define KEX_AKE_SENDABYTES (CRYPTO_PUBLICKEYBYTES + CRYPTO_CIPHERTEXTBYTES)
#define KEX_AKE_SENDBBYTES (2*CRYPTO_CIPHERTEXTBYTES)

#define KEX_SSBYTES 32


typedef unsigned char u8;

void PQCLEAN_NAMESPACE_kex_uake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb);

void PQCLEAN_NAMESPACE_kex_uake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb);

void PQCLEAN_NAMESPACE_kex_uake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk);


void PQCLEAN_NAMESPACE_kex_ake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb);

void PQCLEAN_NAMESPACE_kex_ake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb, const u8 *pka);

void PQCLEAN_NAMESPACE_kex_ake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk, const u8 *ska);


#endif
