
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

# qhasm: reg256 qinv

# qhasm: reg256 q

# qhasm: reg256 c

# qhasm: reg256 t

# qhasm: reg256 neg2

# qhasm: reg256 neg4

# qhasm: reg256 omega0

# qhasm: reg256 omega1

# qhasm: int64 ii

# qhasm: int64 l0w

# qhasm: int64 l1w

# qhasm: enter nttlevels01
.p2align 5
.global _nttlevels01
.global nttlevels01
_nttlevels01:
nttlevels01:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: l0w = input_1
# asm 1: mov  <input_1=int64#2,>l0w=int64#3
# asm 2: mov  <input_1=%rsi,>l0w=%rdx
mov  %rsi,%rdx

# qhasm: l1w = input_1
# asm 1: mov  <input_1=int64#2,>l1w=int64#2
# asm 2: mov  <input_1=%rsi,>l1w=%rsi
mov  %rsi,%rsi

# qhasm: ii = 64
# asm 1: mov  $64,>ii=int64#4
# asm 2: mov  $64,>ii=%rcx
mov  $64,%rcx

# qhasm: neg2 = mem256[_neg2]
# asm 1: vmovdqu _neg2,>neg2=reg256#1
# asm 2: vmovdqu _neg2,>neg2=%ymm0
vmovdqu _neg2,%ymm0

# qhasm: neg4[0,1,2,3] =  neg2[0,0,3,3]
# asm 1: vpermilpd $0xc,<neg2=reg256#1,>neg4=reg256#2
# asm 2: vpermilpd $0xc,<neg2=%ymm0,>neg4=%ymm1
vpermilpd $0xc,%ymm0,%ymm1

# qhasm: qinv = mem256[_qinv]
# asm 1: vmovdqu _qinv,>qinv=reg256#3
# asm 2: vmovdqu _qinv,>qinv=%ymm2
vmovdqu _qinv,%ymm2

# qhasm: q    = mem256[_q]
# asm 1: vmovdqu _q,>q=reg256#4
# asm 2: vmovdqu _q,>q=%ymm3
vmovdqu _q,%ymm3

# qhasm: looptop:
._looptop:

# qhasm: omega0 = mem256[l0w + 0]
# asm 1: vmovupd   0(<l0w=int64#3),>omega0=reg256#5
# asm 2: vmovupd   0(<l0w=%rdx),>omega0=%ymm4
vmovupd   0(%rdx),%ymm4

# qhasm: omega1 = 2x mem128[l1w + 0]
# asm 1: vbroadcastf128 0(<l1w=int64#2),>omega1=reg256#6
# asm 2: vbroadcastf128 0(<l1w=%rsi),>omega1=%ymm5
vbroadcastf128 0(%rsi),%ymm5

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#6,>omega1=reg256#6
# asm 2: vpermilpd $0xc,<omega1=%ymm5,>omega1=%ymm5
vpermilpd $0xc,%ymm5,%ymm5

# qhasm: a0123 = mem256[input_0 + 0]
# asm 1: vmovupd   0(<input_0=int64#1),>a0123=reg256#7
# asm 2: vmovupd   0(<input_0=%rdi),>a0123=%ymm6
vmovupd   0(%rdi),%ymm6

# qhasm: 4x t = approx a0123 * neg2
# asm 1: vmulpd <a0123=reg256#7,<neg2=reg256#1,>t=reg256#8
# asm 2: vmulpd <a0123=%ymm6,<neg2=%ymm0,>t=%ymm7
vmulpd %ymm6,%ymm0,%ymm7

# qhasm: a0123[0,1,2,3] = a0123[0]approx+a0123[1],t[0]approx+t[1],a0123[2]approx+a0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#8,<a0123=reg256#7,>a0123=reg256#7
# asm 2: vhaddpd <t=%ymm7,<a0123=%ymm6,>a0123=%ymm6
vhaddpd %ymm7,%ymm6,%ymm6

# qhasm: 4x a0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#5,<a0123=reg256#7,>a0123=reg256#5
# asm 2: vmulpd <omega0=%ymm4,<a0123=%ymm6,>a0123=%ymm4
vmulpd %ymm4,%ymm6,%ymm4

# qhasm: a2301[0,1,2,3] = a0123[2,3],a0123[0,1]
# asm 1: vperm2f128 $0x21,<a0123=reg256#5,<a0123=reg256#5,>a2301=reg256#7
# asm 2: vperm2f128 $0x21,<a0123=%ymm4,<a0123=%ymm4,>a2301=%ymm6
vperm2f128 $0x21,%ymm4,%ymm4,%ymm6

# qhasm: 4x a0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vmulpd <neg4=%ymm1,<a0123=%ymm4,>a0123=%ymm4
vmulpd %ymm1,%ymm4,%ymm4

# qhasm: 4x a0123 approx+= a2301
# asm 1: vaddpd <a2301=reg256#7,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vaddpd <a2301=%ymm6,<a0123=%ymm4,>a0123=%ymm4
vaddpd %ymm6,%ymm4,%ymm4

# qhasm: 4x a0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#6,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vmulpd <omega1=%ymm5,<a0123=%ymm4,>a0123=%ymm4
vmulpd %ymm5,%ymm4,%ymm4

# qhasm: 4x c = approx a0123 * qinv
# asm 1: vmulpd <a0123=reg256#5,<qinv=reg256#3,>c=reg256#6
# asm 2: vmulpd <a0123=%ymm4,<qinv=%ymm2,>c=%ymm5
vmulpd %ymm4,%ymm2,%ymm5

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#6,>c=reg256#6
# asm 2: vroundpd $8,<c=%ymm5,>c=%ymm5
vroundpd $8,%ymm5,%ymm5

# qhasm: 4x c approx*= q
# asm 1: vmulpd <q=reg256#4,<c=reg256#6,>c=reg256#6
# asm 2: vmulpd <q=%ymm3,<c=%ymm5,>c=%ymm5
vmulpd %ymm3,%ymm5,%ymm5

# qhasm: 4x a0123 approx-= c
# asm 1: vsubpd <c=reg256#6,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vsubpd <c=%ymm5,<a0123=%ymm4,>a0123=%ymm4
vsubpd %ymm5,%ymm4,%ymm4

# qhasm: mem256[input_0 + 0] = a0123
# asm 1: vmovupd   <a0123=reg256#5,0(<input_0=int64#1)
# asm 2: vmovupd   <a0123=%ymm4,0(<input_0=%rdi)
vmovupd   %ymm4,0(%rdi)

# qhasm: input_0 += 32
# asm 1: add  $32,<input_0=int64#1
# asm 2: add  $32,<input_0=%rdi
add  $32,%rdi

# qhasm: l0w += 32
# asm 1: add  $32,<l0w=int64#3
# asm 2: add  $32,<l0w=%rdx
add  $32,%rdx

# qhasm: l1w += 16
# asm 1: add  $16,<l1w=int64#2
# asm 2: add  $16,<l1w=%rsi
add  $16,%rsi

# qhasm: unsigned>? ii -= 1
# asm 1: sub  $1,<ii=int64#4
# asm 2: sub  $1,<ii=%rcx
sub  $1,%rcx
# comment:fp stack unchanged by jump

# qhasm: goto looptop if unsigned>
ja ._looptop

# qhasm: return
add %r11,%rsp
ret
