
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

# qhasm: reg256 neg2

# qhasm: reg256 neg4

# qhasm: reg256 omega0

# qhasm: reg256 omega1

# qhasm: reg256 omega2

# qhasm: reg256 omega3

# qhasm: reg256 omega4

# qhasm: int64 ii

# qhasm: int64 l0w

# qhasm: int64 l1w

# qhasm: int64 l2w

# qhasm: int64 l3w

# qhasm: int64 l4w

# qhasm: enter nttlevels0t4
.p2align 5
.global _nttlevels0t4
.global nttlevels0t4
_nttlevels0t4:
nttlevels0t4:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: l0w = input_2
# asm 1: mov  <input_2=int64#3,>l0w=int64#4
# asm 2: mov  <input_2=%rdx,>l0w=%rcx
mov  %rdx,%rcx

# qhasm: l1w = input_2
# asm 1: mov  <input_2=int64#3,>l1w=int64#5
# asm 2: mov  <input_2=%rdx,>l1w=%r8
mov  %rdx,%r8

# qhasm: l2w = input_2
# asm 1: mov  <input_2=int64#3,>l2w=int64#6
# asm 2: mov  <input_2=%rdx,>l2w=%r9
mov  %rdx,%r9

# qhasm: l3w = input_2
# asm 1: mov  <input_2=int64#3,>l3w=int64#7
# asm 2: mov  <input_2=%rdx,>l3w=%rax
mov  %rdx,%rax

# qhasm: l4w = input_2
# asm 1: mov  <input_2=int64#3,>l4w=int64#3
# asm 2: mov  <input_2=%rdx,>l4w=%rdx
mov  %rdx,%rdx

# qhasm: ii = 128
# asm 1: mov  $128,>ii=int64#8
# asm 2: mov  $128,>ii=%r10
mov  $128,%r10

# qhasm: neg2 = mem256[_neg2]
# asm 1: vmovupd _neg2(%rip),>neg2=reg256#1
# asm 2: vmovupd _neg2(%rip),>neg2=%ymm0
vmovupd _neg2(%rip),%ymm0

# qhasm: neg4[0,1,2,3] =  neg2[0,0,3,3]
# asm 1: vpermilpd $0xc,<neg2=reg256#1,>neg4=reg256#2
# asm 2: vpermilpd $0xc,<neg2=%ymm0,>neg4=%ymm1
vpermilpd $0xc,%ymm0,%ymm1

# qhasm: qinv = mem256[_qinv]
# asm 1: vmovupd _qinv(%rip),>qinv=reg256#3
# asm 2: vmovupd _qinv(%rip),>qinv=%ymm2
vmovupd _qinv(%rip),%ymm2

# qhasm: q    = mem256[_q]
# asm 1: vmovupd _q(%rip),>q=reg256#4
# asm 2: vmovupd _q(%rip),>q=%ymm3
vmovupd _q(%rip),%ymm3

# qhasm: looptop:
._looptop:

# qhasm: a0123 = (4x double)(4x int32)mem128[input_1 +   0]
# asm 1: vcvtdq2pd 0(<input_1=int64#2),>a0123=reg256#5
# asm 2: vcvtdq2pd 0(<input_1=%rsi),>a0123=%ymm4
vcvtdq2pd 0(%rsi),%ymm4

# qhasm: b0123 = (4x double)(4x int32)mem128[input_1 +  16]
# asm 1: vcvtdq2pd 16(<input_1=int64#2),>b0123=reg256#6
# asm 2: vcvtdq2pd 16(<input_1=%rsi),>b0123=%ymm5
vcvtdq2pd 16(%rsi),%ymm5

# qhasm: c0123 = (4x double)(4x int32)mem128[input_1 +  32]
# asm 1: vcvtdq2pd 32(<input_1=int64#2),>c0123=reg256#7
# asm 2: vcvtdq2pd 32(<input_1=%rsi),>c0123=%ymm6
vcvtdq2pd 32(%rsi),%ymm6

# qhasm: d0123 = (4x double)(4x int32)mem128[input_1 +  48]
# asm 1: vcvtdq2pd 48(<input_1=int64#2),>d0123=reg256#8
# asm 2: vcvtdq2pd 48(<input_1=%rsi),>d0123=%ymm7
vcvtdq2pd 48(%rsi),%ymm7

# qhasm: e0123 = (4x double)(4x int32)mem128[input_1 +  64]
# asm 1: vcvtdq2pd 64(<input_1=int64#2),>e0123=reg256#9
# asm 2: vcvtdq2pd 64(<input_1=%rsi),>e0123=%ymm8
vcvtdq2pd 64(%rsi),%ymm8

# qhasm: f0123 = (4x double)(4x int32)mem128[input_1 +  80]
# asm 1: vcvtdq2pd 80(<input_1=int64#2),>f0123=reg256#10
# asm 2: vcvtdq2pd 80(<input_1=%rsi),>f0123=%ymm9
vcvtdq2pd 80(%rsi),%ymm9

# qhasm: g0123 = (4x double)(4x int32)mem128[input_1 +  96]
# asm 1: vcvtdq2pd 96(<input_1=int64#2),>g0123=reg256#11
# asm 2: vcvtdq2pd 96(<input_1=%rsi),>g0123=%ymm10
vcvtdq2pd 96(%rsi),%ymm10

# qhasm: h0123 = (4x double)(4x int32)mem128[input_1 + 112]
# asm 1: vcvtdq2pd 112(<input_1=int64#2),>h0123=reg256#12
# asm 2: vcvtdq2pd 112(<input_1=%rsi),>h0123=%ymm11
vcvtdq2pd 112(%rsi),%ymm11

# qhasm: omega0 = mem256[l0w + 0]
# asm 1: vmovupd   0(<l0w=int64#4),>omega0=reg256#13
# asm 2: vmovupd   0(<l0w=%rcx),>omega0=%ymm12
vmovupd   0(%rcx),%ymm12

# qhasm: omega1 = 2x mem128[l1w + 0]
# asm 1: vbroadcastf128 0(<l1w=int64#5),>omega1=reg256#14
# asm 2: vbroadcastf128 0(<l1w=%r8),>omega1=%ymm13
vbroadcastf128 0(%r8),%ymm13

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#14,>omega1=reg256#14
# asm 2: vpermilpd $0xc,<omega1=%ymm13,>omega1=%ymm13
vpermilpd $0xc,%ymm13,%ymm13

# qhasm: 4x t = approx a0123 * neg2
# asm 1: vmulpd <a0123=reg256#5,<neg2=reg256#1,>t=reg256#15
# asm 2: vmulpd <a0123=%ymm4,<neg2=%ymm0,>t=%ymm14
vmulpd %ymm4,%ymm0,%ymm14

