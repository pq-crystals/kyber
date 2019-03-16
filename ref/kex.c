#include "api.h"
#include "kex.h"
#include "fips202.h"

void kex_uake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb)
{
  crypto_kem_keypair(send, sk);
  crypto_kem_enc(send+CRYPTO_PUBLICKEYBYTES, tk, pkb);
}

void kex_uake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb)
{
  unsigned char buf[2*CRYPTO_BYTES];
  crypto_kem_enc(send, buf, recv);
  crypto_kem_dec(buf+CRYPTO_BYTES, recv+CRYPTO_PUBLICKEYBYTES, skb);
  shake256(k,KEX_SSBYTES,buf,2*CRYPTO_BYTES);
}

void kex_uake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk)
{
  unsigned char buf[2*CRYPTO_BYTES];
  int i;
  crypto_kem_dec(buf, recv, sk);
  for(i=0;i<CRYPTO_BYTES;i++)
    buf[i+CRYPTO_BYTES] = tk[i];
  shake256(k,KEX_SSBYTES,buf,2*CRYPTO_BYTES);
}



void kex_ake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb)
{
  crypto_kem_keypair(send, sk);
  crypto_kem_enc(send+CRYPTO_PUBLICKEYBYTES, tk, pkb);
}

void kex_ake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb, const u8 *pka)
{
  unsigned char buf[3*CRYPTO_BYTES];
  crypto_kem_enc(send, buf, recv);
  crypto_kem_enc(send+CRYPTO_CIPHERTEXTBYTES, buf+CRYPTO_BYTES, pka);
  crypto_kem_dec(buf+2*CRYPTO_BYTES, recv+CRYPTO_PUBLICKEYBYTES, skb);
  shake256(k,KEX_SSBYTES,buf,3*CRYPTO_BYTES);
}

void kex_ake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk, const u8 *ska)
{
  unsigned char buf[3*CRYPTO_BYTES];
  int i;
  crypto_kem_dec(buf, recv, sk);
  crypto_kem_dec(buf+CRYPTO_BYTES, recv+CRYPTO_CIPHERTEXTBYTES, ska);
  for(i=0;i<CRYPTO_BYTES;i++)
    buf[i+2*CRYPTO_BYTES] = tk[i];
  shake256(k,KEX_SSBYTES,buf,3*CRYPTO_BYTES);
}
