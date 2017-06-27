
# qhasm: int64 input_0

# qhasm: int64 input_1

# qhasm: int64 input_2

# qhasm: int64 input_3

# qhasm: int64 input_4

# qhasm: int64 input_5

# qhasm: stack64 input_6

# qhasm: stack64 input_7

# qhasm: int64 caller_r11

# qhasm: int64 caller_r12

# qhasm: int64 caller_r13

# qhasm: int64 caller_r14

# qhasm: int64 caller_r15

# qhasm: int64 caller_rbx

# qhasm: int64 caller_rbp

# qhasm: reg256 a0123

# qhasm: reg256 a2301

# qhasm: reg256 b0123

# qhasm: reg256 b2301

# qhasm: reg256 c0123

# qhasm: reg256 c2301

# qhasm: reg256 d0123

# qhasm: reg256 d2301

# qhasm: reg256 e0123

# qhasm: reg256 e2301

# qhasm: reg256 f0123

# qhasm: reg256 f2301

# qhasm: reg256 g0123

# qhasm: reg256 g2301

# qhasm: reg256 h0123

# qhasm: reg256 h2301

# qhasm: reg256 qinv

# qhasm: reg256 q

# qhasm: reg256 c

# qhasm: reg256 t

# qhasm: reg256 omega51

# qhasm: reg256 omega52

# qhasm: reg256 omega53

# qhasm: int64 ii

# qhasm: enter nttlevels5t7
.p2align 5
.global _nttlevels5t7
.global nttlevels5t7
_nttlevels5t7:
nttlevels5t7:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: ii = 128
# asm 1: mov  $128,>ii=int64#4
# asm 2: mov  $128,>ii=%rcx
mov  $128,%rcx

# qhasm: qinv = mem256[_qinv]
# asm 1: vmovdqu _qinv,>qinv=reg256#1
# asm 2: vmovdqu _qinv,>qinv=%ymm0
vmovdqu _qinv,%ymm0

# qhasm: q    = mem256[_q]
# asm 1: vmovdqu _q,>q=reg256#2
# asm 2: vmovdqu _q,>q=%ymm1
vmovdqu _q,%ymm1

# qhasm: omega51 = 4x mem64[input_2 + 24]
# asm 1: vbroadcastsd 24(<input_2=int64#3),>omega51=reg256#3
# asm 2: vbroadcastsd 24(<input_2=%rdx),>omega51=%ymm2
vbroadcastsd 24(%rdx),%ymm2

# qhasm: omega52 = 4x mem64[input_2 + 40]
# asm 1: vbroadcastsd 40(<input_2=int64#3),>omega52=reg256#4
# asm 2: vbroadcastsd 40(<input_2=%rdx),>omega52=%ymm3
vbroadcastsd 40(%rdx),%ymm3

# qhasm: omega53 = 4x mem64[input_2 + 56]
# asm 1: vbroadcastsd 56(<input_2=int64#3),>omega53=reg256#5
# asm 2: vbroadcastsd 56(<input_2=%rdx),>omega53=%ymm4
vbroadcastsd 56(%rdx),%ymm4

# qhasm: looptop:
._looptop:

# qhasm: a0123 = mem256[input_1 +    0]
# asm 1: vmovupd   0(<input_1=int64#2),>a0123=reg256#6
# asm 2: vmovupd   0(<input_1=%rsi),>a0123=%ymm5
vmovupd   0(%rsi),%ymm5

# qhasm: b0123 = mem256[input_1 +  256]
# asm 1: vmovupd   256(<input_1=int64#2),>b0123=reg256#7
# asm 2: vmovupd   256(<input_1=%rsi),>b0123=%ymm6
vmovupd   256(%rsi),%ymm6

# qhasm: c0123 = mem256[input_1 +  512]
# asm 1: vmovupd   512(<input_1=int64#2),>c0123=reg256#8
# asm 2: vmovupd   512(<input_1=%rsi),>c0123=%ymm7
vmovupd   512(%rsi),%ymm7

# qhasm: d0123 = mem256[input_1 +  768]
# asm 1: vmovupd   768(<input_1=int64#2),>d0123=reg256#9
# asm 2: vmovupd   768(<input_1=%rsi),>d0123=%ymm8
vmovupd   768(%rsi),%ymm8

