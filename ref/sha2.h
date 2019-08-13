#ifndef SHA_2_H
#define SHA_2_H

#include <stdint.h>

void sha512(uint8_t *out,const unsigned char *in,unsigned long long inlen);

void sha256(uint8_t *out,const unsigned char *in,unsigned long long inlen);

#endif
