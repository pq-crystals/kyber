  .thumb
  .syntax unified
.text

/*************************************************
* Barrett reduction
* - a: register to be reduced
* - tmp: temporary register
* - q: register containing q
**************************************************/
.macro barrett_reduce_asm a, tmp, q
  lsr \tmp, \a, #13    // tmp = a>>13
  mls \a, \tmp, \q, \a // a -= tmp * q
.endm

/*************************************************
* Montgomery reduction
* - a: register to be reduced
* - tmp: temporary register
* - q: register containing q
* - qinv: register containing qinv = -inverse_mod(q,2^18)
* - mask: register containing (1<<18)-1
**************************************************/
.macro montgomery_reduce_asm a, tmp, q, qinv, mask
  mul \tmp, \a, \qinv  // tmp = (a * qinv)
  and \tmp, \mask      // tmp &= mask
  mla \a, \tmp, \q, \a // a += tmp * q
  lsr \a, #18          // a = a>>18
.endm

/*************************************************
* NTT Butterfly
**************************************************/
.macro butterfly coeffs, jlevel, r4, r5, r6, qinv, zeta, q4, mask, q
  ldrh \r6, [\jlevel] // r6 = p[j + (1<<level)]
  mul  \r6, \zeta     // r6 *= zeta
  montgomery_reduce_asm \r6, \r4, \q, \qinv, \mask
  ldrh \r4, [\coeffs] // f4 = p[j]
  add  \r5, \r4, \q4  // r5 = r4 + 4*q
  add  \r4, \r4, \r6  // r4 += r6
  sub  \r5, \r6       // r5 -= r6
  barrett_reduce_asm \r5, \r6, \q // \r6 can be used as tmp
.endm

/*************************************************
* NTT Odd Level
**************************************************/
.macro oddlevel coeffs, jlevel, r4, r5, r6, qinv, zeta, q4, mask, q
  butterfly \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
  strh \r5, [\jlevel], #2 // store in p[j + (1<<level)]
  // omit reduction
  strh \r4, [\coeffs], #2 // store in p[j]
.endm

/*************************************************
* NTT Even Level
**************************************************/
.macro evenlevel coeffs, jlevel, r4, r5, r6, qinv, zeta, q4, mask, q
  butterfly \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
  strh \r5, [\jlevel], #2 // store in p[j + (1<<level)]
  barrett_reduce_asm \r4, \r5, \q // reduction (\r5 can be used as tmp)
  strh \r4, [\coeffs], #2 // store in p[j]
.endm

/*************************************************
* NTT Level 7
**************************************************/
.macro level7 coeffs, zetas, jlevel, counter, r4, r5, r6, qinv, zeta, q4, mask, q
  add  \jlevel, \coeffs, #256 // + (1<<level) * #2
  ldrh \zeta, [\zetas], #2    // zeta = zetas[k++]
  mov  \counter, #128         // do 128 iterations
1:
  oddlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
  subs \counter, #1           // counter -= 1
  bne  1b                     // conditional jump to label 1
.endm

/*************************************************
* NTT Level 6
**************************************************/
.macro level6 coeffs, zetas, jlevel, counter, r4, r5, r6, qinv, zeta, q4, mask, q
  add  \jlevel, \coeffs, #128 // + (1<<level) * #2
  ldrh \zeta, [\zetas], #2    // zeta = zetas[k++]
  mov  \counter, #64          // do 64 iterations
1:
  evenlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
  subs \counter, #1           // counter -= 1
  bne  1b                     // conditional jump to label 1
.endm

/*************************************************
* NTT Level 5
**************************************************/
.macro level5 coeffs, zetas, jlevel, counter, r4, r5, r6, qinv, zeta, q4, mask, q
  add  \jlevel, \coeffs, #64  // + (1<<level) * #2
  ldrh \zeta, [\zetas], #2    // zeta = zetas[k++]
  mov  \counter, #32          // do 32 iterations
1:
  oddlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
  subs \counter, #1           // counter -= 1
  bne  1b                     // conditional jump to label 1
.endm

/*************************************************
* NTT Level 4
**************************************************/
.macro level4 coeffs, zetas, counter, jlevel, r4, r5, r6, qinv, zeta, q4, mask, q
  add  \jlevel, \coeffs, #32  // + (1<<level) * #2
  ldrh \zeta, [\zetas], #2    // zeta = zetas[k++]
  mov  \counter, #16          // do 16 iterations