# qhasm: e0123 = mem256[input_1 + 1024]
# asm 1: vmovupd   1024(<input_1=int64#2),>e0123=reg256#10
# asm 2: vmovupd   1024(<input_1=%rsi),>e0123=%ymm9
vmovupd   1024(%rsi),%ymm9

# qhasm: f0123 = mem256[input_1 + 1280]
# asm 1: vmovupd   1280(<input_1=int64#2),>f0123=reg256#11
# asm 2: vmovupd   1280(<input_1=%rsi),>f0123=%ymm10
vmovupd   1280(%rsi),%ymm10

# qhasm: g0123 = mem256[input_1 + 1536]
# asm 1: vmovupd   1536(<input_1=int64#2),>g0123=reg256#12
# asm 2: vmovupd   1536(<input_1=%rsi),>g0123=%ymm11
vmovupd   1536(%rsi),%ymm11

# qhasm: h0123 = mem256[input_1 + 1792]
# asm 1: vmovupd   1792(<input_1=int64#2),>h0123=reg256#13
# asm 2: vmovupd   1792(<input_1=%rsi),>h0123=%ymm12
vmovupd   1792(%rsi),%ymm12

# qhasm: 4x t = approx a0123 - b0123
# asm 1: vsubpd <b0123=reg256#7,<a0123=reg256#6,>t=reg256#14
# asm 2: vsubpd <b0123=%ymm6,<a0123=%ymm5,>t=%ymm13
vsubpd %ymm6,%ymm5,%ymm13

# qhasm: 4x a0123 = approx a0123 + b0123
# asm 1: vaddpd <a0123=reg256#6,<b0123=reg256#7,>a0123=reg256#6
# asm 2: vaddpd <a0123=%ymm5,<b0123=%ymm6,>a0123=%ymm5
vaddpd %ymm5,%ymm6,%ymm5

# qhasm: b0123 = t
# asm 1: vmovapd <t=reg256#14,>b0123=reg256#7
# asm 2: vmovapd <t=%ymm13,>b0123=%ymm6
vmovapd %ymm13,%ymm6

# qhasm: 4x t = approx c0123 - d0123
# asm 1: vsubpd <d0123=reg256#9,<c0123=reg256#8,>t=reg256#14
# asm 2: vsubpd <d0123=%ymm8,<c0123=%ymm7,>t=%ymm13
vsubpd %ymm8,%ymm7,%ymm13

# qhasm: 4x c0123 = approx c0123 + d0123
# asm 1: vaddpd <c0123=reg256#8,<d0123=reg256#9,>c0123=reg256#8
# asm 2: vaddpd <c0123=%ymm7,<d0123=%ymm8,>c0123=%ymm7
vaddpd %ymm7,%ymm8,%ymm7

# qhasm: 4x d0123 = approx t * omega51
# asm 1: vmulpd <t=reg256#14,<omega51=reg256#3,>d0123=reg256#9
# asm 2: vmulpd <t=%ymm13,<omega51=%ymm2,>d0123=%ymm8
vmulpd %ymm13,%ymm2,%ymm8

# qhasm: 4x t = approx e0123 - f0123
# asm 1: vsubpd <f0123=reg256#11,<e0123=reg256#10,>t=reg256#14
# asm 2: vsubpd <f0123=%ymm10,<e0123=%ymm9,>t=%ymm13
vsubpd %ymm10,%ymm9,%ymm13

# qhasm: 4x e0123 = approx e0123 + f0123
# asm 1: vaddpd <e0123=reg256#10,<f0123=reg256#11,>e0123=reg256#10
# asm 2: vaddpd <e0123=%ymm9,<f0123=%ymm10,>e0123=%ymm9
vaddpd %ymm9,%ymm10,%ymm9

# qhasm: 4x f0123 = approx t * omega52
# asm 1: vmulpd <t=reg256#14,<omega52=reg256#4,>f0123=reg256#11
# asm 2: vmulpd <t=%ymm13,<omega52=%ymm3,>f0123=%ymm10
vmulpd %ymm13,%ymm3,%ymm10

# qhasm: 4x t = approx g0123 - h0123
# asm 1: vsubpd <h0123=reg256#13,<g0123=reg256#12,>t=reg256#14
# asm 2: vsubpd <h0123=%ymm12,<g0123=%ymm11,>t=%ymm13
vsubpd %ymm12,%ymm11,%ymm13