# qhasm: a0123[0,1,2,3] = a0123[0]approx+a0123[1],t[0]approx+t[1],a0123[2]approx+a0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#15,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vhaddpd <t=%ymm14,<a0123=%ymm4,>a0123=%ymm4
vhaddpd %ymm14,%ymm4,%ymm4

# qhasm: 4x a0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#13,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vmulpd <omega0=%ymm12,<a0123=%ymm4,>a0123=%ymm4
vmulpd %ymm12,%ymm4,%ymm4

# qhasm: a2301[0,1,2,3] = a0123[2,3],a0123[0,1]
# asm 1: vperm2f128 $0x21,<a0123=reg256#5,<a0123=reg256#5,>a2301=reg256#13
# asm 2: vperm2f128 $0x21,<a0123=%ymm4,<a0123=%ymm4,>a2301=%ymm12
vperm2f128 $0x21,%ymm4,%ymm4,%ymm12

# qhasm: 4x a0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vmulpd <neg4=%ymm1,<a0123=%ymm4,>a0123=%ymm4
vmulpd %ymm1,%ymm4,%ymm4

# qhasm: 4x a0123 approx+= a2301
# asm 1: vaddpd <a2301=reg256#13,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vaddpd <a2301=%ymm12,<a0123=%ymm4,>a0123=%ymm4
vaddpd %ymm12,%ymm4,%ymm4

# qhasm: 4x a0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#14,<a0123=reg256#5,>a0123=reg256#5
# asm 2: vmulpd <omega1=%ymm13,<a0123=%ymm4,>a0123=%ymm4
vmulpd %ymm13,%ymm4,%ymm4

# qhasm: 4x c = approx a0123 * qinv
# asm 1: vmulpd <a0123=reg256#5,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <a0123=%ymm4,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm4,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x a0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<a0123=reg256#5
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<a0123=%ymm4
vfnmadd231pd %ymm12,%ymm3,%ymm4

# qhasm: omega0 = mem256[l0w + 32]
# asm 1: vmovupd   32(<l0w=int64#4),>omega0=reg256#13
# asm 2: vmovupd   32(<l0w=%rcx),>omega0=%ymm12
vmovupd   32(%rcx),%ymm12

# qhasm: omega1 = 2x mem128[l1w + 16]
# asm 1: vbroadcastf128 16(<l1w=int64#5),>omega1=reg256#14
# asm 2: vbroadcastf128 16(<l1w=%r8),>omega1=%ymm13
vbroadcastf128 16(%r8),%ymm13

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#14,>omega1=reg256#14
# asm 2: vpermilpd $0xc,<omega1=%ymm13,>omega1=%ymm13
vpermilpd $0xc,%ymm13,%ymm13

# qhasm: 4x t = approx b0123 * neg2
# asm 1: vmulpd <b0123=reg256#6,<neg2=reg256#1,>t=reg256#15
# asm 2: vmulpd <b0123=%ymm5,<neg2=%ymm0,>t=%ymm14
vmulpd %ymm5,%ymm0,%ymm14

# qhasm: b0123[0,1,2,3] = b0123[0]approx+b0123[1],t[0]approx+t[1],b0123[2]approx+b0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#15,<b0123=reg256#6,>b0123=reg256#6
# asm 2: vhaddpd <t=%ymm14,<b0123=%ymm5,>b0123=%ymm5
vhaddpd %ymm14,%ymm5,%ymm5

# qhasm: 4x b0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#13,<b0123=reg256#6,>b0123=reg256#6
# asm 2: vmulpd <omega0=%ymm12,<b0123=%ymm5,>b0123=%ymm5
vmulpd %ymm12,%ymm5,%ymm5

# qhasm: b2301[0,1,2,3] = b0123[2,3],b0123[0,1]
# asm 1: vperm2f128 $0x21,<b0123=reg256#6,<b0123=reg256#6,>b2301=reg256#13
# asm 2: vperm2f128 $0x21,<b0123=%ymm5,<b0123=%ymm5,>b2301=%ymm12
vperm2f128 $0x21,%ymm5,%ymm5,%ymm12

# qhasm: 4x b0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<b0123=reg256#6,>b0123=reg256#6
# asm 2: vmulpd <neg4=%ymm1,<b0123=%ymm5,>b0123=%ymm5
vmulpd %ymm1,%ymm5,%ymm5

# qhasm: 4x b0123 approx+= b2301
# asm 1: vaddpd <b2301=reg256#13,<b0123=reg256#6,>b0123=reg256#6
# asm 2: vaddpd <b2301=%ymm12,<b0123=%ymm5,>b0123=%ymm5
vaddpd %ymm12,%ymm5,%ymm5

# qhasm: 4x b0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#14,<b0123=reg256#6,>b0123=reg256#6
# asm 2: vmulpd <omega1=%ymm13,<b0123=%ymm5,>b0123=%ymm5
vmulpd %ymm13,%ymm5,%ymm5

# qhasm: 4x c = approx b0123 * qinv
# asm 1: vmulpd <b0123=reg256#6,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <b0123=%ymm5,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm5,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x b0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<b0123=reg256#6
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<b0123=%ymm5
vfnmadd231pd %ymm12,%ymm3,%ymm5

# qhasm: omega0 = mem256[l0w + 64]
# asm 1: vmovupd   64(<l0w=int64#4),>omega0=reg256#13
# asm 2: vmovupd   64(<l0w=%rcx),>omega0=%ymm12
vmovupd   64(%rcx),%ymm12

# qhasm: omega1 = 2x mem128[l1w + 32]
# asm 1: vbroadcastf128 32(<l1w=int64#5),>omega1=reg256#14
# asm 2: vbroadcastf128 32(<l1w=%r8),>omega1=%ymm13
vbroadcastf128 32(%r8),%ymm13

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#14,>omega1=reg256#14
# asm 2: vpermilpd $0xc,<omega1=%ymm13,>omega1=%ymm13
vpermilpd $0xc,%ymm13,%ymm13

# qhasm: 4x t = approx c0123 * neg2
# asm 1: vmulpd <c0123=reg256#7,<neg2=reg256#1,>t=reg256#15
# asm 2: vmulpd <c0123=%ymm6,<neg2=%ymm0,>t=%ymm14
vmulpd %ymm6,%ymm0,%ymm14

