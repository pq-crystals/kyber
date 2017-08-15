
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
# asm 1: vmovupd _mask11(%rip),>mask11=reg256#1
# asm 2: vmovupd _mask11(%rip),>mask11=%ymm0
vmovupd _mask11(%rip),%ymm0

# qhasm: mask0f = mem256[_mask0f]
# asm 1: vmovupd _mask0f(%rip),>mask0f=reg256#2
# asm 2: vmovupd _mask0f(%rip),>mask0f=%ymm1
vmovupd _mask0f(%rip),%ymm1

# qhasm: q8x   = mem256[_q8x]
# asm 1: vmovupd _q8x(%rip),>q8x=reg256#3
# asm 2: vmovupd _q8x(%rip),>q8x=%ymm2
vmovupd _q8x(%rip),%ymm2

# qhasm: ctr = 256
# asm 1: mov  $256,>ctr=int64#3
# asm 2: mov  $256,>ctr=%rdx
mov  $256,%rdx

# qhasm: looptop:
._looptop:

# qhasm:   r  = mem256[input_1 + 0]
# asm 1: vmovupd   0(<input_1=int64#2),>r=reg256#4
# asm 2: vmovupd   0(<input_1=%rsi),>r=%ymm3
vmovupd   0(%rsi),%ymm3

# qhasm:   a = r & mask11
# asm 1: vpand <r=reg256#4,<mask11=reg256#1,>a=reg256#5
# asm 2: vpand <r=%ymm3,<mask11=%ymm0,>a=%ymm4
vpand %ymm3,%ymm0,%ymm4

# qhasm:   16x r unsigned>>= 1
# asm 1: vpsrlw $1,<r=reg256#4,>r=reg256#4
# asm 2: vpsrlw $1,<r=%ymm3,>r=%ymm3
vpsrlw $1,%ymm3,%ymm3

# qhasm:   t = r & mask11
# asm 1: vpand <r=reg256#4,<mask11=reg256#1,>t=reg256#6
# asm 2: vpand <r=%ymm3,<mask11=%ymm0,>t=%ymm5
vpand %ymm3,%ymm0,%ymm5

# qhasm:   32x a += t
# asm 1: vpaddb <t=reg256#6,<a=reg256#5,>a=reg256#5
# asm 2: vpaddb <t=%ymm5,<a=%ymm4,>a=%ymm4
vpaddb %ymm5,%ymm4,%ymm4

# qhasm:   16x r unsigned>>= 1
# asm 1: vpsrlw $1,<r=reg256#4,>r=reg256#4
# asm 2: vpsrlw $1,<r=%ymm3,>r=%ymm3
vpsrlw $1,%ymm3,%ymm3

# qhasm:   t = r & mask11
# asm 1: vpand <r=reg256#4,<mask11=reg256#1,>t=reg256#6
# asm 2: vpand <r=%ymm3,<mask11=%ymm0,>t=%ymm5
vpand %ymm3,%ymm0,%ymm5

# qhasm:   32x a += t
# asm 1: vpaddb <t=reg256#6,<a=reg256#5,>a=reg256#5
# asm 2: vpaddb <t=%ymm5,<a=%ymm4,>a=%ymm4
vpaddb %ymm5,%ymm4,%ymm4

# qhasm:   16x r unsigned>>= 1
# asm 1: vpsrlw $1,<r=reg256#4,>r=reg256#4
# asm 2: vpsrlw $1,<r=%ymm3,>r=%ymm3
vpsrlw $1,%ymm3,%ymm3

# qhasm:   t = r & mask11
# asm 1: vpand <r=reg256#4,<mask11=reg256#1,>t=reg256#4
# asm 2: vpand <r=%ymm3,<mask11=%ymm0,>t=%ymm3
vpand %ymm3,%ymm0,%ymm3

# qhasm:   32x a += t
# asm 1: vpaddb <t=reg256#4,<a=reg256#5,>a=reg256#4
# asm 2: vpaddb <t=%ymm3,<a=%ymm4,>a=%ymm3
vpaddb %ymm3,%ymm4,%ymm3

# qhasm:   16x b = a unsigned>> 4
# asm 1: vpsrlw $4,<a=reg256#4,>b=reg256#5
# asm 2: vpsrlw $4,<a=%ymm3,>b=%ymm4
vpsrlw $4,%ymm3,%ymm4

# qhasm:   a = a & mask0f
# asm 1: vpand <a=reg256#4,<mask0f=reg256#2,>a=reg256#4
# asm 2: vpand <a=%ymm3,<mask0f=%ymm1,>a=%ymm3
vpand %ymm3,%ymm1,%ymm3

# qhasm:   b = b & mask0f
# asm 1: vpand <b=reg256#5,<mask0f=reg256#2,>b=reg256#5
# asm 2: vpand <b=%ymm4,<mask0f=%ymm1,>b=%ymm4
vpand %ymm4,%ymm1,%ymm4

