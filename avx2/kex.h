#ifndef KEX_H
#define KEX_H

#include "params.h"
#include "kyber.h"

#define KYBER_UAKE_SENDABYTES (KYBER_PUBLICKEYBYTES + KYBER_BYTES)
#define KYBER_UAKE_SENDBBYTES (KYBER_BYTES)

#define KYBER_AKE_SENDABYTES (KYBER_PUBLICKEYBYTES + KYBER_BYTES)
#define KYBER_AKE_SENDBBYTES (2*KYBER_BYTES)


typedef unsigned char u8;

void kyber_uake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb);

void kyber_uake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb);

void kyber_uake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk);


void kyber_ake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb);

void kyber_ake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb, const u8 *pka);

void kyber_ake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk, const u8 *ska);


#endif