# qhasm: c0123[0,1,2,3] = c0123[0]approx+c0123[1],t[0]approx+t[1],c0123[2]approx+c0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#15,<c0123=reg256#7,>c0123=reg256#7
# asm 2: vhaddpd <t=%ymm14,<c0123=%ymm6,>c0123=%ymm6
vhaddpd %ymm14,%ymm6,%ymm6

# qhasm: 4x c0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#13,<c0123=reg256#7,>c0123=reg256#7
# asm 2: vmulpd <omega0=%ymm12,<c0123=%ymm6,>c0123=%ymm6
vmulpd %ymm12,%ymm6,%ymm6

# qhasm: c2301[0,1,2,3] = c0123[2,3],c0123[0,1]
# asm 1: vperm2f128 $0x21,<c0123=reg256#7,<c0123=reg256#7,>c2301=reg256#13
# asm 2: vperm2f128 $0x21,<c0123=%ymm6,<c0123=%ymm6,>c2301=%ymm12
vperm2f128 $0x21,%ymm6,%ymm6,%ymm12

# qhasm: 4x c0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<c0123=reg256#7,>c0123=reg256#7
# asm 2: vmulpd <neg4=%ymm1,<c0123=%ymm6,>c0123=%ymm6
vmulpd %ymm1,%ymm6,%ymm6

# qhasm: 4x c0123 approx+= c2301
# asm 1: vaddpd <c2301=reg256#13,<c0123=reg256#7,>c0123=reg256#7
# asm 2: vaddpd <c2301=%ymm12,<c0123=%ymm6,>c0123=%ymm6
vaddpd %ymm12,%ymm6,%ymm6

# qhasm: 4x c0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#14,<c0123=reg256#7,>c0123=reg256#7
# asm 2: vmulpd <omega1=%ymm13,<c0123=%ymm6,>c0123=%ymm6
vmulpd %ymm13,%ymm6,%ymm6

# qhasm: 4x c = approx c0123 * qinv
# asm 1: vmulpd <c0123=reg256#7,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <c0123=%ymm6,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm6,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x c0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<c0123=reg256#7
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<c0123=%ymm6
vfnmadd231pd %ymm12,%ymm3,%ymm6

# qhasm: omega0 = mem256[l0w + 96]
# asm 1: vmovupd   96(<l0w=int64#4),>omega0=reg256#13
# asm 2: vmovupd   96(<l0w=%rcx),>omega0=%ymm12
vmovupd   96(%rcx),%ymm12

# qhasm: omega1 = 2x mem128[l1w + 48]
# asm 1: vbroadcastf128 48(<l1w=int64#5),>omega1=reg256#14
# asm 2: vbroadcastf128 48(<l1w=%r8),>omega1=%ymm13
vbroadcastf128 48(%r8),%ymm13

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#14,>omega1=reg256#14
# asm 2: vpermilpd $0xc,<omega1=%ymm13,>omega1=%ymm13
vpermilpd $0xc,%ymm13,%ymm13

# qhasm: 4x t = approx d0123 * neg2
# asm 1: vmulpd <d0123=reg256#8,<neg2=reg256#1,>t=reg256#15
# asm 2: vmulpd <d0123=%ymm7,<neg2=%ymm0,>t=%ymm14
vmulpd %ymm7,%ymm0,%ymm14

# qhasm: d0123[0,1,2,3] = d0123[0]approx+d0123[1],t[0]approx+t[1],d0123[2]approx+d0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#15,<d0123=reg256#8,>d0123=reg256#8
# asm 2: vhaddpd <t=%ymm14,<d0123=%ymm7,>d0123=%ymm7
vhaddpd %ymm14,%ymm7,%ymm7

# qhasm: 4x d0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#13,<d0123=reg256#8,>d0123=reg256#8
# asm 2: vmulpd <omega0=%ymm12,<d0123=%ymm7,>d0123=%ymm7
vmulpd %ymm12,%ymm7,%ymm7

# qhasm: d2301[0,1,2,3] = d0123[2,3],d0123[0,1]
# asm 1: vperm2f128 $0x21,<d0123=reg256#8,<d0123=reg256#8,>d2301=reg256#13
# asm 2: vperm2f128 $0x21,<d0123=%ymm7,<d0123=%ymm7,>d2301=%ymm12
vperm2f128 $0x21,%ymm7,%ymm7,%ymm12

# qhasm: 4x d0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<d0123=reg256#8,>d0123=reg256#8
# asm 2: vmulpd <neg4=%ymm1,<d0123=%ymm7,>d0123=%ymm7
vmulpd %ymm1,%ymm7,%ymm7

# qhasm: 4x d0123 approx+= d2301
# asm 1: vaddpd <d2301=reg256#13,<d0123=reg256#8,>d0123=reg256#8
# asm 2: vaddpd <d2301=%ymm12,<d0123=%ymm7,>d0123=%ymm7
vaddpd %ymm12,%ymm7,%ymm7

# qhasm: 4x d0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#14,<d0123=reg256#8,>d0123=reg256#8
# asm 2: vmulpd <omega1=%ymm13,<d0123=%ymm7,>d0123=%ymm7
vmulpd %ymm13,%ymm7,%ymm7

# qhasm: 4x c = approx d0123 * qinv
# asm 1: vmulpd <d0123=reg256#8,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <d0123=%ymm7,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm7,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x d0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<d0123=reg256#8
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<d0123=%ymm7
vfnmadd231pd %ymm12,%ymm3,%ymm7

# qhasm: omega0 = mem256[l0w + 128]
# asm 1: vmovupd   128(<l0w=int64#4),>omega0=reg256#13
# asm 2: vmovupd   128(<l0w=%rcx),>omega0=%ymm12
vmovupd   128(%rcx),%ymm12

# qhasm: omega1 = 2x mem128[l1w + 64]
# asm 1: vbroadcastf128 64(<l1w=int64#5),>omega1=reg256#14
# asm 2: vbroadcastf128 64(<l1w=%r8),>omega1=%ymm13
vbroadcastf128 64(%r8),%ymm13

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#14,>omega1=reg256#14
# asm 2: vpermilpd $0xc,<omega1=%ymm13,>omega1=%ymm13
vpermilpd $0xc,%ymm13,%ymm13

# qhasm: 4x t = approx e0123 * neg2
# asm 1: vmulpd <e0123=reg256#9,<neg2=reg256#1,>t=reg256#15
# asm 2: vmulpd <e0123=%ymm8,<neg2=%ymm0,>t=%ymm14
vmulpd %ymm8,%ymm0,%ymm14

