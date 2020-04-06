#ifndef SHA_2_H
#define SHA_2_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"

#define sha256 pqcrystals_sha2_ref_sha256
void sha256(uint8_t out[32], const uint8_t *in, size_t inlen);
#define sha512 pqcrystals_sha2_ref_sha512
void sha512(uint8_t out[64], const uint8_t *in, size_t inlen);

#endif
