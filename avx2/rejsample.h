#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include <stdint.h>
#include "params.h"

#define rej_uniform_avx KYBER_NAMESPACE(rej_uniform_avx)
unsigned int rej_uniform_avx(int16_t *r,
                             unsigned int len,
                             const unsigned char *buf,
                             unsigned int buflen);

#endif