# qhasm: e0123[0,1,2,3] = e0123[0]approx+e0123[1],t[0]approx+t[1],e0123[2]approx+e0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#15,<e0123=reg256#9,>e0123=reg256#9
# asm 2: vhaddpd <t=%ymm14,<e0123=%ymm8,>e0123=%ymm8
vhaddpd %ymm14,%ymm8,%ymm8

# qhasm: 4x e0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#13,<e0123=reg256#9,>e0123=reg256#9
# asm 2: vmulpd <omega0=%ymm12,<e0123=%ymm8,>e0123=%ymm8
vmulpd %ymm12,%ymm8,%ymm8

# qhasm: e2301[0,1,2,3] = e0123[2,3],e0123[0,1]
# asm 1: vperm2f128 $0x21,<e0123=reg256#9,<e0123=reg256#9,>e2301=reg256#13
# asm 2: vperm2f128 $0x21,<e0123=%ymm8,<e0123=%ymm8,>e2301=%ymm12
vperm2f128 $0x21,%ymm8,%ymm8,%ymm12

# qhasm: 4x e0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<e0123=reg256#9,>e0123=reg256#9
# asm 2: vmulpd <neg4=%ymm1,<e0123=%ymm8,>e0123=%ymm8
vmulpd %ymm1,%ymm8,%ymm8

# qhasm: 4x e0123 approx+= e2301
# asm 1: vaddpd <e2301=reg256#13,<e0123=reg256#9,>e0123=reg256#9
# asm 2: vaddpd <e2301=%ymm12,<e0123=%ymm8,>e0123=%ymm8
vaddpd %ymm12,%ymm8,%ymm8

# qhasm: 4x e0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#14,<e0123=reg256#9,>e0123=reg256#9
# asm 2: vmulpd <omega1=%ymm13,<e0123=%ymm8,>e0123=%ymm8
vmulpd %ymm13,%ymm8,%ymm8

# qhasm: 4x c = approx e0123 * qinv
# asm 1: vmulpd <e0123=reg256#9,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <e0123=%ymm8,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm8,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x e0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<e0123=reg256#9
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<e0123=%ymm8
vfnmadd231pd %ymm12,%ymm3,%ymm8

# qhasm: omega0 = mem256[l0w + 160]
# asm 1: vmovupd   160(<l0w=int64#4),>omega0=reg256#13
# asm 2: vmovupd   160(<l0w=%rcx),>omega0=%ymm12
vmovupd   160(%rcx),%ymm12

# qhasm: omega1 = 2x mem128[l1w + 80]
# asm 1: vbroadcastf128 80(<l1w=int64#5),>omega1=reg256#14
# asm 2: vbroadcastf128 80(<l1w=%r8),>omega1=%ymm13
vbroadcastf128 80(%r8),%ymm13

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#14,>omega1=reg256#14
# asm 2: vpermilpd $0xc,<omega1=%ymm13,>omega1=%ymm13
vpermilpd $0xc,%ymm13,%ymm13

# qhasm: 4x t = approx f0123 * neg2
# asm 1: vmulpd <f0123=reg256#10,<neg2=reg256#1,>t=reg256#15
# asm 2: vmulpd <f0123=%ymm9,<neg2=%ymm0,>t=%ymm14
vmulpd %ymm9,%ymm0,%ymm14

# qhasm: f0123[0,1,2,3] = f0123[0]approx+f0123[1],t[0]approx+t[1],f0123[2]approx+f0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#15,<f0123=reg256#10,>f0123=reg256#10
# asm 2: vhaddpd <t=%ymm14,<f0123=%ymm9,>f0123=%ymm9
vhaddpd %ymm14,%ymm9,%ymm9

# qhasm: 4x f0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#13,<f0123=reg256#10,>f0123=reg256#10
# asm 2: vmulpd <omega0=%ymm12,<f0123=%ymm9,>f0123=%ymm9
vmulpd %ymm12,%ymm9,%ymm9

# qhasm: f2301[0,1,2,3] = f0123[2,3],f0123[0,1]
# asm 1: vperm2f128 $0x21,<f0123=reg256#10,<f0123=reg256#10,>f2301=reg256#13
# asm 2: vperm2f128 $0x21,<f0123=%ymm9,<f0123=%ymm9,>f2301=%ymm12
vperm2f128 $0x21,%ymm9,%ymm9,%ymm12

# qhasm: 4x f0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<f0123=reg256#10,>f0123=reg256#10
# asm 2: vmulpd <neg4=%ymm1,<f0123=%ymm9,>f0123=%ymm9
vmulpd %ymm1,%ymm9,%ymm9

# qhasm: 4x f0123 approx+= f2301
# asm 1: vaddpd <f2301=reg256#13,<f0123=reg256#10,>f0123=reg256#10
# asm 2: vaddpd <f2301=%ymm12,<f0123=%ymm9,>f0123=%ymm9
vaddpd %ymm12,%ymm9,%ymm9

# qhasm: 4x f0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#14,<f0123=reg256#10,>f0123=reg256#10
# asm 2: vmulpd <omega1=%ymm13,<f0123=%ymm9,>f0123=%ymm9
vmulpd %ymm13,%ymm9,%ymm9

# qhasm: 4x c = approx f0123 * qinv
# asm 1: vmulpd <f0123=reg256#10,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <f0123=%ymm9,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm9,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x f0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<f0123=reg256#10
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<f0123=%ymm9
vfnmadd231pd %ymm12,%ymm3,%ymm9

# qhasm: omega0 = mem256[l0w + 192]
# asm 1: vmovupd   192(<l0w=int64#4),>omega0=reg256#13
# asm 2: vmovupd   192(<l0w=%rcx),>omega0=%ymm12
vmovupd   192(%rcx),%ymm12

# qhasm: omega1 = 2x mem128[l1w + 96]
# asm 1: vbroadcastf128 96(<l1w=int64#5),>omega1=reg256#14
# asm 2: vbroadcastf128 96(<l1w=%r8),>omega1=%ymm13
vbroadcastf128 96(%r8),%ymm13

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#14,>omega1=reg256#14
# asm 2: vpermilpd $0xc,<omega1=%ymm13,>omega1=%ymm13
vpermilpd $0xc,%ymm13,%ymm13

# qhasm: 4x t = approx g0123 * neg2
# asm 1: vmulpd <g0123=reg256#11,<neg2=reg256#1,>t=reg256#15
# asm 2: vmulpd <g0123=%ymm10,<neg2=%ymm0,>t=%ymm14
vmulpd %ymm10,%ymm0,%ymm14

# qhasm: g0123[0,1,2,3] = g0123[0]approx+g0123[1],t[0]approx+t[1],g0123[2]approx+g0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#15,<g0123=reg256#11,>g0123=reg256#11
# asm 2: vhaddpd <t=%ymm14,<g0123=%ymm10,>g0123=%ymm10
vhaddpd %ymm14,%ymm10,%ymm10

