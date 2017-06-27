#include "kex.h"
#include "verify.h"
#include "fips202.h"

void kyber_uake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb)
{
  crypto_kem_keypair(send, sk);
  crypto_kem_enc(send+KYBER_PUBLICKEYBYTES, tk, pkb);
}

void kyber_uake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb)
{
  unsigned char buf[2*KYBER_SHAREDKEYBYTES];
  crypto_kem_enc(send, buf, recv);
  crypto_kem_dec(buf+KYBER_SHAREDKEYBYTES, recv+KYBER_PUBLICKEYBYTES, skb);
  shake128(k,KYBER_SHAREDKEYBYTES,buf,2*KYBER_SHAREDKEYBYTES);
}

void kyber_uake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk)
{
  unsigned char buf[2*KYBER_SHAREDKEYBYTES];
  int i;
  crypto_kem_dec(buf, recv, sk);
  for(i=0;i<KYBER_SHAREDKEYBYTES;i++) 
    buf[i+KYBER_SHAREDKEYBYTES] = tk[i];
  shake128(k,KYBER_SHAREDKEYBYTES,buf,2*KYBER_SHAREDKEYBYTES);
}



void kyber_ake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb)
{
  crypto_kem_keypair(send, sk);
  crypto_kem_enc(send+KYBER_PUBLICKEYBYTES, tk, pkb);
}

void kyber_ake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb, const u8 *pka)
{
  unsigned char buf[3*KYBER_SHAREDKEYBYTES];
  crypto_kem_enc(send, buf, recv);
  crypto_kem_enc(send+KYBER_BYTES, buf+KYBER_SHAREDKEYBYTES, pka);
  crypto_kem_dec(buf+2*KYBER_SHAREDKEYBYTES, recv+KYBER_PUBLICKEYBYTES, skb);
  shake128(k,KYBER_SHAREDKEYBYTES,buf,3*KYBER_SHAREDKEYBYTES);
}

void kyber_ake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk, const u8 *ska)
{
  unsigned char buf[3*KYBER_SHAREDKEYBYTES];
  int i;
  crypto_kem_dec(buf, recv, sk);
  crypto_kem_dec(buf+KYBER_SHAREDKEYBYTES, recv+KYBER_BYTES, ska);
  for(i=0;i<KYBER_SHAREDKEYBYTES;i++) 
    buf[i+2*KYBER_SHAREDKEYBYTES] = tk[i];
  shake128(k,KYBER_SHAREDKEYBYTES,buf,3*KYBER_SHAREDKEYBYTES);
}
