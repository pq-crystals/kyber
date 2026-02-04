#include <stdint.h>
#include "params.h"
#include "ntt.h"
#include "reduce.h"
#include <stdio.h>
#include <arm_neon.h>

/* Code to generate zetas and zetas_inv used in the number-theoretic transform:

#define KYBER_ROOT_OF_UNITY 17

static const uint8_t tree[128] = {
  0, 64, 32, 96, 16, 80, 48, 112, 8, 72, 40, 104, 24, 88, 56, 120,
  4, 68, 36, 100, 20, 84, 52, 116, 12, 76, 44, 108, 28, 92, 60, 124,
  2, 66, 34, 98, 18, 82, 50, 114, 10, 74, 42, 106, 26, 90, 58, 122,
  6, 70, 38, 102, 22, 86, 54, 118, 14, 78, 46, 110, 30, 94, 62, 126,
  1, 65, 33, 97, 17, 81, 49, 113, 9, 73, 41, 105, 25, 89, 57, 121,
  5, 69, 37, 101, 21, 85, 53, 117, 13, 77, 45, 109, 29, 93, 61, 125,
  3, 67, 35, 99, 19, 83, 51, 115, 11, 75, 43, 107, 27, 91, 59, 123,
  7, 71, 39, 103, 23, 87, 55, 119, 15, 79, 47, 111, 31, 95, 63, 127
};

void init_ntt() {
  unsigned int i;
  int16_t tmp[128];

  tmp[0] = MONT;
  for(i=1;i<128;i++)
    tmp[i] = fqmul(tmp[i-1],MONT*KYBER_ROOT_OF_UNITY % KYBER_Q);

  for(i=0;i<128;i++) {
    zetas[i] = tmp[tree[i]];
    if(zetas[i] > KYBER_Q/2)
      zetas[i] -= KYBER_Q;
    if(zetas[i] < -KYBER_Q/2)
      zetas[i] += KYBER_Q;
  }
}
*/

const int16_t zetas[128] = {
  -1044,  -758,  -359, -1517,  1493,  1422,   287,   202,
   -171,   622,  1577,   182,   962, -1202, -1474,  1468,
    573, -1325,   264,   383,  -829,  1458, -1602,  -130,
   -681,  1017,   732,   608, -1542,   411,  -205, -1571,
   1223,   652,  -552,  1015, -1293,  1491,  -282, -1544,
    516,    -8,  -320,  -666, -1618, -1162,   126,  1469,
   -853,   -90,  -271,   830,   107, -1421,  -247,  -951,
   -398,   961, -1508,  -725,   448, -1065,   677, -1275,
  -1103,   430,   555,   843, -1251,   871,  1550,   105,
    422,   587,   177,  -235,  -291,  -460,  1574,  1653,
   -246,   778,  1159,  -147,  -777,  1483,  -602,  1119,
  -1590,   644,  -872,   349,   418,   329,  -156,   -75,
    817,  1097,   603,   610,  1322, -1285, -1465,   384,
  -1215,  -136,  1218, -1335,  -874,   220, -1187, -1659,
  -1185, -1530, -1278,   794, -1510,  -854,  -870,   478,
   -108,  -308,   996,   991,   958, -1460,  1522,  1628
};

/*************************************************
* Name:        fqmul
*
* Description: Multiplication followed by Montgomery reduction
*
* Arguments:   - int16_t a: first factor
*              - int16_t b: second factor
*
* Returns 16-bit integer congruent to a*b*R^{-1} mod q
**************************************************/



static int16_t fqmul(int16_t a, int16_t b) {
  return montgomery_reduce((int32_t)a * b);
}