# qhasm: 4x g0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#13,<g0123=reg256#11,>g0123=reg256#11
# asm 2: vmulpd <omega0=%ymm12,<g0123=%ymm10,>g0123=%ymm10
vmulpd %ymm12,%ymm10,%ymm10

# qhasm: g2301[0,1,2,3] = g0123[2,3],g0123[0,1]
# asm 1: vperm2f128 $0x21,<g0123=reg256#11,<g0123=reg256#11,>g2301=reg256#13
# asm 2: vperm2f128 $0x21,<g0123=%ymm10,<g0123=%ymm10,>g2301=%ymm12
vperm2f128 $0x21,%ymm10,%ymm10,%ymm12

# qhasm: 4x g0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<g0123=reg256#11,>g0123=reg256#11
# asm 2: vmulpd <neg4=%ymm1,<g0123=%ymm10,>g0123=%ymm10
vmulpd %ymm1,%ymm10,%ymm10

# qhasm: 4x g0123 approx+= g2301
# asm 1: vaddpd <g2301=reg256#13,<g0123=reg256#11,>g0123=reg256#11
# asm 2: vaddpd <g2301=%ymm12,<g0123=%ymm10,>g0123=%ymm10
vaddpd %ymm12,%ymm10,%ymm10

# qhasm: 4x g0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#14,<g0123=reg256#11,>g0123=reg256#11
# asm 2: vmulpd <omega1=%ymm13,<g0123=%ymm10,>g0123=%ymm10
vmulpd %ymm13,%ymm10,%ymm10

# qhasm: 4x c = approx g0123 * qinv
# asm 1: vmulpd <g0123=reg256#11,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <g0123=%ymm10,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm10,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x g0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<g0123=reg256#11
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<g0123=%ymm10
vfnmadd231pd %ymm12,%ymm3,%ymm10

# qhasm: omega0 = mem256[l0w + 224]
# asm 1: vmovupd   224(<l0w=int64#4),>omega0=reg256#13
# asm 2: vmovupd   224(<l0w=%rcx),>omega0=%ymm12
vmovupd   224(%rcx),%ymm12

# qhasm: omega1 = 2x mem128[l1w + 112]
# asm 1: vbroadcastf128 112(<l1w=int64#5),>omega1=reg256#14
# asm 2: vbroadcastf128 112(<l1w=%r8),>omega1=%ymm13
vbroadcastf128 112(%r8),%ymm13

# qhasm: omega1[0,1,2,3] = omega1[0,0,3,3]
# asm 1: vpermilpd $0xc,<omega1=reg256#14,>omega1=reg256#14
# asm 2: vpermilpd $0xc,<omega1=%ymm13,>omega1=%ymm13
vpermilpd $0xc,%ymm13,%ymm13

# qhasm: 4x t = approx h0123 * neg2
# asm 1: vmulpd <h0123=reg256#12,<neg2=reg256#1,>t=reg256#15
# asm 2: vmulpd <h0123=%ymm11,<neg2=%ymm0,>t=%ymm14
vmulpd %ymm11,%ymm0,%ymm14

# qhasm: h0123[0,1,2,3] = h0123[0]approx+h0123[1],t[0]approx+t[1],h0123[2]approx+h0123[3],t[2]approx+t[3]
# asm 1: vhaddpd <t=reg256#15,<h0123=reg256#12,>h0123=reg256#12
# asm 2: vhaddpd <t=%ymm14,<h0123=%ymm11,>h0123=%ymm11
vhaddpd %ymm14,%ymm11,%ymm11

# qhasm: 4x h0123 approx*= omega0
# asm 1: vmulpd <omega0=reg256#13,<h0123=reg256#12,>h0123=reg256#12
# asm 2: vmulpd <omega0=%ymm12,<h0123=%ymm11,>h0123=%ymm11
vmulpd %ymm12,%ymm11,%ymm11

# qhasm: h2301[0,1,2,3] = h0123[2,3],h0123[0,1]
# asm 1: vperm2f128 $0x21,<h0123=reg256#12,<h0123=reg256#12,>h2301=reg256#13
# asm 2: vperm2f128 $0x21,<h0123=%ymm11,<h0123=%ymm11,>h2301=%ymm12
vperm2f128 $0x21,%ymm11,%ymm11,%ymm12

# qhasm: 4x h0123 approx*= neg4
# asm 1: vmulpd <neg4=reg256#2,<h0123=reg256#12,>h0123=reg256#12
# asm 2: vmulpd <neg4=%ymm1,<h0123=%ymm11,>h0123=%ymm11
vmulpd %ymm1,%ymm11,%ymm11

# qhasm: 4x h0123 approx+= h2301
# asm 1: vaddpd <h2301=reg256#13,<h0123=reg256#12,>h0123=reg256#12
# asm 2: vaddpd <h2301=%ymm12,<h0123=%ymm11,>h0123=%ymm11
vaddpd %ymm12,%ymm11,%ymm11

# qhasm: 4x h0123 approx*= omega1
# asm 1: vmulpd <omega1=reg256#14,<h0123=reg256#12,>h0123=reg256#12
# asm 2: vmulpd <omega1=%ymm13,<h0123=%ymm11,>h0123=%ymm11
vmulpd %ymm13,%ymm11,%ymm11

# qhasm: 4x c = approx h0123 * qinv
# asm 1: vmulpd <h0123=reg256#12,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <h0123=%ymm11,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm11,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x h0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<h0123=reg256#12
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<h0123=%ymm11
vfnmadd231pd %ymm12,%ymm3,%ymm11

# qhasm: omega2 = 4x mem64[l2w + 8]
# asm 1: vbroadcastsd 8(<l2w=int64#6),>omega2=reg256#13
# asm 2: vbroadcastsd 8(<l2w=%r9),>omega2=%ymm12
vbroadcastsd 8(%r9),%ymm12

# qhasm: 4x t = approx a0123 - b0123
# asm 1: vsubpd <b0123=reg256#6,<a0123=reg256#5,>t=reg256#14
# asm 2: vsubpd <b0123=%ymm5,<a0123=%ymm4,>t=%ymm13
vsubpd %ymm5,%ymm4,%ymm13

# qhasm: 4x a0123 = approx a0123 + b0123
# asm 1: vaddpd <a0123=reg256#5,<b0123=reg256#6,>a0123=reg256#5
# asm 2: vaddpd <a0123=%ymm4,<b0123=%ymm5,>a0123=%ymm4
vaddpd %ymm4,%ymm5,%ymm4

