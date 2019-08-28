#ifndef CONSTS_H
#define CONSTS_H

#include <immintrin.h>
#include <stdint.h>

typedef union {
    uint16_t as_arr[16];
    __m256i as_vec;
} aligned_uint16_t;

extern const uint16_t zetas_exp[396];
extern const uint16_t zetas_inv_exp[396];

extern const aligned_uint16_t _16xq;
extern const aligned_uint16_t _16xqinv;
extern const aligned_uint16_t _16xv;
extern const aligned_uint16_t _16xflo;
extern const aligned_uint16_t _16xfhi;
extern const aligned_uint16_t _16xmontsqlo;
extern const aligned_uint16_t _16xmontsqhi;
extern const aligned_uint16_t _16xmask;

#endif
