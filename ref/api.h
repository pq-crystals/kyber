#ifndef API_H
#define API_H

#include <stdint.h>

#if   (KYBER_K == 2)
#define CRYPTO_SECRETKEYBYTES  1632
#define CRYPTO_PUBLICKEYBYTES  800
#define CRYPTO_CIPHERTEXTBYTES 736
#define CRYPTO_BYTES           32
#define CRYPTO_ALGNAME "Kyber512"
#elif (KYBER_K == 3)
#define CRYPTO_SECRETKEYBYTES  2400
#define CRYPTO_PUBLICKEYBYTES  1184
#define CRYPTO_CIPHERTEXTBYTES 1088
#define CRYPTO_BYTES           32
#define CRYPTO_ALGNAME "Kyber768"
#elif (KYBER_K == 4)
#define CRYPTO_SECRETKEYBYTES  3168
#define CRYPTO_PUBLICKEYBYTES  1568
#define CRYPTO_CIPHERTEXTBYTES 1568
#define CRYPTO_BYTES           32
#define CRYPTO_ALGNAME "Kyber1024"
#else
#error "KYBER_K must be in {2,3,4}"
#endif

int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);


#endif