# qhasm: 4x b0123 = approx t * omega2
# asm 1: vmulpd <t=reg256#14,<omega2=reg256#13,>b0123=reg256#6
# asm 2: vmulpd <t=%ymm13,<omega2=%ymm12,>b0123=%ymm5
vmulpd %ymm13,%ymm12,%ymm5

# qhasm: omega2 = 4x mem64[l2w + 24]
# asm 1: vbroadcastsd 24(<l2w=int64#6),>omega2=reg256#13
# asm 2: vbroadcastsd 24(<l2w=%r9),>omega2=%ymm12
vbroadcastsd 24(%r9),%ymm12

# qhasm: 4x t = approx c0123 - d0123
# asm 1: vsubpd <d0123=reg256#8,<c0123=reg256#7,>t=reg256#14
# asm 2: vsubpd <d0123=%ymm7,<c0123=%ymm6,>t=%ymm13
vsubpd %ymm7,%ymm6,%ymm13

# qhasm: 4x c0123 = approx c0123 + d0123
# asm 1: vaddpd <c0123=reg256#7,<d0123=reg256#8,>c0123=reg256#7
# asm 2: vaddpd <c0123=%ymm6,<d0123=%ymm7,>c0123=%ymm6
vaddpd %ymm6,%ymm7,%ymm6

# qhasm: 4x d0123 = approx t * omega2
# asm 1: vmulpd <t=reg256#14,<omega2=reg256#13,>d0123=reg256#8
# asm 2: vmulpd <t=%ymm13,<omega2=%ymm12,>d0123=%ymm7
vmulpd %ymm13,%ymm12,%ymm7

# qhasm: omega2 = 4x mem64[l2w + 40]
# asm 1: vbroadcastsd 40(<l2w=int64#6),>omega2=reg256#13
# asm 2: vbroadcastsd 40(<l2w=%r9),>omega2=%ymm12
vbroadcastsd 40(%r9),%ymm12

# qhasm: 4x t = approx e0123 - f0123
# asm 1: vsubpd <f0123=reg256#10,<e0123=reg256#9,>t=reg256#14
# asm 2: vsubpd <f0123=%ymm9,<e0123=%ymm8,>t=%ymm13
vsubpd %ymm9,%ymm8,%ymm13

# qhasm: 4x e0123 = approx e0123 + f0123
# asm 1: vaddpd <e0123=reg256#9,<f0123=reg256#10,>e0123=reg256#9
# asm 2: vaddpd <e0123=%ymm8,<f0123=%ymm9,>e0123=%ymm8
vaddpd %ymm8,%ymm9,%ymm8

# qhasm: 4x f0123 = approx t * omega2
# asm 1: vmulpd <t=reg256#14,<omega2=reg256#13,>f0123=reg256#10
# asm 2: vmulpd <t=%ymm13,<omega2=%ymm12,>f0123=%ymm9
vmulpd %ymm13,%ymm12,%ymm9

# qhasm: omega2 = 4x mem64[l2w + 56]
# asm 1: vbroadcastsd 56(<l2w=int64#6),>omega2=reg256#13
# asm 2: vbroadcastsd 56(<l2w=%r9),>omega2=%ymm12
vbroadcastsd 56(%r9),%ymm12

# qhasm: 4x t = approx g0123 - h0123
# asm 1: vsubpd <h0123=reg256#12,<g0123=reg256#11,>t=reg256#14
# asm 2: vsubpd <h0123=%ymm11,<g0123=%ymm10,>t=%ymm13
vsubpd %ymm11,%ymm10,%ymm13

# qhasm: 4x g0123 = approx g0123 + h0123
# asm 1: vaddpd <g0123=reg256#11,<h0123=reg256#12,>g0123=reg256#11
# asm 2: vaddpd <g0123=%ymm10,<h0123=%ymm11,>g0123=%ymm10
vaddpd %ymm10,%ymm11,%ymm10

# qhasm: 4x h0123 = approx t * omega2
# asm 1: vmulpd <t=reg256#14,<omega2=reg256#13,>h0123=reg256#12
# asm 2: vmulpd <t=%ymm13,<omega2=%ymm12,>h0123=%ymm11
vmulpd %ymm13,%ymm12,%ymm11

# qhasm: omega3 = 4x mem64[l3w + 8]
# asm 1: vbroadcastsd 8(<l3w=int64#7),>omega3=reg256#13
# asm 2: vbroadcastsd 8(<l3w=%rax),>omega3=%ymm12
vbroadcastsd 8(%rax),%ymm12

# qhasm: 4x t = approx a0123 - c0123
# asm 1: vsubpd <c0123=reg256#7,<a0123=reg256#5,>t=reg256#14
# asm 2: vsubpd <c0123=%ymm6,<a0123=%ymm4,>t=%ymm13
vsubpd %ymm6,%ymm4,%ymm13

# qhasm: 4x a0123 = approx a0123 + c0123
# asm 1: vaddpd <a0123=reg256#5,<c0123=reg256#7,>a0123=reg256#5
# asm 2: vaddpd <a0123=%ymm4,<c0123=%ymm6,>a0123=%ymm4
vaddpd %ymm4,%ymm6,%ymm4

# qhasm: 4x c0123 = approx t * omega3
# asm 1: vmulpd <t=reg256#14,<omega3=reg256#13,>c0123=reg256#7
# asm 2: vmulpd <t=%ymm13,<omega3=%ymm12,>c0123=%ymm6
vmulpd %ymm13,%ymm12,%ymm6

# qhasm: 4x t = approx b0123 - d0123
# asm 1: vsubpd <d0123=reg256#8,<b0123=reg256#6,>t=reg256#14
# asm 2: vsubpd <d0123=%ymm7,<b0123=%ymm5,>t=%ymm13
vsubpd %ymm7,%ymm5,%ymm13

# qhasm: 4x b0123 = approx b0123 + d0123
# asm 1: vaddpd <b0123=reg256#6,<d0123=reg256#8,>b0123=reg256#6
# asm 2: vaddpd <b0123=%ymm5,<d0123=%ymm7,>b0123=%ymm5
vaddpd %ymm5,%ymm7,%ymm5

# qhasm: 4x d0123 = approx t * omega3
# asm 1: vmulpd <t=reg256#14,<omega3=reg256#13,>d0123=reg256#8
# asm 2: vmulpd <t=%ymm13,<omega3=%ymm12,>d0123=%ymm7
vmulpd %ymm13,%ymm12,%ymm7