static inline int16x8_t v_fqmul_neon(int16x8_t v_a, int16x8_t v_zeta) {
    int32x4_t v_prod_l = vmull_s16(vget_low_s16(v_a), vget_low_s16(v_zeta));
    int32x4_t v_prod_h = vmull_high_s16(v_a, v_zeta);
/*
  vget_low_s16: 32x4=128비트에서 앞 64비트(4개)만 추출
  vmull_s16: 16비트x16비트=32비트. vmul = 16비트
  vmull_high_s16: 상위 64비트(4개) 추출하여 16비트x16비트 곱셈 수행
*/
    int16x4_t v_m_l = vmul_n_s16(vmovn_s32(v_prod_l), (int16_t)QINV);
    int16x4_t v_m_h = vmul_n_s16(vmovn_s32(v_prod_h), (int16_t)QINV); 
/*
  vmul_n_s16: 레지스터에 든 값들에 특정 수 n(QINV) 하나를 똑같이 곱함
  vmovn_s32: 32비트 데이터를 16비트로 압축. 하위 16비트만 가져옴
  명령어 끝의 n: narrow. 결과값을 절반으로 압축할 때 (32비트 → 16비트).
*/
    int32x4_t v_res_l = vmlsl_n_s16(v_prod_l, v_m_l, (int16_t)KYBER_Q);
    int32x4_t v_res_h = vmlsl_n_s16(v_prod_h, v_m_h, (int16_t)KYBER_Q);
/*
  Vector Muliply Look-up Substract Long: 
  Mul: v_m_l에 Q를 곱함
  Sub: 원래 곱셈 결과인 v_prod_l에서 v_m_l * Q를 뺌
  Long: 32비트 공간에서 수행 
  연산 중간의 _n_: number (Scalar(숫자 하나/Immediate(상수)). 즉, 레지스터가 아니라 뒤에 특정 숫자 하나를 인자로 받음
*/

    return vcombine_s16(vshrn_n_s32(v_res_l, 16), vshrn_n_s32(v_res_h, 16));
/*
  vshrn_n_s32(Vector Shift Hright Right and Narrow): >>16, 32bit->16bit
  vcombine_s16(l, h): 16비트 데이터 8개 리턴 -> 하나의 128비트 레지스터. 64비트(16x4, l)조각과 또다른 64비트(16x4, h)를 가져와
  하나의 128비트 레지스터로. int16x8_t 데이터를 return
*/
/*
  "몽고메리 리덕션의 최종 단계인 $2^{16}$ 나눗셈을 NEON의 vshrn(Shift Right and Narrow) 명령어로 구현하여 
  연산과 데이터 압축을 동시에 처리했습니다. 분리되어 연산 된 상/하위 4개 요소(Elements)를 vcombine으로 병합함으로써, 
  128비트 레지스터의 활용률을 극대화하고 후속 NTT 나비 연산으로 즉시 전달 가능한 구조를 만들었습니다.
*/
}







/*************************************************
* Name:        ntt
*
* Description: Inplace number-theoretic transform (NTT) in Rq.
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/

void ntt(int16_t r[256]) {
  unsigned int len, start, j, k;
  int16x8_t v_rj, v_rj_len, v_t, v_zeta, v_sum, v_diff;

  k = 1;
  for (len = 128; len >= 2; len >>= 1) {
    for (start = 0; start < 256; start = j + len) {
      //zeta 하나를 가져와 8개 칸에 복사 (vdup)
      int16_t zeta = zetas[k++];
      v_zeta = vdupq_n_s16(zeta);

      //데이터가 연속적인 len >= 8 구간만 NEON으로 처리
      if (len >= 8) {
        for (j = start; j < start + len; j += 8) {
          v_rj = vld1q_s16(&r[j]);
          v_rj_len = vld1q_s16(&r[j + len]);

          v_t = v_fqmul_neon(v_rj_len, v_zeta);

          //나비연산 차/합
          v_diff = vsubq_s16(v_rj, v_t); 
          v_sum = vaddq_s16(v_rj, v_t); 

          vst1q_s16(&r[j], v_sum);
          vst1q_s16(&r[j + len], v_diff); //vstlq: 계산이 끝난 벡터를 다시 배열에 넣을 때
        }
      }
      else {
                // len이 4, 2일 때는 8개씩 묶을 수 없으므로 기존 스칼라 방식으로 처리
                // (이 부분은 나중에 vtrn 같은 셔플 명령어로 최적화 가능)
                for(j = start; j < start + len; j++) {
                    int16_t t = fqmul(zeta, r[j + len]);
                    r[j + len] = r[j] - t;
                    r[j] = r[j] + t;
                }
      }
    }
  }
} 


/*************************************************
* Name:        invntt_tomont
*
* Description: Inplace inverse number-theoretic transform in Rq and
*              multiplication by Montgomery factor 2^16.
*              Input is in bitreversed order, output is in standard order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void invntt(int16_t r[256]) {
  unsigned int start, len, j, k;
  int16_t t, zeta;
  const int16_t f = 1441; // mont^2/128

  k = 127;
  for(len = 2; len <= 128; len <<= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas[k--];
      for(j = start; j < start + len; j++) {
        t = r[j];
        r[j] = barrett_reduce(t + r[j + len]);
        r[j + len] = r[j + len] - t;
        r[j + len] = fqmul(zeta, r[j + len]);
      }
    }
  }

  for(j = 0; j < 256; j++)
    r[j] = fqmul(r[j], f);
}

/*************************************************
* Name:        basemul
*
* Description: Multiplication of polynomials in Zq[X]/(X^2-zeta)
*              used for multiplication of elements in Rq in NTT domain
*
* Arguments:   - int16_t r[2]: pointer to the output polynomial
*              - const int16_t a[2]: pointer to the first factor
*              - const int16_t b[2]: pointer to the second factor
*              - int16_t zeta: integer defining the reduction polynomial
**************************************************/
void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta)
{
  r[0]  = fqmul(a[1], b[1]);
  r[0]  = fqmul(r[0], zeta);
  r[0] += fqmul(a[0], b[0]);
  r[1]  = fqmul(a[0], b[1]);
  r[1] += fqmul(a[1], b[0]);
}