# qhasm: 4x g0123 = approx g0123 + h0123
# asm 1: vaddpd <g0123=reg256#12,<h0123=reg256#13,>g0123=reg256#12
# asm 2: vaddpd <g0123=%ymm11,<h0123=%ymm12,>g0123=%ymm11
vaddpd %ymm11,%ymm12,%ymm11

# qhasm: 4x h0123 = approx t * omega53
# asm 1: vmulpd <t=reg256#14,<omega53=reg256#5,>h0123=reg256#13
# asm 2: vmulpd <t=%ymm13,<omega53=%ymm4,>h0123=%ymm12
vmulpd %ymm13,%ymm4,%ymm12

# qhasm: 4x t = approx a0123 - c0123
# asm 1: vsubpd <c0123=reg256#8,<a0123=reg256#6,>t=reg256#14
# asm 2: vsubpd <c0123=%ymm7,<a0123=%ymm5,>t=%ymm13
vsubpd %ymm7,%ymm5,%ymm13

# qhasm: 4x a0123 = approx a0123 + c0123
# asm 1: vaddpd <a0123=reg256#6,<c0123=reg256#8,>a0123=reg256#6
# asm 2: vaddpd <a0123=%ymm5,<c0123=%ymm7,>a0123=%ymm5
vaddpd %ymm5,%ymm7,%ymm5

# qhasm: c0123 = t
# asm 1: vmovapd <t=reg256#14,>c0123=reg256#8
# asm 2: vmovapd <t=%ymm13,>c0123=%ymm7
vmovapd %ymm13,%ymm7

# qhasm: 4x t = approx b0123 - d0123
# asm 1: vsubpd <d0123=reg256#9,<b0123=reg256#7,>t=reg256#14
# asm 2: vsubpd <d0123=%ymm8,<b0123=%ymm6,>t=%ymm13
vsubpd %ymm8,%ymm6,%ymm13

# qhasm: 4x b0123 = approx b0123 + d0123
# asm 1: vaddpd <b0123=reg256#7,<d0123=reg256#9,>b0123=reg256#7
# asm 2: vaddpd <b0123=%ymm6,<d0123=%ymm8,>b0123=%ymm6
vaddpd %ymm6,%ymm8,%ymm6

# qhasm: d0123 = t
# asm 1: vmovapd <t=reg256#14,>d0123=reg256#9
# asm 2: vmovapd <t=%ymm13,>d0123=%ymm8
vmovapd %ymm13,%ymm8

# qhasm: 4x t = approx e0123 - g0123
# asm 1: vsubpd <g0123=reg256#12,<e0123=reg256#10,>t=reg256#14
# asm 2: vsubpd <g0123=%ymm11,<e0123=%ymm9,>t=%ymm13
vsubpd %ymm11,%ymm9,%ymm13

# qhasm: 4x e0123 = approx e0123 + g0123
# asm 1: vaddpd <e0123=reg256#10,<g0123=reg256#12,>e0123=reg256#10
# asm 2: vaddpd <e0123=%ymm9,<g0123=%ymm11,>e0123=%ymm9
vaddpd %ymm9,%ymm11,%ymm9

# qhasm: 4x g0123 = approx t * omega51
# asm 1: vmulpd <t=reg256#14,<omega51=reg256#3,>g0123=reg256#12
# asm 2: vmulpd <t=%ymm13,<omega51=%ymm2,>g0123=%ymm11
vmulpd %ymm13,%ymm2,%ymm11

# qhasm: 4x t = approx f0123 - h0123
# asm 1: vsubpd <h0123=reg256#13,<f0123=reg256#11,>t=reg256#14
# asm 2: vsubpd <h0123=%ymm12,<f0123=%ymm10,>t=%ymm13
vsubpd %ymm12,%ymm10,%ymm13

# qhasm: 4x f0123 = approx f0123 + h0123
# asm 1: vaddpd <f0123=reg256#11,<h0123=reg256#13,>f0123=reg256#11
# asm 2: vaddpd <f0123=%ymm10,<h0123=%ymm12,>f0123=%ymm10
vaddpd %ymm10,%ymm12,%ymm10

# qhasm: 4x h0123 = approx t * omega51
# asm 1: vmulpd <t=reg256#14,<omega51=reg256#3,>h0123=reg256#13
# asm 2: vmulpd <t=%ymm13,<omega51=%ymm2,>h0123=%ymm12
vmulpd %ymm13,%ymm2,%ymm12

