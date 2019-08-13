#include "api.h"
#include "kex.h"
#include "fips202.h"

void PQCLEAN_NAMESPACE_kex_uake_initA(uint8_t *send, uint8_t* tk, uint8_t *sk, const uint8_t *pkb)
{
  PQCLEAN_NAMESPACE_crypto_kem_keypair(send, sk);
  PQCLEAN_NAMESPACE_crypto_kem_enc(send+PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES, tk, pkb);
}

void PQCLEAN_NAMESPACE_kex_uake_sharedB(uint8_t *send, uint8_t *k, const uint8_t* recv, const uint8_t *skb)
{
  uint8_t buf[2*PQCLEAN_NAMESPACE_CRYPTO_BYTES];
  PQCLEAN_NAMESPACE_crypto_kem_enc(send, buf, recv);
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf+PQCLEAN_NAMESPACE_CRYPTO_BYTES, recv+PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES, skb);
  shake256(k,KEX_SSBYTES,buf,2*PQCLEAN_NAMESPACE_CRYPTO_BYTES);
}

void PQCLEAN_NAMESPACE_kex_uake_sharedA(uint8_t *k, const uint8_t *recv, const uint8_t *tk, const uint8_t *sk)
{
  uint8_t buf[2*PQCLEAN_NAMESPACE_CRYPTO_BYTES];
  int i;
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf, recv, sk);
  for(i=0;i<PQCLEAN_NAMESPACE_CRYPTO_BYTES;i++)
    buf[i+PQCLEAN_NAMESPACE_CRYPTO_BYTES] = tk[i];
  shake256(k,KEX_SSBYTES,buf,2*PQCLEAN_NAMESPACE_CRYPTO_BYTES);
}



void PQCLEAN_NAMESPACE_kex_ake_initA(uint8_t *send, uint8_t* tk, uint8_t *sk, const uint8_t *pkb)
{
  PQCLEAN_NAMESPACE_crypto_kem_keypair(send, sk);
  PQCLEAN_NAMESPACE_crypto_kem_enc(send+PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES, tk, pkb);
}

void PQCLEAN_NAMESPACE_kex_ake_sharedB(uint8_t *send, uint8_t *k, const uint8_t* recv, const uint8_t *skb, const uint8_t *pka)
{
  uint8_t buf[3*PQCLEAN_NAMESPACE_CRYPTO_BYTES];
  PQCLEAN_NAMESPACE_crypto_kem_enc(send, buf, recv);
  PQCLEAN_NAMESPACE_crypto_kem_enc(send+PQCLEAN_NAMESPACE_CRYPTO_CIPHERTEXTBYTES, buf+PQCLEAN_NAMESPACE_CRYPTO_BYTES, pka);
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf+2*PQCLEAN_NAMESPACE_CRYPTO_BYTES, recv+PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES, skb);
  shake256(k,KEX_SSBYTES,buf,3*PQCLEAN_NAMESPACE_CRYPTO_BYTES);
}

void PQCLEAN_NAMESPACE_kex_ake_sharedA(uint8_t *k, const uint8_t *recv, const uint8_t *tk, const uint8_t *sk, const uint8_t *ska)
{
  uint8_t buf[3*PQCLEAN_NAMESPACE_CRYPTO_BYTES];
  int i;
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf, recv, sk);
  PQCLEAN_NAMESPACE_crypto_kem_dec(buf+PQCLEAN_NAMESPACE_CRYPTO_BYTES, recv+PQCLEAN_NAMESPACE_CRYPTO_CIPHERTEXTBYTES, ska);
  for(i=0;i<PQCLEAN_NAMESPACE_CRYPTO_BYTES;i++)
    buf[i+2*PQCLEAN_NAMESPACE_CRYPTO_BYTES] = tk[i];
  shake256(k,KEX_SSBYTES,buf,3*PQCLEAN_NAMESPACE_CRYPTO_BYTES);
}