# qhasm: omega3 = 4x mem64[l3w + 24]
# asm 1: vbroadcastsd 24(<l3w=int64#7),>omega3=reg256#13
# asm 2: vbroadcastsd 24(<l3w=%rax),>omega3=%ymm12
vbroadcastsd 24(%rax),%ymm12

# qhasm: 4x t = approx e0123 - g0123
# asm 1: vsubpd <g0123=reg256#11,<e0123=reg256#9,>t=reg256#14
# asm 2: vsubpd <g0123=%ymm10,<e0123=%ymm8,>t=%ymm13
vsubpd %ymm10,%ymm8,%ymm13

# qhasm: 4x e0123 = approx e0123 + g0123
# asm 1: vaddpd <e0123=reg256#9,<g0123=reg256#11,>e0123=reg256#9
# asm 2: vaddpd <e0123=%ymm8,<g0123=%ymm10,>e0123=%ymm8
vaddpd %ymm8,%ymm10,%ymm8

# qhasm: 4x g0123 = approx t * omega3
# asm 1: vmulpd <t=reg256#14,<omega3=reg256#13,>g0123=reg256#11
# asm 2: vmulpd <t=%ymm13,<omega3=%ymm12,>g0123=%ymm10
vmulpd %ymm13,%ymm12,%ymm10

# qhasm: 4x t = approx f0123 - h0123
# asm 1: vsubpd <h0123=reg256#12,<f0123=reg256#10,>t=reg256#14
# asm 2: vsubpd <h0123=%ymm11,<f0123=%ymm9,>t=%ymm13
vsubpd %ymm11,%ymm9,%ymm13

# qhasm: 4x f0123 = approx f0123 + h0123
# asm 1: vaddpd <f0123=reg256#10,<h0123=reg256#12,>f0123=reg256#10
# asm 2: vaddpd <f0123=%ymm9,<h0123=%ymm11,>f0123=%ymm9
vaddpd %ymm9,%ymm11,%ymm9

# qhasm: 4x h0123 = approx t * omega3
# asm 1: vmulpd <t=reg256#14,<omega3=reg256#13,>h0123=reg256#12
# asm 2: vmulpd <t=%ymm13,<omega3=%ymm12,>h0123=%ymm11
vmulpd %ymm13,%ymm12,%ymm11

# qhasm: omega4= 4x mem64[l4w + 8]
# asm 1: vbroadcastsd 8(<l4w=int64#3),>omega4=reg256#13
# asm 2: vbroadcastsd 8(<l4w=%rdx),>omega4=%ymm12
vbroadcastsd 8(%rdx),%ymm12

# qhasm: 4x t = approx a0123 - e0123
# asm 1: vsubpd <e0123=reg256#9,<a0123=reg256#5,>t=reg256#14
# asm 2: vsubpd <e0123=%ymm8,<a0123=%ymm4,>t=%ymm13
vsubpd %ymm8,%ymm4,%ymm13

# qhasm: 4x a0123 = approx a0123 + e0123
# asm 1: vaddpd <a0123=reg256#5,<e0123=reg256#9,>a0123=reg256#5
# asm 2: vaddpd <a0123=%ymm4,<e0123=%ymm8,>a0123=%ymm4
vaddpd %ymm4,%ymm8,%ymm4

# qhasm: 4x e0123 = approx t * omega4
# asm 1: vmulpd <t=reg256#14,<omega4=reg256#13,>e0123=reg256#9
# asm 2: vmulpd <t=%ymm13,<omega4=%ymm12,>e0123=%ymm8
vmulpd %ymm13,%ymm12,%ymm8

# qhasm: 4x t = approx b0123 - f0123
# asm 1: vsubpd <f0123=reg256#10,<b0123=reg256#6,>t=reg256#14
# asm 2: vsubpd <f0123=%ymm9,<b0123=%ymm5,>t=%ymm13
vsubpd %ymm9,%ymm5,%ymm13

# qhasm: 4x b0123 = approx b0123 + f0123
# asm 1: vaddpd <b0123=reg256#6,<f0123=reg256#10,>b0123=reg256#6
# asm 2: vaddpd <b0123=%ymm5,<f0123=%ymm9,>b0123=%ymm5
vaddpd %ymm5,%ymm9,%ymm5

# qhasm: 4x f0123 = approx t * omega4
# asm 1: vmulpd <t=reg256#14,<omega4=reg256#13,>f0123=reg256#10
# asm 2: vmulpd <t=%ymm13,<omega4=%ymm12,>f0123=%ymm9
vmulpd %ymm13,%ymm12,%ymm9

# qhasm: 4x t = approx c0123 - g0123
# asm 1: vsubpd <g0123=reg256#11,<c0123=reg256#7,>t=reg256#14
# asm 2: vsubpd <g0123=%ymm10,<c0123=%ymm6,>t=%ymm13
vsubpd %ymm10,%ymm6,%ymm13

# qhasm: 4x c0123 = approx c0123 + g0123
# asm 1: vaddpd <c0123=reg256#7,<g0123=reg256#11,>c0123=reg256#7
# asm 2: vaddpd <c0123=%ymm6,<g0123=%ymm10,>c0123=%ymm6
vaddpd %ymm6,%ymm10,%ymm6

# qhasm: 4x g0123 = approx t * omega4
# asm 1: vmulpd <t=reg256#14,<omega4=reg256#13,>g0123=reg256#11
# asm 2: vmulpd <t=%ymm13,<omega4=%ymm12,>g0123=%ymm10
vmulpd %ymm13,%ymm12,%ymm10

# qhasm: 4x t = approx d0123 - h0123
# asm 1: vsubpd <h0123=reg256#12,<d0123=reg256#8,>t=reg256#14
# asm 2: vsubpd <h0123=%ymm11,<d0123=%ymm7,>t=%ymm13
vsubpd %ymm11,%ymm7,%ymm13

# qhasm: 4x d0123 = approx d0123 + h0123
# asm 1: vaddpd <d0123=reg256#8,<h0123=reg256#12,>d0123=reg256#8
# asm 2: vaddpd <d0123=%ymm7,<h0123=%ymm11,>d0123=%ymm7
vaddpd %ymm7,%ymm11,%ymm7

# qhasm: 4x h0123 = approx t * omega4
# asm 1: vmulpd <t=reg256#14,<omega4=reg256#13,>h0123=reg256#12
# asm 2: vmulpd <t=%ymm13,<omega4=%ymm12,>h0123=%ymm11
vmulpd %ymm13,%ymm12,%ymm11