1:
  evenlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
  subs \counter, #1           // counter -= 1
  bne  1b                     // conditional jump to label 1
.endm

/*************************************************
* NTT Level 3 (unrolled)
**************************************************/
.macro level3 coeffs, zetas, jlevel, r4, r5, r6, qinv, zeta, q4, mask, q
  add \jlevel, \coeffs, #16 // + (1<<level) * #2
  ldrh \zeta, [\zetas], #2  // zeta = zetas[k++]
.rept 8
  oddlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
.endr
.endm

/*************************************************
* NTT Level 2 (unrolled)
**************************************************/
.macro level2 coeffs, zetas, jlevel, r4, r5, r6, qinv, zeta, q4, mask, q
  add \jlevel, \coeffs, #8 // + (1<<level) * #2
  ldrh \zeta, [\zetas], #2 // zeta = zetas[k++]
.rept 4
  evenlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
.endr
.endm

/*************************************************
* NTT Level 1 (unrolled)
**************************************************/
.macro level1 coeffs, zetas, jlevel, r4, r5, r6, qinv, zeta, q4, mask, q
  add \jlevel, \coeffs, #4 // + (1<<level) * #2
  ldrh \zeta, [\zetas], #2 // zeta = zetas[k++]
  oddlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
  oddlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
.endm

/*************************************************
* NTT Level 0
**************************************************/
.macro level0 coeffs, zetas, jlevel, r4, r5, r6, qinv, zeta, q4, mask, q
  add \jlevel, \coeffs, #2 // + (1<<level) * #2
  ldrh \zeta, [\zetas], #2 // zeta = zetas[k++] 
  evenlevel \coeffs, \jlevel, \r4, \r5, \r6, \qinv, \zeta, \q4, \mask, \q
.endm

/*************************************************
* Forward NTT, normal to bitreversed order
* - r0: pointer to coefficients
* - r1: pointer to zetas
**************************************************/
.align 4
.global ntt_asm
.type ntt_asm, %function
ntt_asm:
  push    {r2 - r11, lr} 

  /// store parameters in registers ///
  movw r11, #0x1e01 // r11 = q = 7681
  movw r7, #0x1dff // r7 = qinv = 7679
  .equ mask, 0x3ffff
  movw r10, #:lower16:mask
  movt r10, #:upper16:mask // r10 = mask = (1<<18)-1
  movw r9, #0x7804  // r9 = 4*q

  /// level = 7 ///
  level7 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11

  /// level = 6 ///
  sub r0, #256 // reset r0
  level6 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11 
  add r0, #128 // + (1<<level) * #2
  level6 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11

  /// level = 5 ///
  sub r0, #384 // reset r0
  level5 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11 
.rept 3  // do 3 iterations
  add r0, #64 // + (1<<level) * #2
  level5 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11
.endr

  /// level = 4 ///
  sub r0, #448 // reset r0
  level4 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11 
.rept 7 // do 7 iterations
  add r0, #32 // + (1<<level) * #2
  level4 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11
.endr

  /// level = 3 ///
  sub r0, #480  // reset r0
  level3 r0, r1, r3, r4, r5, r6, r7, r8, r9, r10, r11
  mov r2, #15   // do 15 iterations
1:
  add r0, #16   // + (1<<level) * #2
  level3 r0, r1, r3, r4, r5, r6, r7, r8, r9, r10, r11
  subs r2, #1   // counter -= 1
  bne 1b        // conditional jump to label 1

  /// level = 2 ///
  sub r0, #496 // reset r0
  level2 r0, r1, r3, r4, r5, r6, r7, r8, r9, r10, r11
  mov r2, #31  // do 31 iterations
1:
  add r0, #8 // + (1<<level) * #2
  level2 r0, r1, r3, r4, r5, r6, r7, r8, r9, r10, r11
  subs r2, #1 // counter -= 1
  bne 1b // conditional jump to label 1

  /// level = 1 ///
  sub r0, #504 // reset r0
  level1 r0, r1, r3, r4, r5, r6, r7, r8, r9, r10, r11
  mov r2, #63 // do 63 iterations