# qhasm: 4x t = approx a0123 - e0123
# asm 1: vsubpd <e0123=reg256#10,<a0123=reg256#6,>t=reg256#14
# asm 2: vsubpd <e0123=%ymm9,<a0123=%ymm5,>t=%ymm13
vsubpd %ymm9,%ymm5,%ymm13

# qhasm: 4x a0123 = approx a0123 + e0123
# asm 1: vaddpd <a0123=reg256#6,<e0123=reg256#10,>a0123=reg256#6
# asm 2: vaddpd <a0123=%ymm5,<e0123=%ymm9,>a0123=%ymm5
vaddpd %ymm5,%ymm9,%ymm5

# qhasm: e0123 = t
# asm 1: vmovapd <t=reg256#14,>e0123=reg256#10
# asm 2: vmovapd <t=%ymm13,>e0123=%ymm9
vmovapd %ymm13,%ymm9

# qhasm: 4x t = approx b0123 - f0123
# asm 1: vsubpd <f0123=reg256#11,<b0123=reg256#7,>t=reg256#14
# asm 2: vsubpd <f0123=%ymm10,<b0123=%ymm6,>t=%ymm13
vsubpd %ymm10,%ymm6,%ymm13

# qhasm: 4x b0123 = approx b0123 + f0123
# asm 1: vaddpd <b0123=reg256#7,<f0123=reg256#11,>b0123=reg256#7
# asm 2: vaddpd <b0123=%ymm6,<f0123=%ymm10,>b0123=%ymm6
vaddpd %ymm6,%ymm10,%ymm6

# qhasm: f0123 = t
# asm 1: vmovapd <t=reg256#14,>f0123=reg256#11
# asm 2: vmovapd <t=%ymm13,>f0123=%ymm10
vmovapd %ymm13,%ymm10

# qhasm: 4x t = approx c0123 - g0123
# asm 1: vsubpd <g0123=reg256#12,<c0123=reg256#8,>t=reg256#14
# asm 2: vsubpd <g0123=%ymm11,<c0123=%ymm7,>t=%ymm13
vsubpd %ymm11,%ymm7,%ymm13

# qhasm: 4x c0123 = approx c0123 + g0123
# asm 1: vaddpd <c0123=reg256#8,<g0123=reg256#12,>c0123=reg256#8
# asm 2: vaddpd <c0123=%ymm7,<g0123=%ymm11,>c0123=%ymm7
vaddpd %ymm7,%ymm11,%ymm7

# qhasm: g0123 = t
# asm 1: vmovapd <t=reg256#14,>g0123=reg256#12
# asm 2: vmovapd <t=%ymm13,>g0123=%ymm11
vmovapd %ymm13,%ymm11

# qhasm: 4x t = approx d0123 - h0123
# asm 1: vsubpd <h0123=reg256#13,<d0123=reg256#9,>t=reg256#14
# asm 2: vsubpd <h0123=%ymm12,<d0123=%ymm8,>t=%ymm13
vsubpd %ymm12,%ymm8,%ymm13

# qhasm: 4x d0123 = approx d0123 + h0123
# asm 1: vaddpd <d0123=reg256#9,<h0123=reg256#13,>d0123=reg256#9
# asm 2: vaddpd <d0123=%ymm8,<h0123=%ymm12,>d0123=%ymm8
vaddpd %ymm8,%ymm12,%ymm8

# qhasm: h0123 = t
# asm 1: vmovapd <t=reg256#14,>h0123=reg256#13
# asm 2: vmovapd <t=%ymm13,>h0123=%ymm12
vmovapd %ymm13,%ymm12

# qhasm: 4x c = approx a0123 * qinv
# asm 1: vmulpd <a0123=reg256#6,<qinv=reg256#1,>c=reg256#14
# asm 2: vmulpd <a0123=%ymm5,<qinv=%ymm0,>c=%ymm13
vmulpd %ymm5,%ymm0,%ymm13

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#14,>c=reg256#14
# asm 2: vroundpd $9,<c=%ymm13,>c=%ymm13
vroundpd $9,%ymm13,%ymm13

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#14,>c=reg256#14
# asm 2: vmulpd <q=%ymm1,<c=%ymm13,>c=%ymm13
vmulpd %ymm1,%ymm13,%ymm13

