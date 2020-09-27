#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include <stdint.h>
#include "params.h"

#ifdef KYBER_90S
#define AVX_REJ_UNIFORM_BUFLEN 512
#else
#define AVX_REJ_UNIFORM_BUFLEN 504
#endif


#define rej_uniform_avx KYBER_NAMESPACE(_rej_uniform_avx)
unsigned int rej_uniform_avx(int16_t *r,
                             const unsigned char *buf);

#endif
