
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

# qhasm: reg256 a0

# qhasm: reg256 a1

# qhasm: reg256 a2

# qhasm: reg256 b0

# qhasm: reg256 b1

# qhasm: reg256 b2

# qhasm: reg256 r

# qhasm: reg256 t

# qhasm: reg256 c

# qhasm: reg256 q

# qhasm: reg256 qinv

# qhasm: int64 ii

# qhasm: enter polyvec_pointwise_acc
.p2align 5
.global _polyvec_pointwise_acc
.global polyvec_pointwise_acc
_polyvec_pointwise_acc:
polyvec_pointwise_acc:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: ii = 256
# asm 1: mov  $256,>ii=int64#4
# asm 2: mov  $256,>ii=%rcx
mov  $256,%rcx

# qhasm: qinv = mem256[_qinv]
# asm 1: vmovdqu _qinv,>qinv=reg256#1
# asm 2: vmovdqu _qinv,>qinv=%ymm0
vmovdqu _qinv,%ymm0

# qhasm: q    = mem256[_q]
# asm 1: vmovdqu _q,>q=reg256#2
# asm 2: vmovdqu _q,>q=%ymm1
vmovdqu _q,%ymm1

# qhasm: looptop:
._looptop:

# qhasm: a0 = (4x double)(4x int32)mem128[input_1 +    0]
# asm 1: vcvtdq2pd 0(<input_1=int64#2),>a0=reg256#3
# asm 2: vcvtdq2pd 0(<input_1=%rsi),>a0=%ymm2
vcvtdq2pd 0(%rsi),%ymm2

# qhasm: a1 = (4x double)(4x int32)mem128[input_1 + 1024]
# asm 1: vcvtdq2pd 1024(<input_1=int64#2),>a1=reg256#4
# asm 2: vcvtdq2pd 1024(<input_1=%rsi),>a1=%ymm3
vcvtdq2pd 1024(%rsi),%ymm3

# qhasm: a2 = (4x double)(4x int32)mem128[input_1 + 2048]
# asm 1: vcvtdq2pd 2048(<input_1=int64#2),>a2=reg256#5
# asm 2: vcvtdq2pd 2048(<input_1=%rsi),>a2=%ymm4
vcvtdq2pd 2048(%rsi),%ymm4

# qhasm: b0 = (4x double)(4x int32)mem128[input_2 +    0]
# asm 1: vcvtdq2pd 0(<input_2=int64#3),>b0=reg256#6
# asm 2: vcvtdq2pd 0(<input_2=%rdx),>b0=%ymm5
vcvtdq2pd 0(%rdx),%ymm5

# qhasm: b1 = (4x double)(4x int32)mem128[input_2 + 1024]
# asm 1: vcvtdq2pd 1024(<input_2=int64#3),>b1=reg256#7
# asm 2: vcvtdq2pd 1024(<input_2=%rdx),>b1=%ymm6
vcvtdq2pd 1024(%rdx),%ymm6

# qhasm: b2 = (4x double)(4x int32)mem128[input_2 + 2048]
# asm 1: vcvtdq2pd 2048(<input_2=int64#3),>b2=reg256#8
# asm 2: vcvtdq2pd 2048(<input_2=%rdx),>b2=%ymm7
vcvtdq2pd 2048(%rdx),%ymm7

# qhasm: 4x r = approx a0 * b0
# asm 1: vmulpd <a0=reg256#3,<b0=reg256#6,>r=reg256#3
# asm 2: vmulpd <a0=%ymm2,<b0=%ymm5,>r=%ymm2
vmulpd %ymm2,%ymm5,%ymm2

# qhasm: 4x t = approx a1 * b1
# asm 1: vmulpd <a1=reg256#4,<b1=reg256#7,>t=reg256#4
# asm 2: vmulpd <a1=%ymm3,<b1=%ymm6,>t=%ymm3
vmulpd %ymm3,%ymm6,%ymm3

# qhasm: 4x r approx+= t
# asm 1: vaddpd <t=reg256#4,<r=reg256#3,>r=reg256#3
# asm 2: vaddpd <t=%ymm3,<r=%ymm2,>r=%ymm2
vaddpd %ymm3,%ymm2,%ymm2

# qhasm: 4x t = approx a2 * b2
# asm 1: vmulpd <a2=reg256#5,<b2=reg256#8,>t=reg256#4
# asm 2: vmulpd <a2=%ymm4,<b2=%ymm7,>t=%ymm3
vmulpd %ymm4,%ymm7,%ymm3

# qhasm: 4x r approx+= t
# asm 1: vaddpd <t=reg256#4,<r=reg256#3,>r=reg256#3
# asm 2: vaddpd <t=%ymm3,<r=%ymm2,>r=%ymm2
vaddpd %ymm3,%ymm2,%ymm2

# qhasm: 4x c = approx r * qinv
# asm 1: vmulpd <r=reg256#3,<qinv=reg256#1,>c=reg256#4
# asm 2: vmulpd <r=%ymm2,<qinv=%ymm0,>c=%ymm3
vmulpd %ymm2,%ymm0,%ymm3

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#4,>c=reg256#4
# asm 2: vroundpd $9,<c=%ymm3,>c=%ymm3
vroundpd $9,%ymm3,%ymm3

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#4,>c=reg256#4
# asm 2: vmulpd <q=%ymm1,<c=%ymm3,>c=%ymm3
vmulpd %ymm1,%ymm3,%ymm3

# qhasm: 4x r approx-= c
# asm 1: vsubpd <c=reg256#4,<r=reg256#3,>r=reg256#3
# asm 2: vsubpd <c=%ymm3,<r=%ymm2,>r=%ymm2
vsubpd %ymm3,%ymm2,%ymm2

# qhasm: r = (4x int32)(4x double) r,0,0,0,0
# asm 1: vcvtpd2dq <r=reg256#3,>r=reg256#3dq
# asm 2: vcvtpd2dq <r=%ymm2,>r=%xmm2
vcvtpd2dq %ymm2,%xmm2

# qhasm: mem128[input_0 +   0] = r
# asm 1: vmovupd <r=reg256#3dq,0(<input_0=int64#1)
# asm 2: vmovupd <r=%xmm2,0(<input_0=%rdi)
vmovupd %xmm2,0(%rdi)

# qhasm: input_0 += 16
# asm 1: add  $16,<input_0=int64#1
# asm 2: add  $16,<input_0=%rdi
add  $16,%rdi

# qhasm: input_1 += 16
# asm 1: add  $16,<input_1=int64#2
# asm 2: add  $16,<input_1=%rsi
add  $16,%rsi

# qhasm: input_2 += 16
# asm 1: add  $16,<input_2=int64#3
# asm 2: add  $16,<input_2=%rdx
add  $16,%rdx

# qhasm: unsigned>? ii -= 4
# asm 1: sub  $4,<ii=int64#4
# asm 2: sub  $4,<ii=%rcx
sub  $4,%rcx
# comment:fp stack unchanged by jump

# qhasm: goto looptop if unsigned>
ja ._looptop

# qhasm: return
add %r11,%rsp
ret