# qhasm:   32x a -= b
# asm 1: vpsubb <b=reg256#5,<a=reg256#4,>a=reg256#4
# asm 2: vpsubb <b=%ymm4,<a=%ymm3,>a=%ymm3
vpsubb %ymm4,%ymm3,%ymm3

# qhasm:   b = vpmovsxbd a
# asm 1: vpmovsxbd <a=reg256#4dq,>b=reg256#5
# asm 2: vpmovsxbd <a=%xmm3,>b=%ymm4
vpmovsxbd %xmm3,%ymm4

# qhasm:   8x b += q8x
# asm 1: vpaddd <q8x=reg256#3,<b=reg256#5,>b=reg256#5
# asm 2: vpaddd <q8x=%ymm2,<b=%ymm4,>b=%ymm4
vpaddd %ymm2,%ymm4,%ymm4

# qhasm:   mem256[input_0 +  0] = b
# asm 1: vmovupd   <b=reg256#5,0(<input_0=int64#1)
# asm 2: vmovupd   <b=%ymm4,0(<input_0=%rdi)
vmovupd   %ymm4,0(%rdi)

# qhasm:   a[0,1,2,3] = a[1,0,2,3]
# asm 1: vpermilpd $0x9,<a=reg256#4,>a=reg256#4
# asm 2: vpermilpd $0x9,<a=%ymm3,>a=%ymm3
vpermilpd $0x9,%ymm3,%ymm3

# qhasm:   b = vpmovsxbd a
# asm 1: vpmovsxbd <a=reg256#4dq,>b=reg256#5
# asm 2: vpmovsxbd <a=%xmm3,>b=%ymm4
vpmovsxbd %xmm3,%ymm4

# qhasm:   8x b += q8x
# asm 1: vpaddd <q8x=reg256#3,<b=reg256#5,>b=reg256#5
# asm 2: vpaddd <q8x=%ymm2,<b=%ymm4,>b=%ymm4
vpaddd %ymm2,%ymm4,%ymm4

# qhasm:   mem256[input_0 + 32] = b
# asm 1: vmovupd   <b=reg256#5,32(<input_0=int64#1)
# asm 2: vmovupd   <b=%ymm4,32(<input_0=%rdi)
vmovupd   %ymm4,32(%rdi)

# qhasm:   a[0,1,2,3] = a[2,3],a[0,1]
# asm 1: vperm2f128 $0x21,<a=reg256#4,<a=reg256#4,>a=reg256#4
# asm 2: vperm2f128 $0x21,<a=%ymm3,<a=%ymm3,>a=%ymm3
vperm2f128 $0x21,%ymm3,%ymm3,%ymm3

# qhasm:   b = vpmovsxbd a
# asm 1: vpmovsxbd <a=reg256#4dq,>b=reg256#5
# asm 2: vpmovsxbd <a=%xmm3,>b=%ymm4
vpmovsxbd %xmm3,%ymm4

# qhasm:   8x b += q8x
# asm 1: vpaddd <q8x=reg256#3,<b=reg256#5,>b=reg256#5
# asm 2: vpaddd <q8x=%ymm2,<b=%ymm4,>b=%ymm4
vpaddd %ymm2,%ymm4,%ymm4

# qhasm:   mem256[input_0 + 64] = b
# asm 1: vmovupd   <b=reg256#5,64(<input_0=int64#1)
# asm 2: vmovupd   <b=%ymm4,64(<input_0=%rdi)
vmovupd   %ymm4,64(%rdi)

# qhasm:   a[0,1,2,3] = a[1,0,2,3]
# asm 1: vpermilpd $0x9,<a=reg256#4,>a=reg256#4
# asm 2: vpermilpd $0x9,<a=%ymm3,>a=%ymm3
vpermilpd $0x9,%ymm3,%ymm3

# qhasm:   b = vpmovsxbd a
# asm 1: vpmovsxbd <a=reg256#4dq,>b=reg256#4
# asm 2: vpmovsxbd <a=%xmm3,>b=%ymm3
vpmovsxbd %xmm3,%ymm3

# qhasm:   8x b += q8x
# asm 1: vpaddd <q8x=reg256#3,<b=reg256#4,>b=reg256#4
# asm 2: vpaddd <q8x=%ymm2,<b=%ymm3,>b=%ymm3
vpaddd %ymm2,%ymm3,%ymm3

# qhasm:   mem256[input_0 + 96] = b
# asm 1: vmovupd   <b=reg256#4,96(<input_0=int64#1)
# asm 2: vmovupd   <b=%ymm3,96(<input_0=%rdi)
vmovupd   %ymm3,96(%rdi)

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
