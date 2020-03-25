#ifndef CONSTS_H
#define CONSTS_H

#include "params.h"

#define zetas_exp KYBER_NAMESPACE(zetas_exp)
#define zetas_inv_exp KYBER_NAMESPACE(zetas_inv_exp)
#define _16xq KYBER_NAMESPACE(_16xq)
#define _16xqinv KYBER_NAMESPACE(_16xqinv)
#define _16xv KYBER_NAMESPACE(_16xv)
#define _16xflo KYBER_NAMESPACE(_16xflo)
#define _16xfhi KYBER_NAMESPACE(_16xfhi)
#define _16xmontsqlo KYBER_NAMESPACE(_16xmontsqlo)
#define _16xmontsqhi KYBER_NAMESPACE(_16xmontsqhi)
#define _16xmask KYBER_NAMESPACE(_16xmask)

#ifndef __ASSEMBLER__
#include <stdint.h>
extern const uint16_t zetas_exp[396];
extern const uint16_t zetas_inv_exp[396];
extern const uint16_t _16xq[16];
extern const uint16_t _16xqinv[16];
extern const uint16_t _16xv[16];
#endif

#endif
