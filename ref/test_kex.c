#include <stdio.h>
#include <string.h>

#include "api.h"
#include "kex.h"

#define CRYPTO_BYTES PQCLEAN_NAMESPACE_CRYPTO_BYTES
#define CRYPTO_PUBLICKEYBYTES PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES PQCLEAN_NAMESPACE_CRYPTO_CIPHERTEXTBYTES
#define CRYPTO_SECRETKEYBYTES PQCLEAN_NAMESPACE_CRYPTO_SECRETKEYBYTES

int main(void)
{
  unsigned char pkb[CRYPTO_PUBLICKEYBYTES];
  unsigned char skb[CRYPTO_SECRETKEYBYTES];

  unsigned char pka[CRYPTO_PUBLICKEYBYTES];
  unsigned char ska[CRYPTO_SECRETKEYBYTES];

  unsigned char eska[CRYPTO_SECRETKEYBYTES];

  unsigned char uake_senda[KEX_UAKE_SENDABYTES];
  unsigned char uake_sendb[KEX_UAKE_SENDBBYTES];

  unsigned char ake_senda[KEX_AKE_SENDABYTES];
  unsigned char ake_sendb[KEX_AKE_SENDBBYTES];

  unsigned char tk[KEX_SSBYTES];
  unsigned char ka[KEX_SSBYTES];
  unsigned char kb[KEX_SSBYTES];
  unsigned char zero[KEX_SSBYTES];
  int i;

  for(i=0;i<KEX_SSBYTES;i++)
    zero[i] = 0;

  PQCLEAN_NAMESPACE_crypto_kem_keypair(pkb, skb); // Generate static key for Bob

  PQCLEAN_NAMESPACE_crypto_kem_keypair(pka, ska); // Generate static key for Alice


  // Perform unilaterally authenticated key exchange

  PQCLEAN_NAMESPACE_kex_uake_initA(uake_senda, tk, eska, pkb); // Run by Alice

  PQCLEAN_NAMESPACE_kex_uake_sharedB(uake_sendb, kb, uake_senda, skb); // Run by Bob

  PQCLEAN_NAMESPACE_kex_uake_sharedA(ka, uake_sendb, tk, eska); // Run by Alice

  if(memcmp(ka,kb,KEX_SSBYTES))
    printf("Error in UAKE\n");

  if(!memcmp(ka,zero,KEX_SSBYTES))
    printf("Error: UAKE produces zero key\n");

  // Perform mutually authenticated key exchange

  PQCLEAN_NAMESPACE_kex_ake_initA(ake_senda, tk, eska, pkb); // Run by Alice

  PQCLEAN_NAMESPACE_kex_ake_sharedB(ake_sendb, kb, ake_senda, skb, pka); // Run by Bob

  PQCLEAN_NAMESPACE_kex_ake_sharedA(ka, ake_sendb, tk, eska, ska); // Run by Alice

  if(memcmp(ka,kb,KEX_SSBYTES))
    printf("Error in AKE\n");

  if(!memcmp(ka,zero,KEX_SSBYTES))
    printf("Error: AKE produces zero key\n");


  printf("KEX_UAKE_SENDABYTES: %d\n",KEX_UAKE_SENDABYTES);
  printf("KEX_UAKE_SENDBBYTES: %d\n",KEX_UAKE_SENDBBYTES);

  printf("KEX_AKE_SENDABYTES: %d\n",KEX_AKE_SENDABYTES);
  printf("KEX_AKE_SENDBBYTES: %d\n",KEX_AKE_SENDBBYTES);

  return 0;
}