# qhasm: 4x c = approx a0123 * qinv
# asm 1: vmulpd <a0123=reg256#5,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <a0123=%ymm4,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm4,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x a0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<a0123=reg256#5
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<a0123=%ymm4
vfnmadd231pd %ymm12,%ymm3,%ymm4

# qhasm: 4x c = approx b0123 * qinv
# asm 1: vmulpd <b0123=reg256#6,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <b0123=%ymm5,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm5,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x b0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<b0123=reg256#6
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<b0123=%ymm5
vfnmadd231pd %ymm12,%ymm3,%ymm5

# qhasm: 4x c = approx c0123 * qinv
# asm 1: vmulpd <c0123=reg256#7,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <c0123=%ymm6,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm6,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x c0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<c0123=reg256#7
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<c0123=%ymm6
vfnmadd231pd %ymm12,%ymm3,%ymm6

# qhasm: 4x c = approx d0123 * qinv
# asm 1: vmulpd <d0123=reg256#8,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <d0123=%ymm7,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm7,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x d0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<d0123=reg256#8
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<d0123=%ymm7
vfnmadd231pd %ymm12,%ymm3,%ymm7

# qhasm: 4x c = approx e0123 * qinv
# asm 1: vmulpd <e0123=reg256#9,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <e0123=%ymm8,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm8,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x e0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<e0123=reg256#9
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<e0123=%ymm8
vfnmadd231pd %ymm12,%ymm3,%ymm8

# qhasm: 4x c = approx f0123 * qinv
# asm 1: vmulpd <f0123=reg256#10,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <f0123=%ymm9,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm9,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x f0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<f0123=reg256#10
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<f0123=%ymm9
vfnmadd231pd %ymm12,%ymm3,%ymm9

# qhasm: 4x c = approx g0123 * qinv
# asm 1: vmulpd <g0123=reg256#11,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <g0123=%ymm10,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm10,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x g0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<g0123=reg256#11
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<g0123=%ymm10
vfnmadd231pd %ymm12,%ymm3,%ymm10

# qhasm: 4x c = approx h0123 * qinv
# asm 1: vmulpd <h0123=reg256#12,<qinv=reg256#3,>c=reg256#13
# asm 2: vmulpd <h0123=%ymm11,<qinv=%ymm2,>c=%ymm12
vmulpd %ymm11,%ymm2,%ymm12

# qhasm: 4x c = round(c)
# asm 1: vroundpd $8,<c=reg256#13,>c=reg256#13
# asm 2: vroundpd $8,<c=%ymm12,>c=%ymm12
vroundpd $8,%ymm12,%ymm12

# qhasm: 4x h0123 approx-= c * q
# asm 1: vfnmadd231pd <c=reg256#13,<q=reg256#4,<h0123=reg256#12
# asm 2: vfnmadd231pd <c=%ymm12,<q=%ymm3,<h0123=%ymm11
vfnmadd231pd %ymm12,%ymm3,%ymm11

# qhasm: mem256[input_0 +   0] = a0123
# asm 1: vmovupd   <a0123=reg256#5,0(<input_0=int64#1)
# asm 2: vmovupd   <a0123=%ymm4,0(<input_0=%rdi)
vmovupd   %ymm4,0(%rdi)

# qhasm: mem256[input_0 +  32] = b0123
# asm 1: vmovupd   <b0123=reg256#6,32(<input_0=int64#1)
# asm 2: vmovupd   <b0123=%ymm5,32(<input_0=%rdi)
vmovupd   %ymm5,32(%rdi)

# qhasm: mem256[input_0 +  64] = c0123
# asm 1: vmovupd   <c0123=reg256#7,64(<input_0=int64#1)
# asm 2: vmovupd   <c0123=%ymm6,64(<input_0=%rdi)
vmovupd   %ymm6,64(%rdi)

# qhasm: mem256[input_0 +  96] = d0123
# asm 1: vmovupd   <d0123=reg256#8,96(<input_0=int64#1)
# asm 2: vmovupd   <d0123=%ymm7,96(<input_0=%rdi)
vmovupd   %ymm7,96(%rdi)

# qhasm: mem256[input_0 + 128] = e0123
# asm 1: vmovupd   <e0123=reg256#9,128(<input_0=int64#1)
# asm 2: vmovupd   <e0123=%ymm8,128(<input_0=%rdi)
vmovupd   %ymm8,128(%rdi)

# qhasm: mem256[input_0 + 160] = f0123
# asm 1: vmovupd   <f0123=reg256#10,160(<input_0=int64#1)
# asm 2: vmovupd   <f0123=%ymm9,160(<input_0=%rdi)
vmovupd   %ymm9,160(%rdi)

# qhasm: mem256[input_0 + 192] = g0123
# asm 1: vmovupd   <g0123=reg256#11,192(<input_0=int64#1)
# asm 2: vmovupd   <g0123=%ymm10,192(<input_0=%rdi)
vmovupd   %ymm10,192(%rdi)

# qhasm: mem256[input_0 + 224] = h0123
# asm 1: vmovupd   <h0123=reg256#12,224(<input_0=int64#1)
# asm 2: vmovupd   <h0123=%ymm11,224(<input_0=%rdi)
vmovupd   %ymm11,224(%rdi)

# qhasm: input_0 += 256
# asm 1: add  $256,<input_0=int64#1
# asm 2: add  $256,<input_0=%rdi
add  $256,%rdi

# qhasm: input_1 += 128
# asm 1: add  $128,<input_1=int64#2
# asm 2: add  $128,<input_1=%rsi
add  $128,%rsi

# qhasm: l0w += 256
# asm 1: add  $256,<l0w=int64#4
# asm 2: add  $256,<l0w=%rcx
add  $256,%rcx

# qhasm: l1w += 128
# asm 1: add  $128,<l1w=int64#5
# asm 2: add  $128,<l1w=%r8
add  $128,%r8

# qhasm: l2w += 64
# asm 1: add  $64,<l2w=int64#6
# asm 2: add  $64,<l2w=%r9
add  $64,%r9

# qhasm: l3w += 32
# asm 1: add  $32,<l3w=int64#7
# asm 2: add  $32,<l3w=%rax
add  $32,%rax

# qhasm: l4w += 16
# asm 1: add  $16,<l4w=int64#3
# asm 2: add  $16,<l4w=%rdx
add  $16,%rdx

# qhasm: unsigned>? ii -= 16
# asm 1: sub  $16,<ii=int64#8
# asm 2: sub  $16,<ii=%r10
sub  $16,%r10
# comment:fp stack unchanged by jump

# qhasm: goto looptop if unsigned>
ja ._looptop

# qhasm: return
add %r11,%rsp
ret
