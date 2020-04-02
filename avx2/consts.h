#ifndef CONSTS_H
#define CONSTS_H

#include "params.h"

#define _16XQ            0
#define _16XQINV        16
#define _16XV           32
#define _16XFLO         48
#define _16XFHI         64
#define _16XMONTSQLO    80
#define _16XMONTSQHI    96
#define _16XMASK       112
#define _ZETAS_EXP     128
#define _ZETAS_INV_EXP 528

#ifndef __ASSEMBLER__
#include <stdint.h>
#define qdata KYBER_NAMESPACE(qdata)
extern const uint16_t qdata[];
#endif

#endif
