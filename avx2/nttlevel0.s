
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

# qhasm: reg256 t

# qhasm: reg256 neg2

# qhasm: reg256 omega

# qhasm: int64 ii

# qhasm: enter nttlevel0
.p2align 5
.global _nttlevel0
.global nttlevel0
_nttlevel0:
nttlevel0:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: ii = 64
# asm 1: mov  $64,>ii=int64#3
# asm 2: mov  $64,>ii=%rdx
mov  $64,%rdx

# qhasm: neg2 = mem256[_neg2]
# asm 1: vmovdqu _neg2,>neg2=reg256#1
# asm 2: vmovdqu _neg2,>neg2=%ymm0
vmovdqu _neg2,%ymm0

# qhasm: looptop:
._looptop:

# qhasm: omega = mem256[input_1 + 0]
# asm 1: vmovupd   0(<input_1=int64#2),>omega=reg256#2
# asm 2: vmovupd   0(<input_1=%rsi),>omega=%ymm1
vmovupd   0(%rsi),%ymm1

# qhasm: a0123 = mem256[input_0 + 0]
# asm 1: vmovupd   0(<input_0=int64#1),>a0123=reg256#3
# asm 2: vmovupd   0(<input_0=%rdi),>a0123=%ymm2
vmovupd   0(%rdi),%ymm2

# qhasm: 4x t = approx a0123 * neg2
# asm 1: vmulpd <a0123=reg256#3,<neg2=reg256#1,>t=reg256#4
# asm 2: vmulpd <a0123=%ymm2,<neg2=%ymm0,>t=%ymm3
vmulpd %ymm2,%ymm0,%ymm3

# qhasm: a0123[0,1,2,3] = a0123[0]approx+a0123[1],t[0]approx+t[1],a0123[2]approx+a0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#4,<a0123=reg256#3,>a0123=reg256#3
# asm 2: vhaddpd <t=%ymm3,<a0123=%ymm2,>a0123=%ymm2
vhaddpd %ymm3,%ymm2,%ymm2

# qhasm: 4x a0123 approx*= omega
# asm 1: vmulpd <omega=reg256#2,<a0123=reg256#3,>a0123=reg256#2
# asm 2: vmulpd <omega=%ymm1,<a0123=%ymm2,>a0123=%ymm1
vmulpd %ymm1,%ymm2,%ymm1

# qhasm: mem256[input_0 + 0] = a0123
# asm 1: vmovupd   <a0123=reg256#2,0(<input_0=int64#1)
# asm 2: vmovupd   <a0123=%ymm1,0(<input_0=%rdi)
vmovupd   %ymm1,0(%rdi)

# qhasm: input_0 += 32
# asm 1: add  $32,<input_0=int64#1
# asm 2: add  $32,<input_0=%rdi
add  $32,%rdi

# qhasm: input_1 += 32
# asm 1: add  $32,<input_1=int64#2
# asm 2: add  $32,<input_1=%rsi
add  $32,%rsi

# qhasm: unsigned>? ii -= 1
# asm 1: sub  $1,<ii=int64#3
# asm 2: sub  $1,<ii=%rdx
sub  $1,%rdx
# comment:fp stack unchanged by jump

# qhasm: goto looptop if unsigned>
ja ._looptop

# qhasm: return
add %r11,%rsp
ret
