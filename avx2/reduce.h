/* SPDX-License-Identifier: Apache-2.0 OR CC0-1.0
 * Copyright: Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John Schanck, Peter Schwabe, Gregor Seiler, Damien Stehlé
 * */

#ifndef REDUCE_H
#define REDUCE_H

#include "params.h"
#include <immintrin.h>

#define reduce_avx KYBER_NAMESPACE(reduce_avx)
void reduce_avx(__m256i *r, const __m256i *qdata);
#define tomont_avx KYBER_NAMESPACE(tomont_avx)
void tomont_avx(__m256i *r, const __m256i *qdata);

#endif
