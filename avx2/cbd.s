
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

# qhasm: reg256 r

# qhasm: reg256 a

# qhasm: reg256 b

# qhasm: reg256 t

# qhasm: reg256 mask11

# qhasm: reg256 mask0f

# qhasm: reg256 q8x

# qhasm: int64 ctr

# qhasm: enter cbd
.p2align 5
.global _cbd
.global cbd
_cbd:
cbd:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: mask11 = mem256[_mask11]
# asm 1: vmovdqu _mask11,>mask11=reg256#1
# asm 2: vmovdqu _mask11,>mask11=%ymm0
vmovdqu _mask11,%ymm0

# qhasm: mask0f = mem256[_mask0f]
# asm 1: vmovdqu _mask0f,>mask0f=reg256#2
# asm 2: vmovdqu _mask0f,>mask0f=%ymm1
vmovdqu _mask0f,%ymm1

# qhasm: q8x   = mem256[_q8x]
# asm 1: vmovdqu _q8x,>q8x=reg256#3
# asm 2: vmovdqu _q8x,>q8x=%ymm2
vmovdqu _q8x,%ymm2

# qhasm: ctr = 256
# asm 1: mov  $256,>ctr=int64#3
# asm 2: mov  $256,>ctr=%rdx
mov  $256,%rdx

# qhasm: looptop:
._looptop:

# qhasm:   r  = mem256[input_1 + 0]
# asm 1: vmovupd   0(<input_1=int64#2),>r=reg256#1
# asm 2: vmovupd   0(<input_1=%rsi),>r=%ymm0
vmovupd   0(%rsi),%ymm0

# qhasm:   a = r & _mask11
# asm 1: vpand _mask11,<r=reg256#1,>a=reg256#2
# asm 2: vpand _mask11,<r=%ymm0,>a=%ymm1
vpand _mask11,%ymm0,%ymm1

# qhasm:   16x r unsigned>>= 1
# asm 1: vpsrlw $1,<r=reg256#1,>r=reg256#1
# asm 2: vpsrlw $1,<r=%ymm0,>r=%ymm0
vpsrlw $1,%ymm0,%ymm0

# qhasm:   t = r & _mask11
# asm 1: vpand _mask11,<r=reg256#1,>t=reg256#4
# asm 2: vpand _mask11,<r=%ymm0,>t=%ymm3
vpand _mask11,%ymm0,%ymm3

# qhasm:   32x a += t
# asm 1: vpaddb <t=reg256#4,<a=reg256#2,>a=reg256#2
# asm 2: vpaddb <t=%ymm3,<a=%ymm1,>a=%ymm1
vpaddb %ymm3,%ymm1,%ymm1

# qhasm:   16x r unsigned>>= 1
# asm 1: vpsrlw $1,<r=reg256#1,>r=reg256#1
# asm 2: vpsrlw $1,<r=%ymm0,>r=%ymm0
vpsrlw $1,%ymm0,%ymm0

# qhasm:   t = r & _mask11
# asm 1: vpand _mask11,<r=reg256#1,>t=reg256#4
# asm 2: vpand _mask11,<r=%ymm0,>t=%ymm3
vpand _mask11,%ymm0,%ymm3

# qhasm:   32x a += t
# asm 1: vpaddb <t=reg256#4,<a=reg256#2,>a=reg256#2
# asm 2: vpaddb <t=%ymm3,<a=%ymm1,>a=%ymm1
vpaddb %ymm3,%ymm1,%ymm1

# qhasm:   16x r unsigned>>= 1
# asm 1: vpsrlw $1,<r=reg256#1,>r=reg256#1
# asm 2: vpsrlw $1,<r=%ymm0,>r=%ymm0
vpsrlw $1,%ymm0,%ymm0

# qhasm:   t = r & _mask11
# asm 1: vpand _mask11,<r=reg256#1,>t=reg256#1
# asm 2: vpand _mask11,<r=%ymm0,>t=%ymm0
vpand _mask11,%ymm0,%ymm0

# qhasm:   32x a += t
# asm 1: vpaddb <t=reg256#1,<a=reg256#2,>a=reg256#1
# asm 2: vpaddb <t=%ymm0,<a=%ymm1,>a=%ymm0
vpaddb %ymm0,%ymm1,%ymm0

# qhasm:   16x b = a unsigned>> 4
# asm 1: vpsrlw $4,<a=reg256#1,>b=reg256#2
# asm 2: vpsrlw $4,<a=%ymm0,>b=%ymm1
vpsrlw $4,%ymm0,%ymm1

# qhasm:   a = a & _mask0f
# asm 1: vpand _mask0f,<a=reg256#1,>a=reg256#1
# asm 2: vpand _mask0f,<a=%ymm0,>a=%ymm0
vpand _mask0f,%ymm0,%ymm0

# qhasm:   b = b & _mask0f
# asm 1: vpand _mask0f,<b=reg256#2,>b=reg256#2
# asm 2: vpand _mask0f,<b=%ymm1,>b=%ymm1
vpand _mask0f,%ymm1,%ymm1

