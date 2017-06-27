
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

# qhasm: reg256 qinv

# qhasm: reg256 q

# qhasm: reg256 a0123

# qhasm: reg256 c

# qhasm: int64 ii

# qhasm: enter poly_freeze
.p2align 5
.global _poly_freeze
.global poly_freeze
_poly_freeze:
poly_freeze:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: ii = 256
# asm 1: mov  $256,>ii=int64#3
# asm 2: mov  $256,>ii=%rdx
mov  $256,%rdx

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

# qhasm: a0123 = (4x double)(4x int32)mem128[input_1 +   0]
# asm 1: vcvtdq2pd 0(<input_1=int64#2),>a0123=reg256#3
# asm 2: vcvtdq2pd 0(<input_1=%rsi),>a0123=%ymm2
vcvtdq2pd 0(%rsi),%ymm2

# qhasm: 4x c = approx a0123 * qinv
# asm 1: vmulpd <a0123=reg256#3,<qinv=reg256#1,>c=reg256#4
# asm 2: vmulpd <a0123=%ymm2,<qinv=%ymm0,>c=%ymm3
vmulpd %ymm2,%ymm0,%ymm3

# qhasm: 4x c = floor(c)
# asm 1: vroundpd $9,<c=reg256#4,>c=reg256#4
# asm 2: vroundpd $9,<c=%ymm3,>c=%ymm3
vroundpd $9,%ymm3,%ymm3

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#2,<c=reg256#4,>c=reg256#4
# asm 2: vmulpd <q=%ymm1,<c=%ymm3,>c=%ymm3
vmulpd %ymm1,%ymm3,%ymm3

# qhasm: 4x a0123 approx-= c
# asm 1: vsubpd <c=reg256#4,<a0123=reg256#3,>a0123=reg256#3
# asm 2: vsubpd <c=%ymm3,<a0123=%ymm2,>a0123=%ymm2
vsubpd %ymm3,%ymm2,%ymm2

# qhasm: a0123 = (4x int32)(4x double) a0123,0,0,0,0
# asm 1: vcvtpd2dq <a0123=reg256#3,>a0123=reg256#3dq
# asm 2: vcvtpd2dq <a0123=%ymm2,>a0123=%xmm2
vcvtpd2dq %ymm2,%xmm2

# qhasm: mem128[input_0 +   0] = a0123
# asm 1: vmovupd <a0123=reg256#3dq,0(<input_0=int64#1)
# asm 2: vmovupd <a0123=%xmm2,0(<input_0=%rdi)
vmovupd %xmm2,0(%rdi)

# qhasm: input_0 += 16
# asm 1: add  $16,<input_0=int64#1
# asm 2: add  $16,<input_0=%rdi
add  $16,%rdi

# qhasm: input_1 += 16
# asm 1: add  $16,<input_1=int64#2
# asm 2: add  $16,<input_1=%rsi
add  $16,%rsi

# qhasm: unsigned>? ii -= 4
# asm 1: sub  $4,<ii=int64#3
# asm 2: sub  $4,<ii=%rdx
sub  $4,%rdx
# comment:fp stack unchanged by jump

# qhasm: goto looptop if unsigned>
ja ._looptop

# qhasm: return
add %r11,%rsp
ret
