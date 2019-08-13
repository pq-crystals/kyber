#include "api.h"
#include "kex.h"
#include "fips202.h"

void PQCLEAN_NAMESPACE_kex_uake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb)
{
  PQCLEAN_NAMESPACE_crypto_kem_keypair(send, sk);
  PQCLEAN_NAMESPACE_crypto_kem_enc(send+PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES, tk, pkb);
}

void PQCLEAN_NAMESPACE_kex_uake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb)
{
  unsigned char buf[2*PQCLEAN_NAMESPACE_CRYPTO_BYTES];
  PQCLEAN_NAMESPACE_crypto_kem_enc(send, buf, recv);
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf+PQCLEAN_NAMESPACE_CRYPTO_BYTES, recv+PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES, skb);
  shake256(k,KEX_SSBYTES,buf,2*PQCLEAN_NAMESPACE_CRYPTO_BYTES);
}

void PQCLEAN_NAMESPACE_kex_uake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk)
{
  unsigned char buf[2*PQCLEAN_NAMESPACE_CRYPTO_BYTES];
  int i;
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf, recv, sk);
  for(i=0;i<PQCLEAN_NAMESPACE_CRYPTO_BYTES;i++)
    buf[i+PQCLEAN_NAMESPACE_CRYPTO_BYTES] = tk[i];
  shake256(k,KEX_SSBYTES,buf,2*PQCLEAN_NAMESPACE_CRYPTO_BYTES);
}



void PQCLEAN_NAMESPACE_kex_ake_initA(u8 *send, u8* tk, u8 *sk, const u8 *pkb)
{
  PQCLEAN_NAMESPACE_crypto_kem_keypair(send, sk);
  PQCLEAN_NAMESPACE_crypto_kem_enc(send+PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES, tk, pkb);
}

void PQCLEAN_NAMESPACE_kex_ake_sharedB(u8 *send, u8 *k, const u8* recv, const u8 *skb, const u8 *pka)
{
  unsigned char buf[3*PQCLEAN_NAMESPACE_CRYPTO_BYTES];
  PQCLEAN_NAMESPACE_crypto_kem_enc(send, buf, recv);
  PQCLEAN_NAMESPACE_crypto_kem_enc(send+PQCLEAN_NAMESPACE_CRYPTO_CIPHERTEXTBYTES, buf+PQCLEAN_NAMESPACE_CRYPTO_BYTES, pka);
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf+2*PQCLEAN_NAMESPACE_CRYPTO_BYTES, recv+PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES, skb);
  shake256(k,KEX_SSBYTES,buf,3*PQCLEAN_NAMESPACE_CRYPTO_BYTES);
}

void PQCLEAN_NAMESPACE_kex_ake_sharedA(u8 *k, const u8 *recv, const u8 *tk, const u8 *sk, const u8 *ska)
{
  unsigned char buf[3*PQCLEAN_NAMESPACE_CRYPTO_BYTES];
  int i;
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf, recv, sk);
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf+PQCLEAN_NAMESPACE_CRYPTO_BYTES, recv+PQCLEAN_NAMESPACE_CRYPTO_CIPHERTEXTBYTES, ska);
  for(i=0;i<PQCLEAN_NAMESPACE_CRYPTO_BYTES;i++)
    buf[i+2*PQCLEAN_NAMESPACE_CRYPTO_BYTES] = tk[i];
  shake256(k,KEX_SSBYTES,buf,3*PQCLEAN_NAMESPACE_CRYPTO_BYTES);
}