# qhasm: 4x a0123 approx-= c
# asm 1: vsubpd <c=reg256#14,<a0123=reg256#6,>a0123=reg256#6
# asm 2: vsubpd <c=%ymm13,<a0123=%ymm5,>a0123=%ymm5
vsubpd %ymm13,%ymm5,%ymm5

# qhasm: 4x c = approx b0123 * qinv
# asm 1: vmulpd <b0123=reg256#7,<qinv=reg256#1,>c=reg256#14
# asm 2: vmulpd <b0123=%ymm6,<qinv=%ymm0,>c=%ymm13
vmulpd %ymm6,%ymm0,%ymm13

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#14,>c=reg256#14
# asm 2: vroundpd $9,<c=%ymm13,>c=%ymm13
vroundpd $9,%ymm13,%ymm13

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#14,>c=reg256#14
# asm 2: vmulpd <q=%ymm1,<c=%ymm13,>c=%ymm13
vmulpd %ymm1,%ymm13,%ymm13

# qhasm: 4x b0123 approx-= c
# asm 1: vsubpd <c=reg256#14,<b0123=reg256#7,>b0123=reg256#7
# asm 2: vsubpd <c=%ymm13,<b0123=%ymm6,>b0123=%ymm6
vsubpd %ymm13,%ymm6,%ymm6

# qhasm: 4x c = approx c0123 * qinv
# asm 1: vmulpd <c0123=reg256#8,<qinv=reg256#1,>c=reg256#14
# asm 2: vmulpd <c0123=%ymm7,<qinv=%ymm0,>c=%ymm13
vmulpd %ymm7,%ymm0,%ymm13

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#14,>c=reg256#14
# asm 2: vroundpd $9,<c=%ymm13,>c=%ymm13
vroundpd $9,%ymm13,%ymm13

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#14,>c=reg256#14
# asm 2: vmulpd <q=%ymm1,<c=%ymm13,>c=%ymm13
vmulpd %ymm1,%ymm13,%ymm13

# qhasm: 4x c0123 approx-= c
# asm 1: vsubpd <c=reg256#14,<c0123=reg256#8,>c0123=reg256#8
# asm 2: vsubpd <c=%ymm13,<c0123=%ymm7,>c0123=%ymm7
vsubpd %ymm13,%ymm7,%ymm7

# qhasm: 4x c = approx d0123 * qinv
# asm 1: vmulpd <d0123=reg256#9,<qinv=reg256#1,>c=reg256#14
# asm 2: vmulpd <d0123=%ymm8,<qinv=%ymm0,>c=%ymm13
vmulpd %ymm8,%ymm0,%ymm13

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#14,>c=reg256#14
# asm 2: vroundpd $9,<c=%ymm13,>c=%ymm13
vroundpd $9,%ymm13,%ymm13

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#14,>c=reg256#14
# asm 2: vmulpd <q=%ymm1,<c=%ymm13,>c=%ymm13
vmulpd %ymm1,%ymm13,%ymm13

# qhasm: 4x d0123 approx-= c
# asm 1: vsubpd <c=reg256#14,<d0123=reg256#9,>d0123=reg256#9
# asm 2: vsubpd <c=%ymm13,<d0123=%ymm8,>d0123=%ymm8
vsubpd %ymm13,%ymm8,%ymm8

# qhasm: 4x c = approx e0123 * qinv
# asm 1: vmulpd <e0123=reg256#10,<qinv=reg256#1,>c=reg256#14
# asm 2: vmulpd <e0123=%ymm9,<qinv=%ymm0,>c=%ymm13
vmulpd %ymm9,%ymm0,%ymm13

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#14,>c=reg256#14
# asm 2: vroundpd $9,<c=%ymm13,>c=%ymm13
vroundpd $9,%ymm13,%ymm13

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#14,>c=reg256#14
# asm 2: vmulpd <q=%ymm1,<c=%ymm13,>c=%ymm13
vmulpd %ymm1,%ymm13,%ymm13

# qhasm: 4x e0123 approx-= c
# asm 1: vsubpd <c=reg256#14,<e0123=reg256#10,>e0123=reg256#10
# asm 2: vsubpd <c=%ymm13,<e0123=%ymm9,>e0123=%ymm9
vsubpd %ymm13,%ymm9,%ymm9

