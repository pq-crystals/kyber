#include <stdint.h>
#include "params.h"

double _neg2[4] asm ("_neg2") = {1., -1., 1., -1.};
double _q[4] asm ("_q")   = {KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q};
double _qinv[4] asm ("_qinv") = {0.000130191381330556,0.000130191381330556,0.000130191381330556,0.000130191381330556};
double _one[4] asm ("_one") = {1.,1.,1.,1.};
unsigned char _mask11[32] asm ("_mask11") = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
unsigned char _mask0f[32] asm ("_mask0f") = {0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f};
uint16_t _q16x[16] asm ("_q16x") = {KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q, KYBER_Q};
uint16_t _lowdword[16] asm ("_lowdword") = {0xffff, 0x0, 0xffff, 0x0, 0xffff, 0x0, 0xffff, 0x0, 0xffff, 0x0, 0xffff, 0x0, 0xffff, 0x0, 0xffff, 0x0};

#define Q KYBER_Q
#define LOW ((1U << 13) - 1)
#define MONT 4088U
#define F ((((uint32_t)MONT*MONT % Q) * (Q-1) % Q) * ((Q-1) >> 8) % Q)

const uint16_t _16xqinv[16] __attribute__((aligned(32))) = {57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857, 57857};
const uint16_t _16xq[16] __attribute__((aligned(32))) = {Q, Q, Q, Q, Q, Q, Q, Q, Q, Q, Q, Q, Q, Q, Q, Q};
const uint16_t _16x4q[16] __attribute__((aligned(32))) = {4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q, 4*Q};
const uint16_t _16x2q[16] __attribute__((aligned(32))) = {2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q, 2*Q};
const uint16_t _low_mask[16] __attribute__((aligned(32))) = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
const uint8_t _vpshufb_idx[32] __attribute__((aligned(32))) = {2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13, 2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13};
const uint16_t _f[16] __attribute__((aligned(32))) = {F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F};

#undef Q
#undef F
#undef MONT
#undef LOW