1:
  add r0, #4 // + (1<<level) * #2
  level1 r0, r1, r3, r4, r5, r6, r7, r8, r9, r10, r11
  subs r2, #1 // counter -= 1
  bne 1b // conditional jump to label 1

  /// level = 0 ///
  sub r0, #508 // reset r0
  level0 r0, r1, r3, r4, r5, r6, r7, r8, r9, r10, r11
  mov r2, #127 // do 127 iterations
1:
  add r0, #2 // + (1<<level) * #2
  level0 r0, r1, r3, r4, r5, r6, r7, r8, r9, r10, r11
  subs r2, #1 // counter -= 1
  bne 1b // conditional jump to label 1

  pop    {r2 - r11, pc}

/*************************************************
* Pointwise multiply (a,b) -> a*b
* - r0: pointer to a
* - r1: pointer to b
**************************************************/
.align 4
.global pointwise_multiply
.type pointwise_multiply, %function
pointwise_multiply:
  push    {r2 - r12, lr}

  /// store parameters in registers ///
  movw r12, #0x1e01 // r5 = q = 7681
  movw r11, #0x1dff // r6 = qinv = 7679
  .equ mask, 0x3ffff
  movw r10, #:lower16:mask
  movt r10, #:upper16:mask // r7 = mask = (1<<18)-1

  /// counter in register 2 ///
  mov r2, #32

  /// main loop: do 8 by 8 ///
1:
.rept 2
  ldm r0, {r3, r5}
  ldm r1!, {r4, r6}
  smulbb r7, r3, r4
  smultt r8, r3, r4
  smulbb r9, r5, r6
  smultt r4, r5, r6
  montgomery_reduce_asm r7, r3, r12, r11, r10
  montgomery_reduce_asm r8, r3, r12, r11, r10
  montgomery_reduce_asm r9, r3, r12, r11, r10
  montgomery_reduce_asm r4, r3, r12, r11, r10
  strh r7, [r0], #2
  strh r8, [r0], #2
  strh r9, [r0], #2
  strh r4, [r0], #2
.endr

  subs r2, #1
  bne  1b

  pop    {r2 - r12, pc}

/*************************************************
* Pointwise add (a,b) => r = barrett_reduce(a+b)
* - r0: pointer to r
* - r1: pointer to a
* - r2: pointer to b
**************************************************/
.align 4
.global pointwise_add
.type pointwise_add, %function
pointwise_add:
  push    {r3 - r12, lr}

  /// store parameters in registers ///
  movw r12, #0x1e01 // r12 = q = 7681
  movw r11, #7
  movt r11, #7      // r11 = 0x70007

  /// counter in register 14 ///
  mov r14, #32

  /// main loop: do 8 by 8 ///
1: 
  ldm r1!, {r3, r5, r7, r9}
  ldm r2!, {r4, r6, r8, r10}
  add r3, r4
  add r5, r6
  add r7, r8
  add r9, r10
  lsr r4, r3, #13
  lsr r6, r5, #13
  lsr r8, r7, #13
  lsr r10, r9, #13
  and r4, r11
  and r6, r11
  and r8, r11
  and r10, r11
  mls r3, r4, r12, r3
  mls r5, r6, r12, r5
  mls r7, r8, r12, r7
  mls r9, r10, r12, r9
  stm r0!, {r3, r5, r7, r9}
  subs r14, #1
  bne 1b

  pop    {r3 - r12, pc}

/*************************************************
* Pointwise sub (a,b) => r = barrett_reduce(3q+a-b)
* - r0: pointer to r
* - r1: pointer to a
* - r2: pointer to b
**************************************************/
.align 4
.global pointwise_sub
.type pointwise_sub, %function
pointwise_sub:
  push    {r3 - r12, lr}

  /// store parameters in registers ///
  movw r12, #0x5a03 
  movt r12, #0x5a03 // 3q on both halfwords
  movw r11, #7
  movt r11, #7      // r11 = 0x70007
  movw r10, #0x1e01 // r10 = q = 7681

  /// counter in register 14 ///
  mov r14, #64

  /// main loop: do 4 by 4 ///