# qhasm: 4x c = approx f0123 * qinv
# asm 1: vmulpd <f0123=reg256#11,<qinv=reg256#1,>c=reg256#14
# asm 2: vmulpd <f0123=%ymm10,<qinv=%ymm0,>c=%ymm13
vmulpd %ymm10,%ymm0,%ymm13

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#14,>c=reg256#14
# asm 2: vroundpd $9,<c=%ymm13,>c=%ymm13
vroundpd $9,%ymm13,%ymm13

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#14,>c=reg256#14
# asm 2: vmulpd <q=%ymm1,<c=%ymm13,>c=%ymm13
vmulpd %ymm1,%ymm13,%ymm13

# qhasm: 4x f0123 approx-= c
# asm 1: vsubpd <c=reg256#14,<f0123=reg256#11,>f0123=reg256#11
# asm 2: vsubpd <c=%ymm13,<f0123=%ymm10,>f0123=%ymm10
vsubpd %ymm13,%ymm10,%ymm10

# qhasm: 4x c = approx g0123 * qinv
# asm 1: vmulpd <g0123=reg256#12,<qinv=reg256#1,>c=reg256#14
# asm 2: vmulpd <g0123=%ymm11,<qinv=%ymm0,>c=%ymm13
vmulpd %ymm11,%ymm0,%ymm13

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#14,>c=reg256#14
# asm 2: vroundpd $9,<c=%ymm13,>c=%ymm13
vroundpd $9,%ymm13,%ymm13

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#14,>c=reg256#14
# asm 2: vmulpd <q=%ymm1,<c=%ymm13,>c=%ymm13
vmulpd %ymm1,%ymm13,%ymm13

# qhasm: 4x g0123 approx-= c
# asm 1: vsubpd <c=reg256#14,<g0123=reg256#12,>g0123=reg256#12
# asm 2: vsubpd <c=%ymm13,<g0123=%ymm11,>g0123=%ymm11
vsubpd %ymm13,%ymm11,%ymm11

# qhasm: 4x c = approx h0123 * qinv
# asm 1: vmulpd <h0123=reg256#13,<qinv=reg256#1,>c=reg256#14
# asm 2: vmulpd <h0123=%ymm12,<qinv=%ymm0,>c=%ymm13
vmulpd %ymm12,%ymm0,%ymm13

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#14,>c=reg256#14
# asm 2: vroundpd $9,<c=%ymm13,>c=%ymm13
vroundpd $9,%ymm13,%ymm13

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#14,>c=reg256#14
# asm 2: vmulpd <q=%ymm1,<c=%ymm13,>c=%ymm13
vmulpd %ymm1,%ymm13,%ymm13

# qhasm: 4x h0123 approx-= c
# asm 1: vsubpd <c=reg256#14,<h0123=reg256#13,>h0123=reg256#13
# asm 2: vsubpd <c=%ymm13,<h0123=%ymm12,>h0123=%ymm12
vsubpd %ymm13,%ymm12,%ymm12

# qhasm: a0123 = (4x int32)(4x double) a0123,0,0,0,0
# asm 1: vcvtpd2dq <a0123=reg256#6,>a0123=reg256#6dq
# asm 2: vcvtpd2dq <a0123=%ymm5,>a0123=%xmm5
vcvtpd2dq %ymm5,%xmm5

# qhasm: b0123 = (4x int32)(4x double) b0123,0,0,0,0
# asm 1: vcvtpd2dq <b0123=reg256#7,>b0123=reg256#7dq
# asm 2: vcvtpd2dq <b0123=%ymm6,>b0123=%xmm6
vcvtpd2dq %ymm6,%xmm6

# qhasm: c0123 = (4x int32)(4x double) c0123,0,0,0,0
# asm 1: vcvtpd2dq <c0123=reg256#8,>c0123=reg256#8dq
# asm 2: vcvtpd2dq <c0123=%ymm7,>c0123=%xmm7
vcvtpd2dq %ymm7,%xmm7

# qhasm: d0123 = (4x int32)(4x double) d0123,0,0,0,0
# asm 1: vcvtpd2dq <d0123=reg256#9,>d0123=reg256#9dq
# asm 2: vcvtpd2dq <d0123=%ymm8,>d0123=%xmm8
vcvtpd2dq %ymm8,%xmm8

