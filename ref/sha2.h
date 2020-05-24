#ifndef SHA_2_H
#define SHA_2_H

#include <stddef.h>
#include <stdint.h>

#define SHA2_NAMESPACE(s) pqcrystals_sha2_ref##s

#define sha256 SHA2_NAMESPACE(_sha2)
void sha256(uint8_t out[32], const uint8_t *in, size_t inlen);
#define sha512 SHA2_NAMESPACE(_sha512)
void sha512(uint8_t out[64], const uint8_t *in, size_t inlen);

#endif
