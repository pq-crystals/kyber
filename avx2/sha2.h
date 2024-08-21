#ifndef SHA_2_H
#define SHA_2_H

#include <openssl/sha.h>

#define sha256(OUT, IN, INBYTES) SHA256(IN, INBYTES, OUT)
#define sha512(OUT, IN, INBYTES) SHA512(IN, INBYTES, OUT)

#endif