# qhasm: e0123 = (4x int32)(4x double) e0123,0,0,0,0
# asm 1: vcvtpd2dq <e0123=reg256#10,>e0123=reg256#10dq
# asm 2: vcvtpd2dq <e0123=%ymm9,>e0123=%xmm9
vcvtpd2dq %ymm9,%xmm9

# qhasm: f0123 = (4x int32)(4x double) f0123,0,0,0,0
# asm 1: vcvtpd2dq <f0123=reg256#11,>f0123=reg256#11dq
# asm 2: vcvtpd2dq <f0123=%ymm10,>f0123=%xmm10
vcvtpd2dq %ymm10,%xmm10

# qhasm: g0123 = (4x int32)(4x double) g0123,0,0,0,0
# asm 1: vcvtpd2dq <g0123=reg256#12,>g0123=reg256#12dq
# asm 2: vcvtpd2dq <g0123=%ymm11,>g0123=%xmm11
vcvtpd2dq %ymm11,%xmm11

# qhasm: h0123 = (4x int32)(4x double) h0123,0,0,0,0
# asm 1: vcvtpd2dq <h0123=reg256#13,>h0123=reg256#13dq
# asm 2: vcvtpd2dq <h0123=%ymm12,>h0123=%xmm12
vcvtpd2dq %ymm12,%xmm12

# qhasm: mem128[input_0 +    0] = a0123
# asm 1: vmovupd <a0123=reg256#6dq,0(<input_0=int64#1)
# asm 2: vmovupd <a0123=%xmm5,0(<input_0=%rdi)
vmovupd %xmm5,0(%rdi)

# qhasm: mem128[input_0 +  128] = b0123
# asm 1: vmovupd <b0123=reg256#7dq,128(<input_0=int64#1)
# asm 2: vmovupd <b0123=%xmm6,128(<input_0=%rdi)
vmovupd %xmm6,128(%rdi)

# qhasm: mem128[input_0 +  256] = c0123
# asm 1: vmovupd <c0123=reg256#8dq,256(<input_0=int64#1)
# asm 2: vmovupd <c0123=%xmm7,256(<input_0=%rdi)
vmovupd %xmm7,256(%rdi)

# qhasm: mem128[input_0 +  384] = d0123
# asm 1: vmovupd <d0123=reg256#9dq,384(<input_0=int64#1)
# asm 2: vmovupd <d0123=%xmm8,384(<input_0=%rdi)
vmovupd %xmm8,384(%rdi)

# qhasm: mem128[input_0 +  512] = e0123
# asm 1: vmovupd <e0123=reg256#10dq,512(<input_0=int64#1)
# asm 2: vmovupd <e0123=%xmm9,512(<input_0=%rdi)
vmovupd %xmm9,512(%rdi)

# qhasm: mem128[input_0 +  640] = f0123
# asm 1: vmovupd <f0123=reg256#11dq,640(<input_0=int64#1)
# asm 2: vmovupd <f0123=%xmm10,640(<input_0=%rdi)
vmovupd %xmm10,640(%rdi)

# qhasm: mem128[input_0 +  768] = g0123
# asm 1: vmovupd <g0123=reg256#12dq,768(<input_0=int64#1)
# asm 2: vmovupd <g0123=%xmm11,768(<input_0=%rdi)
vmovupd %xmm11,768(%rdi)

# qhasm: mem128[input_0 +  896] = h0123
# asm 1: vmovupd <h0123=reg256#13dq,896(<input_0=int64#1)
# asm 2: vmovupd <h0123=%xmm12,896(<input_0=%rdi)
vmovupd %xmm12,896(%rdi)

# qhasm: input_0 += 16
# asm 1: add  $16,<input_0=int64#1
# asm 2: add  $16,<input_0=%rdi
add  $16,%rdi

# qhasm: input_1 += 32
# asm 1: add  $32,<input_1=int64#2
# asm 2: add  $32,<input_1=%rsi
add  $32,%rsi

# qhasm: unsigned>? ii -= 16
# asm 1: sub  $16,<ii=int64#4
# asm 2: sub  $16,<ii=%rcx
sub  $16,%rcx
# comment:fp stack unchanged by jump

# qhasm: goto looptop if unsigned>
ja ._looptop

# qhasm: return
add %r11,%rsp
ret