# qhasm:   32x a -= b
# asm 1: vpsubb <b=reg256#2,<a=reg256#1,>a=reg256#1
# asm 2: vpsubb <b=%ymm1,<a=%ymm0,>a=%ymm0
vpsubb %ymm1,%ymm0,%ymm0

# qhasm:   b = vpmovsxbd a
# asm 1: vpmovsxbd <a=reg256#1dq,>b=reg256#2
# asm 2: vpmovsxbd <a=%xmm0,>b=%ymm1
vpmovsxbd %xmm0,%ymm1

# qhasm:   8x b += q8x
# asm 1: vpaddd <q8x=reg256#3,<b=reg256#2,>b=reg256#2
# asm 2: vpaddd <q8x=%ymm2,<b=%ymm1,>b=%ymm1
vpaddd %ymm2,%ymm1,%ymm1

# qhasm:   mem256[input_0 +  0] = b
# asm 1: vmovupd   <b=reg256#2,0(<input_0=int64#1)
# asm 2: vmovupd   <b=%ymm1,0(<input_0=%rdi)
vmovupd   %ymm1,0(%rdi)

# qhasm:   a[0,1,2,3] = a[1,0,2,3]
# asm 1: vpermilpd $0x9,<a=reg256#1,>a=reg256#1
# asm 2: vpermilpd $0x9,<a=%ymm0,>a=%ymm0
vpermilpd $0x9,%ymm0,%ymm0

# qhasm:   b = vpmovsxbd a
# asm 1: vpmovsxbd <a=reg256#1dq,>b=reg256#2
# asm 2: vpmovsxbd <a=%xmm0,>b=%ymm1
vpmovsxbd %xmm0,%ymm1

# qhasm:   8x b += q8x
# asm 1: vpaddd <q8x=reg256#3,<b=reg256#2,>b=reg256#2
# asm 2: vpaddd <q8x=%ymm2,<b=%ymm1,>b=%ymm1
vpaddd %ymm2,%ymm1,%ymm1

# qhasm:   mem256[input_0 + 32] = b
# asm 1: vmovupd   <b=reg256#2,32(<input_0=int64#1)
# asm 2: vmovupd   <b=%ymm1,32(<input_0=%rdi)
vmovupd   %ymm1,32(%rdi)

# qhasm:   a[0,1,2,3] = a[2,3],a[0,1]
# asm 1: vperm2f128 $0x21,<a=reg256#1,<a=reg256#1,>a=reg256#1
# asm 2: vperm2f128 $0x21,<a=%ymm0,<a=%ymm0,>a=%ymm0
vperm2f128 $0x21,%ymm0,%ymm0,%ymm0

# qhasm:   b = vpmovsxbd a
# asm 1: vpmovsxbd <a=reg256#1dq,>b=reg256#2
# asm 2: vpmovsxbd <a=%xmm0,>b=%ymm1
vpmovsxbd %xmm0,%ymm1

# qhasm:   8x b += q8x
# asm 1: vpaddd <q8x=reg256#3,<b=reg256#2,>b=reg256#2
# asm 2: vpaddd <q8x=%ymm2,<b=%ymm1,>b=%ymm1
vpaddd %ymm2,%ymm1,%ymm1

# qhasm:   mem256[input_0 + 64] = b
# asm 1: vmovupd   <b=reg256#2,64(<input_0=int64#1)
# asm 2: vmovupd   <b=%ymm1,64(<input_0=%rdi)
vmovupd   %ymm1,64(%rdi)

# qhasm:   a[0,1,2,3] = a[1,0,2,3]
# asm 1: vpermilpd $0x9,<a=reg256#1,>a=reg256#1
# asm 2: vpermilpd $0x9,<a=%ymm0,>a=%ymm0
vpermilpd $0x9,%ymm0,%ymm0

# qhasm:   b = vpmovsxbd a
# asm 1: vpmovsxbd <a=reg256#1dq,>b=reg256#1
# asm 2: vpmovsxbd <a=%xmm0,>b=%ymm0
vpmovsxbd %xmm0,%ymm0

# qhasm:   8x b += q8x
# asm 1: vpaddd <q8x=reg256#3,<b=reg256#1,>b=reg256#1
# asm 2: vpaddd <q8x=%ymm2,<b=%ymm0,>b=%ymm0
vpaddd %ymm2,%ymm0,%ymm0

# qhasm:   mem256[input_0 + 96] = b
# asm 1: vmovupd   <b=reg256#1,96(<input_0=int64#1)
# asm 2: vmovupd   <b=%ymm0,96(<input_0=%rdi)
vmovupd   %ymm0,96(%rdi)

# qhasm:   input_0 += 128
# asm 1: add  $128,<input_0=int64#1
# asm 2: add  $128,<input_0=%rdi
add  $128,%rdi

# qhasm:   input_1 += 32
# asm 1: add  $32,<input_1=int64#2
# asm 2: add  $32,<input_1=%rsi
add  $32,%rsi

# qhasm: unsigned>? ctr -= 32
# asm 1: sub  $32,<ctr=int64#3
# asm 2: sub  $32,<ctr=%rdx
sub  $32,%rdx
# comment:fp stack unchanged by jump

# qhasm: goto looptop if unsigned>
ja ._looptop

# qhasm: return
add %r11,%rsp
ret