1:
  ldm r1!, {r3, r5}
  ldm r2!, {r4, r6}
  add r3, r12
  add r5, r12
  sub r3, r4
  sub r5, r6
  lsr r4, r3, #13
  lsr r6, r5, #13
  and r4, r11
  and r6, r11
  mls r3, r4, r10, r3
  mls r5, r6, r10, r5
  stm r0!, {r3, r5}
  subs r14, #1
  bne 1b

  pop    {r3 - r12, pc}


/*************************************************
* Inverse NTT Butterfly
**************************************************/
.macro butterfly_inv coeffs, jlevel, r4, r5, r6, omega, q4, mask, q, qinv
  ldrh \r4, [\coeffs] // load a[j]
  ldrh \r5, [\jlevel] // load a[j + (1<<level)] 
  add  \r6, \r4, \r5  // r6 = a[j] + a[j + (1<<level)] 
  sub  \r5, \q4, \r5  // r5 = 4*q - a[j + (1<<level)] 
  add  \r5, \r4       // r5 = a[j] + 4*q - a[j + (1<<level)] 
  mul  \r5, \omega    // r5 = omega*(a[j] + 4*q - a[j + (1<<level)])
  montgomery_reduce_asm \r5, \r4, \q, \qinv, \mask
.endm

/*************************************************
* Inverse NTT Even Level
**************************************************/
.macro evenlevel_inv coeffs, jlevel, r4, r5, r6, omega, q4, mask, q, qinv
  butterfly_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  strh \r5, [\jlevel] // store in p[j + (1<<level)] 
  // omit reduction
  strh \r6, [\coeffs] // store in p[j]
.endm

/*************************************************
* Inverse NTT Odd Level
**************************************************/
.macro oddlevel_inv coeffs, jlevel, r4, r5, r6, omega, q4, mask, q, qinv
  butterfly_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  strh \r5, [\jlevel] // store in p[j + (1<<level)]
  barrett_reduce_asm \r6, \r4, \q // reduction
  strh \r6, [\coeffs] // store in p[j]
.endm

/*************************************************
* Inverse NTT Level 0
**************************************************/
.macro level0_inv coeffs, omegas, jlevel, r3, r4, r5, r6, omega, q4, mask, q, qinv
  add \jlevel, \coeffs, #2    // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  evenlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  mov \r3, #127               // do 127 iterations
1:
  add \coeffs, #4             // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #2    // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  evenlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  subs \r3, #1                // counter -= 1
  bne 1b                      // conditional jump to label 1
  sub \omegas, #256
.endm

/*************************************************
* Inverse NTT Level 1
**************************************************/
.macro level1_inv coeffs, omegas, jlevel, r3, r4, r5, r6, omega, q4, mask, q, qinv
  add \jlevel, \coeffs, #4    // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  mov \r3, #63                // do 63 iterations
1:
  add \coeffs, #8             // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #4    // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  subs \r3, #1                // counter -= 1
  bne 1b                      // conditional jump to label 1
  sub \omegas, #128
.endm

/*************************************************
* Inverse NTT Level 2
**************************************************/
.macro level2_inv coeffs, omegas, jlevel, r3, r4, r5, r6, omega, q4, mask, q, qinv
  add \jlevel, \coeffs, #8    // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  evenlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  mov \r3, #31                // do 31 iterations
1:
  add \coeffs, #16            // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #8    // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  evenlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  subs \r3, #1                // counter -= 1
  bne 1b                      // conditional jump to label 1
  sub \omegas, #64
.endm

/*************************************************
* Inverse NTT Level 3
**************************************************/
.macro level3_inv coeffs, omegas, jlevel, r3, r4, r5, r6, omega, q4, mask, q, qinv
  add \jlevel, \coeffs, #16   // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  mov \r3, #15                // do 15 iterations
1:
  add \coeffs, #32            // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #16   // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  subs \r3, #1                // counter -= 1
  bne 1b                      // conditional jump to label 1
  sub \omegas, #32
.endm

/*************************************************
* Inverse NTT Level 4
**************************************************/
.macro level4_inv coeffs, omegas, jlevel, r3, r4, r5, r6, omega, q4, mask, q, qinv
  add \jlevel, \coeffs, #32   // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  evenlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  mov \r3, #7                 // do 7 iterations
1:
  add \coeffs, #64            // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #32   // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  evenlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  subs \r3, #1                // counter -= 1
  bne 1b                      // conditional jump to label 1
  sub \omegas, #16
.endm

/*************************************************
* Inverse NTT Level 5 (unrolled)
**************************************************/
.macro level5_inv coeffs, omegas, jlevel, r4, r5, r6, omega, q4, mask, q, qinv
  add \jlevel, \coeffs, #64   // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  add \coeffs, #128           // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #64   // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  add \coeffs, #128           // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #64   // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  add \coeffs, #128           // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #64   // + (1<<level) * #2
  ldrh \omega, [\omegas], #2  // omega = omegas[k++]
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv 
  sub \omegas, #8
.endm

/*************************************************
* Inverse NTT Level 6 (unrolled)
**************************************************/
.macro level6_inv coeffs, omegas, jlevel, r4, r5, r6, omega, q4, mask, q, qinv
  add \jlevel, \coeffs, #128  // + (1<<level) * #2
  ldrh \omega, [\omegas]      // omega = omegas[k++]
  evenlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
  add \coeffs, #256           // + 2*(1<<level) * #2
  add \jlevel, \coeffs, #128  // + (1<<level) * #2
  ldrh \omega, [\omegas, #2]  // omega = omegas[k++]
  evenlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
.endm

/*************************************************
* Inverse NTT Level 7
**************************************************/
.macro level7_inv coeffs, jlevel, r4, r5, r6, omega, q4, mask, q, qinv
  add \jlevel, \coeffs, #256 // + (1<<level) * #2
  oddlevel_inv \coeffs, \jlevel, \r4, \r5, \r6, \omega, \q4, \mask, \q, \qinv
.endm

/*************************************************
* Inverse NTT, bitreversed to normal order
* - r0: pointer to coefficients
* - r1: pointer to omegas_inv_bitrev_montgomery
**************************************************/
.align 4
.global invntt_asm
.type invntt_asm, %function
invntt_asm:
  push    {r2 - r12, lr} 

  /// store parameters in registers ///
  movw r11, #0x1e01 // r11 = q = 7681
  movw r12, #0x1dff // r12 = qinv = 7679
  .equ mask, 0x3ffff
  movw r10, #:lower16:mask
  movt r10, #:upper16:mask // r10 = mask = (1<<18)-1
  movw r9, #0x7804 // r9 = 4*q

  mov r7, r0 // save value r0

  /// level = 0 ///
  level0_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12
  
  /// level = 1 (unrolled) ///
  mov r0, r7
  level1_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12
  add r0, r7, #2
  level1_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12

  /// level = 2 (unrolled) ///
  mov r0, r7
  level2_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12
.rept 3
  sub r0, #494
  level2_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12
.endr

  /// level = 3 (unrolled) ///
  mov r0, r7
  level3_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12
.rept 7
  sub r0, #478
  level3_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12
.endr

  /// level = 4 (unrolled) ///
  mov r0, r7
  level4_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12
.rept 15
  sub r0, #446
  level4_inv r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r11, r12
.endr

  /// level = 5 ///
  mov r0, r7
  level5_inv r0, r1, r2, r4, r5, r6, r8, r9, r10, r11, r12
  mov r3, #31 // do 31 iterations
1:
  sub r0, #382
  level5_inv r0, r1, r2, r4, r5, r6, r8, r9, r10, r11, r12
  subs r3, #1 // counter -= 1
  bne 1b      // conditional jump to label 1

  /// level = 6 ///
  mov r0, r7
  level6_inv r0, r1, r2, r4, r5, r6, r8, r9, r10, r11, r12
  mov r3, #63 // do 63 iterations
1:
  sub r0, #254
  level6_inv r0, r1, r2, r4, r5, r6, r8, r9, r10, r11, r12
  subs r3, #1 // counter -= 1
  bne 1b      // conditional jump to label 1

  /// level = 7 ///
  mov r0, r7
  ldrh r8, [r1] // load omega directly (same value everywhere)
  level7_inv r0, r2, r4, r5, r6, r8, r9, r10, r11, r12
  mov r3, #127  // do 127 iterations
1:
  add r0, #2
  level7_inv r0, r2, r4, r5, r6, r8, r9, r10, r11, r12
  subs r3, #1   // counter -= 1
  bne 1b        // conditional jump to label 1

  pop    {r2 - r12, pc}