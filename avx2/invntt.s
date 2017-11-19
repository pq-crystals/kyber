.global invntt
invntt:
mov		%rsp,%r11
and		$31,%r11
sub		%r11,%rsp

vmovdqa		_16xqinv(%rip),%ymm0
vmovdqa		_16xq(%rip),%ymm1
vmovdqa		_16x4q(%rip),%ymm2

mov		%rdx,%r8

#first round
#load
vmovdqa		(%rsi),%ymm4
vmovdqa		32(%rsi),%ymm5
vmovdqa		64(%rsi),%ymm6
vmovdqa		96(%rsi),%ymm7
vmovdqa		128(%rsi),%ymm8
vmovdqa		160(%rsi),%ymm9
vmovdqa		192(%rsi),%ymm10
vmovdqa		224(%rsi),%ymm11

vmovdqu		_lowdword(%rip),%ymm3

# Establish required permutation
vpand %ymm4, %ymm3, %ymm14
vpand %ymm5, %ymm3, %ymm15
vpsrad $16,%ymm4,%ymm4
vpsrad $16,%ymm5,%ymm5
vpackssdw %ymm5, %ymm4, %ymm5
vpackusdw %ymm15, %ymm14, %ymm4
vpermq $0xd8,%ymm4,%ymm4
vpermq $0xd8,%ymm5,%ymm5

vpand %ymm6, %ymm3, %ymm14
vpand %ymm7, %ymm3, %ymm15
vpsrad $16,%ymm6,%ymm6
vpsrad $16,%ymm7,%ymm7
vpackssdw %ymm7, %ymm6, %ymm7
vpackusdw %ymm15, %ymm14, %ymm6
vpermq $0xd8,%ymm6,%ymm6
vpermq $0xd8,%ymm7,%ymm7

vpand %ymm8, %ymm3, %ymm14
vpand %ymm9, %ymm3, %ymm15
vpsrad $16,%ymm8,%ymm8
vpsrad $16,%ymm9,%ymm9
vpackssdw %ymm9, %ymm8, %ymm9
vpackusdw %ymm15, %ymm14, %ymm8
vpermq $0xd8,%ymm8,%ymm8
vpermq $0xd8,%ymm9,%ymm9

vpand %ymm10, %ymm3, %ymm14
vpand %ymm11, %ymm3, %ymm15
vpsrad $16,%ymm10,%ymm10
vpsrad $16,%ymm11,%ymm11
vpackssdw %ymm11, %ymm10, %ymm11
vpackusdw %ymm15, %ymm14, %ymm10
vpermq $0xd8,%ymm10,%ymm10
vpermq $0xd8,%ymm11,%ymm11

#level 0
#update
vmovdqa		%ymm4,%ymm12
vmovdqa		%ymm6,%ymm13
vmovdqa		%ymm8,%ymm14
vmovdqa		%ymm10,%ymm15
vpaddw		%ymm4,%ymm5,%ymm4
vpaddw		%ymm6,%ymm7,%ymm6
vpaddw		%ymm8,%ymm9,%ymm8
vpaddw		%ymm10,%ymm11,%ymm10
vpsubw		%ymm5,%ymm12,%ymm5
vpsubw		%ymm7,%ymm13,%ymm7
vpsubw		%ymm9,%ymm14,%ymm9
vpsubw		%ymm11,%ymm15,%ymm11

#zetas
vmovdqa		(%r8),%ymm13
vmovdqa		32(%r8),%ymm14
vmovdqa		64(%r8),%ymm15
vmovdqa		96(%r8),%ymm3

#mul
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9
vpmullw		%ymm3,%ymm11,%ymm15
vpmulhw		%ymm3,%ymm11,%ymm11

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm7,%ymm7
vpsubw		%ymm14,%ymm9,%ymm9
vpsubw		%ymm15,%ymm11,%ymm11

#level 1
#shuffle
vmovdqa		_vpshufb_idx(%rip),%ymm3
vpshufb		%ymm3,%ymm4,%ymm12
vpshufb		%ymm3,%ymm5,%ymm13
vpshufb		%ymm3,%ymm6,%ymm14
vpshufb		%ymm3,%ymm7,%ymm15
vpblendw	$0x55,%ymm4,%ymm13,%ymm4
vpblendw	$0xAA,%ymm5,%ymm12,%ymm5
vpblendw	$0x55,%ymm6,%ymm15,%ymm6
vpblendw	$0xAA,%ymm7,%ymm14,%ymm7
vpshufb		%ymm3,%ymm8,%ymm12
vpshufb		%ymm3,%ymm9,%ymm13
vpshufb		%ymm3,%ymm10,%ymm14
vpshufb		%ymm3,%ymm11,%ymm15
vpblendw	$0x55,%ymm8,%ymm13,%ymm8
vpblendw	$0xAA,%ymm9,%ymm12,%ymm9
vpblendw	$0x55,%ymm10,%ymm15,%ymm10
vpblendw	$0xAA,%ymm11,%ymm14,%ymm11

#update
vmovdqa		%ymm4,%ymm12
vmovdqa		%ymm6,%ymm13
vmovdqa		%ymm8,%ymm14
vmovdqa		%ymm10,%ymm15
vpaddw		%ymm4,%ymm5,%ymm4
vpaddw		%ymm6,%ymm7,%ymm6
vpaddw		%ymm8,%ymm9,%ymm8
vpaddw		%ymm10,%ymm11,%ymm10
vpsubw		%ymm5,%ymm12,%ymm5
vpsubw		%ymm7,%ymm13,%ymm7
vpsubw		%ymm9,%ymm14,%ymm9
vpsubw		%ymm11,%ymm15,%ymm11

#zetas
vmovdqa		256(%r8),%ymm13
vmovdqa		288(%r8),%ymm14
vmovdqa		320(%r8),%ymm15
vmovdqa		352(%r8),%ymm3

#mul
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9
vpmullw		%ymm3,%ymm11,%ymm15
vpmulhw		%ymm3,%ymm11,%ymm11

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm7,%ymm7
vpsubw		%ymm14,%ymm9,%ymm9
vpsubw		%ymm15,%ymm11,%ymm11

#reduce 2
vmovdqa		_low_mask(%rip),%ymm3
vpaddw		%ymm2,%ymm4,%ymm4
vpaddw		%ymm2,%ymm6,%ymm6
vpaddw		%ymm2,%ymm8,%ymm8
vpaddw		%ymm2,%ymm10,%ymm10
vpsrlw		$13,%ymm4,%ymm12
vpsrlw		$13,%ymm6,%ymm13
vpsrlw		$13,%ymm8,%ymm14
vpsrlw		$13,%ymm10,%ymm15
#vpmullw	%ymm1,%ymm12,%ymm12
#vpmullw	%ymm1,%ymm13,%ymm13
#vpmullw	%ymm1,%ymm14,%ymm14
#vpmullw	%ymm1,%ymm15,%ymm15
vpand		%ymm3,%ymm4,%ymm4
vpand		%ymm3,%ymm6,%ymm6
vpand		%ymm3,%ymm8,%ymm8
vpand		%ymm3,%ymm10,%ymm10
vpsubw		%ymm12,%ymm4,%ymm4
vpsubw		%ymm13,%ymm6,%ymm6
vpsubw		%ymm14,%ymm8,%ymm8
vpsubw		%ymm15,%ymm10,%ymm10
vpsllw		$9,%ymm12,%ymm12
vpsllw		$9,%ymm13,%ymm13
vpsllw		$9,%ymm14,%ymm14
vpsllw		$9,%ymm15,%ymm15
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8
vpaddw		%ymm15,%ymm10,%ymm10
vpsubw		%ymm1,%ymm4,%ymm4
vpsubw		%ymm1,%ymm6,%ymm6
vpsubw		%ymm1,%ymm8,%ymm8
vpsubw		%ymm1,%ymm10,%ymm10

#level 2
#shuffle
vpshufd		$0xB1,%ymm4,%ymm12
vpshufd		$0xB1,%ymm5,%ymm13
vpshufd		$0xB1,%ymm6,%ymm14
vpshufd		$0xB1,%ymm7,%ymm15
vpblendd	$0x55,%ymm4,%ymm13,%ymm4
vpblendd	$0xAA,%ymm5,%ymm12,%ymm5
vpblendd	$0x55,%ymm6,%ymm15,%ymm6
vpblendd	$0xAA,%ymm7,%ymm14,%ymm7
vpshufd		$0xB1,%ymm8,%ymm12
vpshufd		$0xB1,%ymm9,%ymm13
vpshufd		$0xB1,%ymm10,%ymm14
vpshufd		$0xB1,%ymm11,%ymm15
vpblendd	$0x55,%ymm8,%ymm13,%ymm8
vpblendd	$0xAA,%ymm9,%ymm12,%ymm9
vpblendd	$0x55,%ymm10,%ymm15,%ymm10
vpblendd	$0xAA,%ymm11,%ymm14,%ymm11

#update
vmovdqa		%ymm4,%ymm12
vmovdqa		%ymm6,%ymm13
vmovdqa		%ymm8,%ymm14
vmovdqa		%ymm10,%ymm15
vpaddw		%ymm4,%ymm5,%ymm4
vpaddw		%ymm6,%ymm7,%ymm6
vpaddw		%ymm8,%ymm9,%ymm8
vpaddw		%ymm10,%ymm11,%ymm10
vpsubw		%ymm5,%ymm12,%ymm5
vpsubw		%ymm7,%ymm13,%ymm7
vpsubw		%ymm9,%ymm14,%ymm9
vpsubw		%ymm11,%ymm15,%ymm11

#zetas
vmovdqa		512(%r8),%ymm13
vmovdqa		544(%r8),%ymm14
vmovdqa		576(%r8),%ymm15
vmovdqa		608(%r8),%ymm3

#mul
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9
vpmullw		%ymm3,%ymm11,%ymm15
vpmulhw		%ymm3,%ymm11,%ymm11

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm7,%ymm7
vpsubw		%ymm14,%ymm9,%ymm9
vpsubw		%ymm15,%ymm11,%ymm11

#level 3
#shuffle
vshufpd		$0x00,%ymm5,%ymm4,%ymm3
vshufpd		$0x0F,%ymm5,%ymm4,%ymm4
vshufpd		$0x00,%ymm7,%ymm6,%ymm5
vshufpd		$0x0F,%ymm7,%ymm6,%ymm6
vshufpd		$0x00,%ymm9,%ymm8,%ymm7
vshufpd		$0x0F,%ymm9,%ymm8,%ymm8
vshufpd		$0x00,%ymm11,%ymm10,%ymm9
vshufpd		$0x0F,%ymm11,%ymm10,%ymm10

#update
vmovdqa		%ymm3,%ymm12
vmovdqa		%ymm5,%ymm13
vmovdqa		%ymm7,%ymm14
vmovdqa		%ymm9,%ymm15
vpaddw		%ymm3,%ymm4,%ymm3
vpaddw		%ymm5,%ymm6,%ymm5
vpaddw		%ymm7,%ymm8,%ymm7
vpaddw		%ymm9,%ymm10,%ymm9
vpsubw		%ymm4,%ymm12,%ymm4
vpsubw		%ymm6,%ymm13,%ymm6
vpsubw		%ymm8,%ymm14,%ymm8
vpsubw		%ymm10,%ymm15,%ymm10

#zetas
vmovdqa		768(%r8),%ymm12
vmovdqa		800(%r8),%ymm13
vmovdqa		832(%r8),%ymm14
vmovdqa		864(%r8),%ymm15

#mul
vpmullw		%ymm12,%ymm4,%ymm11
vpmulhw		%ymm12,%ymm4,%ymm4
vpmullw		%ymm13,%ymm6,%ymm12
vpmulhw		%ymm13,%ymm6,%ymm6
vpmullw		%ymm14,%ymm8,%ymm13
vpmulhw		%ymm14,%ymm8,%ymm8
vpmullw		%ymm15,%ymm10,%ymm14
vpmulhw		%ymm15,%ymm10,%ymm10

#reduce
vpmullw		%ymm0,%ymm11,%ymm11
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm11,%ymm4,%ymm4
vpsubw		%ymm12,%ymm6,%ymm6
vpsubw		%ymm13,%ymm8,%ymm8
vpsubw		%ymm14,%ymm10,%ymm10

#reduce 2
vmovdqa		_low_mask(%rip),%ymm11
vpaddw		%ymm2,%ymm3,%ymm3
vpaddw		%ymm2,%ymm5,%ymm5
vpaddw		%ymm2,%ymm7,%ymm7
vpaddw		%ymm2,%ymm9,%ymm9
vpsrlw		$13,%ymm3,%ymm12
vpsrlw		$13,%ymm5,%ymm13
vpsrlw		$13,%ymm7,%ymm14
vpsrlw		$13,%ymm9,%ymm15
#vpmullw	%ymm1,%ymm12,%ymm12
#vpmullw	%ymm1,%ymm13,%ymm13
#vpmullw	%ymm1,%ymm14,%ymm14
#vpmullw	%ymm1,%ymm15,%ymm15
vpand		%ymm11,%ymm3,%ymm3
vpand		%ymm11,%ymm5,%ymm5
vpand		%ymm11,%ymm7,%ymm7
vpand		%ymm11,%ymm9,%ymm9
vpsubw		%ymm12,%ymm3,%ymm3
vpsubw		%ymm13,%ymm5,%ymm5
vpsubw		%ymm14,%ymm7,%ymm7
vpsubw		%ymm15,%ymm9,%ymm9
vpsllw		$9,%ymm12,%ymm12
vpsllw		$9,%ymm13,%ymm13
vpsllw		$9,%ymm14,%ymm14
vpsllw		$9,%ymm15,%ymm15
vpaddw		%ymm12,%ymm3,%ymm3
vpaddw		%ymm13,%ymm5,%ymm5
vpaddw		%ymm14,%ymm7,%ymm7
vpaddw		%ymm15,%ymm9,%ymm9
vpsubw		%ymm1,%ymm3,%ymm3
vpsubw		%ymm1,%ymm5,%ymm5
vpsubw		%ymm1,%ymm7,%ymm7
vpsubw		%ymm1,%ymm9,%ymm9

#level 4
#shuffle
vperm2i128	$0x02,%ymm3,%ymm4,%ymm11
vperm2i128	$0x13,%ymm3,%ymm4,%ymm3
vperm2i128	$0x02,%ymm5,%ymm6,%ymm4
vperm2i128	$0x13,%ymm5,%ymm6,%ymm5
vperm2i128	$0x02,%ymm7,%ymm8,%ymm6
vperm2i128	$0x13,%ymm7,%ymm8,%ymm7
vperm2i128	$0x02,%ymm9,%ymm10,%ymm8
vperm2i128	$0x13,%ymm9,%ymm10,%ymm9

#update
vmovdqa		%ymm11,%ymm12
vmovdqa		%ymm4,%ymm13
vmovdqa		%ymm6,%ymm14
vmovdqa		%ymm8,%ymm15
vpaddw		%ymm11,%ymm3,%ymm10
vpaddw		%ymm4,%ymm5,%ymm4
vpaddw		%ymm6,%ymm7,%ymm6
vpaddw		%ymm8,%ymm9,%ymm8
vpsubw		%ymm3,%ymm12,%ymm3
vpsubw		%ymm5,%ymm13,%ymm5
vpsubw		%ymm7,%ymm14,%ymm7
vpsubw		%ymm9,%ymm15,%ymm9

#zetas
vmovdqa		1024(%r8),%ymm12
vmovdqa		1056(%r8),%ymm13
vmovdqa		1088(%r8),%ymm14
vmovdqa		1120(%r8),%ymm15

#mul
vpmullw		%ymm12,%ymm3,%ymm11
vpmulhw		%ymm12,%ymm3,%ymm3
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9

#reduce
vpmullw		%ymm0,%ymm11,%ymm11
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm11,%ymm3,%ymm3
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm7,%ymm7
vpsubw		%ymm14,%ymm9,%ymm9

#level 5
#update
vmovdqa		%ymm10,%ymm12
vmovdqa		%ymm3,%ymm13
vmovdqa		%ymm6,%ymm14
vmovdqa		%ymm7,%ymm15
vpaddw		%ymm10,%ymm4,%ymm10
vpaddw		%ymm3,%ymm5,%ymm3
vpaddw		%ymm6,%ymm8,%ymm6
vpaddw		%ymm7,%ymm9,%ymm7
vpsubw		%ymm4,%ymm12,%ymm4
vpsubw		%ymm5,%ymm13,%ymm5
vpsubw		%ymm8,%ymm14,%ymm8
vpsubw		%ymm9,%ymm15,%ymm9

#zetas
vmovdqa		1280(%rdx),%ymm14
vmovdqa		1312(%rdx),%ymm15

#mul
vpmullw		%ymm14,%ymm4,%ymm11
vpmullw		%ymm14,%ymm5,%ymm12
vpmullw		%ymm15,%ymm8,%ymm13
vpmulhw		%ymm14,%ymm4,%ymm4
vpmulhw		%ymm14,%ymm5,%ymm5
vpmulhw		%ymm15,%ymm8,%ymm8
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9

#reduce
vpmullw		%ymm0,%ymm11,%ymm11
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm11,%ymm4,%ymm4
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm8,%ymm8
vpsubw		%ymm14,%ymm9,%ymm9

#reduce 2
vmovdqa		_low_mask(%rip),%ymm11
vpaddw		%ymm2,%ymm10,%ymm10
vpaddw		%ymm2,%ymm6,%ymm6
vpsrlw		$13,%ymm10,%ymm12
vpsrlw		$13,%ymm6,%ymm13
#vpmullw	%ymm1,%ymm12,%ymm12
#vpmullw	%ymm1,%ymm13,%ymm13
vpand		%ymm11,%ymm10,%ymm10
vpand		%ymm11,%ymm6,%ymm6
vpsubw		%ymm12,%ymm10,%ymm10
vpsubw		%ymm13,%ymm6,%ymm6
vpsllw		$9,%ymm12,%ymm12
vpsllw		$9,%ymm13,%ymm13
vpaddw		%ymm12,%ymm10,%ymm10
vpaddw		%ymm13,%ymm6,%ymm6
vpsubw		%ymm1,%ymm10,%ymm10
vpsubw		%ymm1,%ymm6,%ymm6

#level 6
#update
vmovdqa		%ymm10,%ymm12
vmovdqa		%ymm3,%ymm13
vmovdqa		%ymm4,%ymm14
vmovdqa		%ymm5,%ymm15
vpaddw		%ymm10,%ymm6,%ymm10
vpaddw		%ymm3,%ymm7,%ymm3
vpaddw		%ymm4,%ymm8,%ymm4
vpaddw		%ymm5,%ymm9,%ymm5
vpsubw		%ymm6,%ymm12,%ymm6
vpsubw		%ymm7,%ymm13,%ymm7
vpsubw		%ymm8,%ymm14,%ymm8
vpsubw		%ymm9,%ymm15,%ymm9

#zetas
vmovdqa		1408(%rdx),%ymm15

#mul
vpmullw		%ymm15,%ymm6,%ymm11
vpmullw		%ymm15,%ymm7,%ymm12
vpmullw		%ymm15,%ymm8,%ymm13
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm6,%ymm6
vpmulhw		%ymm15,%ymm7,%ymm7
vpmulhw		%ymm15,%ymm8,%ymm8
vpmulhw		%ymm15,%ymm9,%ymm9

#reduce
vpmullw		%ymm0,%ymm11,%ymm11
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm11,%ymm6,%ymm6
vpsubw		%ymm12,%ymm7,%ymm7
vpsubw		%ymm13,%ymm8,%ymm8
vpsubw		%ymm14,%ymm9,%ymm9

#reduce 2
vmovdqa		_low_mask(%rip),%ymm11
vpaddw		%ymm2,%ymm3,%ymm3
vpsrlw		$13,%ymm3,%ymm12
#vpmullw	%ymm1,%ymm12,%ymm12
vpand		%ymm11,%ymm3,%ymm3
vpsubw		%ymm12,%ymm3,%ymm3
vpsllw		$9,%ymm12,%ymm12
vpaddw		%ymm12,%ymm3,%ymm3
vpsubw		%ymm1,%ymm3,%ymm3

#store
vmovdqa		%ymm10,(%rdi)
vmovdqa		%ymm3,32(%rdi)
vmovdqa		%ymm4,64(%rdi)
vmovdqa		%ymm5,96(%rdi)
vmovdqa		%ymm6,128(%rdi)
vmovdqa		%ymm7,160(%rdi)
vmovdqa		%ymm8,192(%rdi)
vmovdqa		%ymm9,224(%rdi)

add		$256,%rsi
add		$256,%rdi
add		$128,%r8

#second round
#load
vmovdqa		(%rsi),%ymm4
vmovdqa		32(%rsi),%ymm5
vmovdqa		64(%rsi),%ymm6
vmovdqa		96(%rsi),%ymm7
vmovdqa		128(%rsi),%ymm8
vmovdqa		160(%rsi),%ymm9
vmovdqa		192(%rsi),%ymm10
vmovdqa		224(%rsi),%ymm11

vmovdqu		_lowdword(%rip),%ymm3

# Establish required permutation
vpand %ymm4, %ymm3, %ymm14
vpand %ymm5, %ymm3, %ymm15
vpsrad $16,%ymm4,%ymm4
vpsrad $16,%ymm5,%ymm5
vpackssdw %ymm5, %ymm4, %ymm5
vpackusdw %ymm15, %ymm14, %ymm4
vpermq $0xd8,%ymm4,%ymm4
vpermq $0xd8,%ymm5,%ymm5

vpand %ymm6, %ymm3, %ymm14
vpand %ymm7, %ymm3, %ymm15
vpsrad $16,%ymm6,%ymm6
vpsrad $16,%ymm7,%ymm7
vpackssdw %ymm7, %ymm6, %ymm7
vpackusdw %ymm15, %ymm14, %ymm6
vpermq $0xd8,%ymm6,%ymm6
vpermq $0xd8,%ymm7,%ymm7

vpand %ymm8, %ymm3, %ymm14
vpand %ymm9, %ymm3, %ymm15
vpsrad $16,%ymm8,%ymm8
vpsrad $16,%ymm9,%ymm9
vpackssdw %ymm9, %ymm8, %ymm9
vpackusdw %ymm15, %ymm14, %ymm8
vpermq $0xd8,%ymm8,%ymm8
vpermq $0xd8,%ymm9,%ymm9

vpand %ymm10, %ymm3, %ymm14
vpand %ymm11, %ymm3, %ymm15
vpsrad $16,%ymm10,%ymm10
vpsrad $16,%ymm11,%ymm11
vpackssdw %ymm11, %ymm10, %ymm11
vpackusdw %ymm15, %ymm14, %ymm10
vpermq $0xd8,%ymm10,%ymm10
vpermq $0xd8,%ymm11,%ymm11


#level 0
#update
vmovdqa		%ymm4,%ymm12
vmovdqa		%ymm6,%ymm13
vmovdqa		%ymm8,%ymm14
vmovdqa		%ymm10,%ymm15
vpaddw		%ymm4,%ymm5,%ymm4
vpaddw		%ymm6,%ymm7,%ymm6
vpaddw		%ymm8,%ymm9,%ymm8
vpaddw		%ymm10,%ymm11,%ymm10
vpsubw		%ymm5,%ymm12,%ymm5
vpsubw		%ymm7,%ymm13,%ymm7
vpsubw		%ymm9,%ymm14,%ymm9
vpsubw		%ymm11,%ymm15,%ymm11

#zetas
vmovdqa		(%r8),%ymm13
vmovdqa		32(%r8),%ymm14
vmovdqa		64(%r8),%ymm15
vmovdqa		96(%r8),%ymm3

#mul
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9
vpmullw		%ymm3,%ymm11,%ymm15
vpmulhw		%ymm3,%ymm11,%ymm11

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm7,%ymm7
vpsubw		%ymm14,%ymm9,%ymm9
vpsubw		%ymm15,%ymm11,%ymm11

#level 1
#shuffle
vmovdqa		_vpshufb_idx(%rip),%ymm3
vpshufb		%ymm3,%ymm4,%ymm12
vpshufb		%ymm3,%ymm5,%ymm13
vpshufb		%ymm3,%ymm6,%ymm14
vpshufb		%ymm3,%ymm7,%ymm15
vpblendw	$0x55,%ymm4,%ymm13,%ymm4
vpblendw	$0xAA,%ymm5,%ymm12,%ymm5
vpblendw	$0x55,%ymm6,%ymm15,%ymm6
vpblendw	$0xAA,%ymm7,%ymm14,%ymm7
vpshufb		%ymm3,%ymm8,%ymm12
vpshufb		%ymm3,%ymm9,%ymm13
vpshufb		%ymm3,%ymm10,%ymm14
vpshufb		%ymm3,%ymm11,%ymm15
vpblendw	$0x55,%ymm8,%ymm13,%ymm8
vpblendw	$0xAA,%ymm9,%ymm12,%ymm9
vpblendw	$0x55,%ymm10,%ymm15,%ymm10
vpblendw	$0xAA,%ymm11,%ymm14,%ymm11

#update
vmovdqa		%ymm4,%ymm12
vmovdqa		%ymm6,%ymm13
vmovdqa		%ymm8,%ymm14
vmovdqa		%ymm10,%ymm15
vpaddw		%ymm4,%ymm5,%ymm4
vpaddw		%ymm6,%ymm7,%ymm6
vpaddw		%ymm8,%ymm9,%ymm8
vpaddw		%ymm10,%ymm11,%ymm10
vpsubw		%ymm5,%ymm12,%ymm5
vpsubw		%ymm7,%ymm13,%ymm7
vpsubw		%ymm9,%ymm14,%ymm9
vpsubw		%ymm11,%ymm15,%ymm11

#zetas
vmovdqa		256(%r8),%ymm13
vmovdqa		288(%r8),%ymm14
vmovdqa		320(%r8),%ymm15
vmovdqa		352(%r8),%ymm3

#mul
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9
vpmullw		%ymm3,%ymm11,%ymm15
vpmulhw		%ymm3,%ymm11,%ymm11

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm7,%ymm7
vpsubw		%ymm14,%ymm9,%ymm9
vpsubw		%ymm15,%ymm11,%ymm11

#reduce 2
vmovdqa		_low_mask(%rip),%ymm3
vpaddw		%ymm2,%ymm4,%ymm4
vpaddw		%ymm2,%ymm6,%ymm6
vpaddw		%ymm2,%ymm8,%ymm8
vpaddw		%ymm2,%ymm10,%ymm10
vpsrlw		$13,%ymm4,%ymm12
vpsrlw		$13,%ymm6,%ymm13
vpsrlw		$13,%ymm8,%ymm14
vpsrlw		$13,%ymm10,%ymm15
#vpmullw	%ymm1,%ymm12,%ymm12
#vpmullw	%ymm1,%ymm13,%ymm13
#vpmullw	%ymm1,%ymm14,%ymm14
#vpmullw	%ymm1,%ymm15,%ymm15
vpand		%ymm3,%ymm4,%ymm4
vpand		%ymm3,%ymm6,%ymm6
vpand		%ymm3,%ymm8,%ymm8
vpand		%ymm3,%ymm10,%ymm10
vpsubw		%ymm12,%ymm4,%ymm4
vpsubw		%ymm13,%ymm6,%ymm6
vpsubw		%ymm14,%ymm8,%ymm8
vpsubw		%ymm15,%ymm10,%ymm10
vpsllw		$9,%ymm12,%ymm12
vpsllw		$9,%ymm13,%ymm13
vpsllw		$9,%ymm14,%ymm14
vpsllw		$9,%ymm15,%ymm15
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8
vpaddw		%ymm15,%ymm10,%ymm10
vpsubw		%ymm1,%ymm4,%ymm4
vpsubw		%ymm1,%ymm6,%ymm6
vpsubw		%ymm1,%ymm8,%ymm8
vpsubw		%ymm1,%ymm10,%ymm10

#level 2
#shuffle
vpshufd		$0xB1,%ymm4,%ymm12
vpshufd		$0xB1,%ymm5,%ymm13
vpshufd		$0xB1,%ymm6,%ymm14
vpshufd		$0xB1,%ymm7,%ymm15
vpblendd	$0x55,%ymm4,%ymm13,%ymm4
vpblendd	$0xAA,%ymm5,%ymm12,%ymm5
vpblendd	$0x55,%ymm6,%ymm15,%ymm6
vpblendd	$0xAA,%ymm7,%ymm14,%ymm7
vpshufd		$0xB1,%ymm8,%ymm12
vpshufd		$0xB1,%ymm9,%ymm13
vpshufd		$0xB1,%ymm10,%ymm14
vpshufd		$0xB1,%ymm11,%ymm15
vpblendd	$0x55,%ymm8,%ymm13,%ymm8
vpblendd	$0xAA,%ymm9,%ymm12,%ymm9
vpblendd	$0x55,%ymm10,%ymm15,%ymm10
vpblendd	$0xAA,%ymm11,%ymm14,%ymm11

#update
vmovdqa		%ymm4,%ymm12
vmovdqa		%ymm6,%ymm13
vmovdqa		%ymm8,%ymm14
vmovdqa		%ymm10,%ymm15
vpaddw		%ymm4,%ymm5,%ymm4
vpaddw		%ymm6,%ymm7,%ymm6
vpaddw		%ymm8,%ymm9,%ymm8
vpaddw		%ymm10,%ymm11,%ymm10
vpsubw		%ymm5,%ymm12,%ymm5
vpsubw		%ymm7,%ymm13,%ymm7
vpsubw		%ymm9,%ymm14,%ymm9
vpsubw		%ymm11,%ymm15,%ymm11

#zetas
vmovdqa		512(%r8),%ymm13
vmovdqa		544(%r8),%ymm14
vmovdqa		576(%r8),%ymm15
vmovdqa		608(%r8),%ymm3

#mul
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9
vpmullw		%ymm3,%ymm11,%ymm15
vpmulhw		%ymm3,%ymm11,%ymm11

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm7,%ymm7
vpsubw		%ymm14,%ymm9,%ymm9
vpsubw		%ymm15,%ymm11,%ymm11

#level 3
#shuffle
vshufpd		$0x00,%ymm5,%ymm4,%ymm3
vshufpd		$0x0F,%ymm5,%ymm4,%ymm4
vshufpd		$0x00,%ymm7,%ymm6,%ymm5
vshufpd		$0x0F,%ymm7,%ymm6,%ymm6
vshufpd		$0x00,%ymm9,%ymm8,%ymm7
vshufpd		$0x0F,%ymm9,%ymm8,%ymm8
vshufpd		$0x00,%ymm11,%ymm10,%ymm9
vshufpd		$0x0F,%ymm11,%ymm10,%ymm10

#update
vmovdqa		%ymm3,%ymm12
vmovdqa		%ymm5,%ymm13
vmovdqa		%ymm7,%ymm14
vmovdqa		%ymm9,%ymm15
vpaddw		%ymm3,%ymm4,%ymm3
vpaddw		%ymm5,%ymm6,%ymm5
vpaddw		%ymm7,%ymm8,%ymm7
vpaddw		%ymm9,%ymm10,%ymm9
vpsubw		%ymm4,%ymm12,%ymm4
vpsubw		%ymm6,%ymm13,%ymm6
vpsubw		%ymm8,%ymm14,%ymm8
vpsubw		%ymm10,%ymm15,%ymm10

#zetas
vmovdqa		768(%r8),%ymm12
vmovdqa		800(%r8),%ymm13
vmovdqa		832(%r8),%ymm14
vmovdqa		864(%r8),%ymm15

#mul
vpmullw		%ymm12,%ymm4,%ymm11
vpmulhw		%ymm12,%ymm4,%ymm4
vpmullw		%ymm13,%ymm6,%ymm12
vpmulhw		%ymm13,%ymm6,%ymm6
vpmullw		%ymm14,%ymm8,%ymm13
vpmulhw		%ymm14,%ymm8,%ymm8
vpmullw		%ymm15,%ymm10,%ymm14
vpmulhw		%ymm15,%ymm10,%ymm10

#reduce
vpmullw		%ymm0,%ymm11,%ymm11
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm11,%ymm4,%ymm4
vpsubw		%ymm12,%ymm6,%ymm6
vpsubw		%ymm13,%ymm8,%ymm8
vpsubw		%ymm14,%ymm10,%ymm10

#reduce 2
vmovdqa		_low_mask(%rip),%ymm11
vpaddw		%ymm2,%ymm3,%ymm3
vpaddw		%ymm2,%ymm5,%ymm5
vpaddw		%ymm2,%ymm7,%ymm7
vpaddw		%ymm2,%ymm9,%ymm9
vpsrlw		$13,%ymm3,%ymm12
vpsrlw		$13,%ymm5,%ymm13
vpsrlw		$13,%ymm7,%ymm14
vpsrlw		$13,%ymm9,%ymm15
#vpmullw	%ymm1,%ymm12,%ymm12
#vpmullw	%ymm1,%ymm13,%ymm13
#vpmullw	%ymm1,%ymm14,%ymm14
#vpmullw	%ymm1,%ymm15,%ymm15
vpand		%ymm11,%ymm3,%ymm3
vpand		%ymm11,%ymm5,%ymm5
vpand		%ymm11,%ymm7,%ymm7
vpand		%ymm11,%ymm9,%ymm9
vpsubw		%ymm12,%ymm3,%ymm3
vpsubw		%ymm13,%ymm5,%ymm5
vpsubw		%ymm14,%ymm7,%ymm7
vpsubw		%ymm15,%ymm9,%ymm9
vpsllw		$9,%ymm12,%ymm12
vpsllw		$9,%ymm13,%ymm13
vpsllw		$9,%ymm14,%ymm14
vpsllw		$9,%ymm15,%ymm15
vpaddw		%ymm12,%ymm3,%ymm3
vpaddw		%ymm13,%ymm5,%ymm5
vpaddw		%ymm14,%ymm7,%ymm7
vpaddw		%ymm15,%ymm9,%ymm9
vpsubw		%ymm1,%ymm3,%ymm3
vpsubw		%ymm1,%ymm5,%ymm5
vpsubw		%ymm1,%ymm7,%ymm7
vpsubw		%ymm1,%ymm9,%ymm9

#level 4
#shuffle
vperm2i128	$0x02,%ymm3,%ymm4,%ymm11
vperm2i128	$0x13,%ymm3,%ymm4,%ymm3
vperm2i128	$0x02,%ymm5,%ymm6,%ymm4
vperm2i128	$0x13,%ymm5,%ymm6,%ymm5
vperm2i128	$0x02,%ymm7,%ymm8,%ymm6
vperm2i128	$0x13,%ymm7,%ymm8,%ymm7
vperm2i128	$0x02,%ymm9,%ymm10,%ymm8
vperm2i128	$0x13,%ymm9,%ymm10,%ymm9

#update
vmovdqa		%ymm11,%ymm12
vmovdqa		%ymm4,%ymm13
vmovdqa		%ymm6,%ymm14
vmovdqa		%ymm8,%ymm15
vpaddw		%ymm11,%ymm3,%ymm10
vpaddw		%ymm4,%ymm5,%ymm4
vpaddw		%ymm6,%ymm7,%ymm6
vpaddw		%ymm8,%ymm9,%ymm8
vpsubw		%ymm3,%ymm12,%ymm3
vpsubw		%ymm5,%ymm13,%ymm5
vpsubw		%ymm7,%ymm14,%ymm7
vpsubw		%ymm9,%ymm15,%ymm9

#zetas
vmovdqa		1024(%r8),%ymm12
vmovdqa		1056(%r8),%ymm13
vmovdqa		1088(%r8),%ymm14
vmovdqa		1120(%r8),%ymm15

#mul
vpmullw		%ymm12,%ymm3,%ymm11
vpmulhw		%ymm12,%ymm3,%ymm3
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9

#reduce
vpmullw		%ymm0,%ymm11,%ymm11
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm11,%ymm3,%ymm3
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm7,%ymm7
vpsubw		%ymm14,%ymm9,%ymm9

#level 5
#update
vmovdqa		%ymm10,%ymm12
vmovdqa		%ymm3,%ymm13
vmovdqa		%ymm6,%ymm14
vmovdqa		%ymm7,%ymm15
vpaddw		%ymm10,%ymm4,%ymm10
vpaddw		%ymm3,%ymm5,%ymm3
vpaddw		%ymm6,%ymm8,%ymm6
vpaddw		%ymm7,%ymm9,%ymm7
vpsubw		%ymm4,%ymm12,%ymm4
vpsubw		%ymm5,%ymm13,%ymm5
vpsubw		%ymm8,%ymm14,%ymm8
vpsubw		%ymm9,%ymm15,%ymm9

#zetas
vmovdqa		1344(%rdx),%ymm14
vmovdqa		1376(%rdx),%ymm15

#mul
vpmullw		%ymm14,%ymm4,%ymm11
vpmullw		%ymm14,%ymm5,%ymm12
vpmullw		%ymm15,%ymm8,%ymm13
vpmulhw		%ymm14,%ymm4,%ymm4
vpmulhw		%ymm14,%ymm5,%ymm5
vpmulhw		%ymm15,%ymm8,%ymm8
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9

#reduce
vpmullw		%ymm0,%ymm11,%ymm11
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm11,%ymm4,%ymm4
vpsubw		%ymm12,%ymm5,%ymm5
vpsubw		%ymm13,%ymm8,%ymm8
vpsubw		%ymm14,%ymm9,%ymm9

#reduce 2
vmovdqa		_low_mask(%rip),%ymm11
vpaddw		%ymm2,%ymm10,%ymm10
vpaddw		%ymm2,%ymm6,%ymm6
vpsrlw		$13,%ymm10,%ymm12
vpsrlw		$13,%ymm6,%ymm13
#vpmullw	%ymm1,%ymm12,%ymm12
#vpmullw	%ymm1,%ymm13,%ymm13
vpand		%ymm11,%ymm10,%ymm10
vpand		%ymm11,%ymm6,%ymm6
vpsubw		%ymm12,%ymm10,%ymm10
vpsubw		%ymm13,%ymm6,%ymm6
vpsllw		$9,%ymm12,%ymm12
vpsllw		$9,%ymm13,%ymm13
vpaddw		%ymm12,%ymm10,%ymm10
vpaddw		%ymm13,%ymm6,%ymm6
vpsubw		%ymm1,%ymm10,%ymm10
vpsubw		%ymm1,%ymm6,%ymm6

#level 6
#update
vmovdqa		%ymm10,%ymm12
vmovdqa		%ymm3,%ymm13
vmovdqa		%ymm4,%ymm14
vmovdqa		%ymm5,%ymm15
vpaddw		%ymm10,%ymm6,%ymm10
vpaddw		%ymm3,%ymm7,%ymm3
vpaddw		%ymm4,%ymm8,%ymm4
vpaddw		%ymm5,%ymm9,%ymm5
vpsubw		%ymm6,%ymm12,%ymm6
vpsubw		%ymm7,%ymm13,%ymm7
vpsubw		%ymm8,%ymm14,%ymm8
vpsubw		%ymm9,%ymm15,%ymm9

#zetas
vmovdqa		1440(%rdx),%ymm15

#mul
vpmullw		%ymm15,%ymm6,%ymm11
vpmullw		%ymm15,%ymm7,%ymm12
vpmullw		%ymm15,%ymm8,%ymm13
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm6,%ymm6
vpmulhw		%ymm15,%ymm7,%ymm7
vpmulhw		%ymm15,%ymm8,%ymm8
vpmulhw		%ymm15,%ymm9,%ymm9

#reduce
vpmullw		%ymm0,%ymm11,%ymm11
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm11,%ymm6,%ymm6
vpsubw		%ymm12,%ymm7,%ymm7
vpsubw		%ymm13,%ymm8,%ymm8
vpsubw		%ymm14,%ymm9,%ymm9

#reduce 2
vmovdqa		_low_mask(%rip),%ymm11
vpaddw		%ymm2,%ymm3,%ymm3
vpsrlw		$13,%ymm3,%ymm12
#vpmullw	%ymm1,%ymm12,%ymm12
vpand		%ymm11,%ymm3,%ymm3
vpsubw		%ymm12,%ymm3,%ymm3
vpsllw		$9,%ymm12,%ymm12
vpaddw		%ymm12,%ymm3,%ymm3
vpsubw		%ymm1,%ymm3,%ymm3

#store
vmovdqa		%ymm10,(%rdi)
vmovdqa		%ymm3,32(%rdi)
vmovdqa		%ymm4,64(%rdi)
vmovdqa		%ymm5,96(%rdi)
vmovdqa		%ymm6,128(%rdi)
vmovdqa		%ymm7,160(%rdi)
vmovdqa		%ymm8,192(%rdi)
vmovdqa		%ymm9,224(%rdi)

sub		$256,%rdi

#first round
#load
vmovdqa		(%rdi),%ymm4
vmovdqa		32(%rdi),%ymm5
vmovdqa		64(%rdi),%ymm6
vmovdqa		96(%rdi),%ymm7
vmovdqa		256(%rdi),%ymm8
vmovdqa		288(%rdi),%ymm9
vmovdqa		320(%rdi),%ymm10
vmovdqa		352(%rdi),%ymm11

#level 7
#update
vmovdqa		%ymm4,%ymm12
vmovdqa		%ymm5,%ymm13
vmovdqa		%ymm6,%ymm14
vmovdqa		%ymm7,%ymm15
vpaddw		%ymm4,%ymm8,%ymm4
vpaddw		%ymm5,%ymm9,%ymm5
vpaddw		%ymm6,%ymm10,%ymm6
vpaddw		%ymm7,%ymm11,%ymm7
vpsubw		%ymm8,%ymm12,%ymm8
vpsubw		%ymm9,%ymm13,%ymm9
vpsubw		%ymm10,%ymm14,%ymm10
vpsubw		%ymm11,%ymm15,%ymm11

#zeta
vmovdqa		1472(%rdx),%ymm3

#mul
vpmullw		%ymm3,%ymm8,%ymm12
vpmullw		%ymm3,%ymm9,%ymm13
vpmullw		%ymm3,%ymm10,%ymm14
vpmullw		%ymm3,%ymm11,%ymm15
vpmulhw		%ymm3,%ymm8,%ymm8
vpmulhw		%ymm3,%ymm9,%ymm9
vpmulhw		%ymm3,%ymm10,%ymm10
vpmulhw		%ymm3,%ymm11,%ymm11

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpaddw		%ymm1,%ymm8,%ymm8
vpaddw		%ymm1,%ymm9,%ymm9
vpaddw		%ymm1,%ymm10,%ymm10
vpaddw		%ymm1,%ymm11,%ymm11
vpsubw		%ymm12,%ymm8,%ymm8
vpsubw		%ymm13,%ymm9,%ymm9
vpsubw		%ymm14,%ymm10,%ymm10
vpsubw		%ymm15,%ymm11,%ymm11

#reduce 2
#vmovdqa	_low_mask(%rip),%ymm11
#vpaddw		%ymm2,%ymm4,%ymm4
#vpaddw		%ymm2,%ymm6,%ymm6
#vpaddw		%ymm2,%ymm7,%ymm7
#vpsrlw		$13,%ymm4,%ymm12
#vpsrlw		$13,%ymm6,%ymm13
#vpsrlw		$13,%ymm7,%ymm14
#vpmullw	%ymm1,%ymm12,%ymm12
#vpmullw	%ymm1,%ymm13,%ymm13
#vpmullw	%ymm1,%ymm14,%ymm14
#vpand		%ymm11,%ymm4,%ymm4
#vpand		%ymm11,%ymm6,%ymm6
#vpand		%ymm11,%ymm7,%ymm7
#vpsubw		%ymm12,%ymm4,%ymm4
#vpsubw		%ymm13,%ymm6,%ymm6
#vpsubw		%ymm14,%ymm7,%ymm7
#vpsllw		$9,%ymm12,%ymm12
#vpsllw		$9,%ymm13,%ymm13
#vpsllw		$9,%ymm14,%ymm14
#vpaddw		%ymm12,%ymm4,%ymm4
#vpaddw		%ymm13,%ymm6,%ymm6
#vpaddw		%ymm14,%ymm7,%ymm7
#vpsubw		%ymm1,%ymm4,%ymm4
#vpsubw		%ymm1,%ymm6,%ymm6
#vpsubw		%ymm1,%ymm7,%ymm7

#f
vmovdqa		_f(%rip),%ymm3

#mul
vpmullw		%ymm3,%ymm4,%ymm12
vpmullw		%ymm3,%ymm5,%ymm13
vpmullw		%ymm3,%ymm6,%ymm14
vpmullw		%ymm3,%ymm7,%ymm15
vpmulhw		%ymm3,%ymm4,%ymm4
vpmulhw		%ymm3,%ymm5,%ymm5
vpmulhw		%ymm3,%ymm6,%ymm6
vpmulhw		%ymm3,%ymm7,%ymm7

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpaddw		%ymm1,%ymm4,%ymm4
vpaddw		%ymm1,%ymm5,%ymm5
vpaddw		%ymm1,%ymm6,%ymm6
vpaddw		%ymm1,%ymm7,%ymm7
vpsubw		%ymm12,%ymm4,%ymm4
vpsubw		%ymm13,%ymm5,%ymm5
vpsubw		%ymm14,%ymm6,%ymm6
vpsubw		%ymm15,%ymm7,%ymm7

#store
vmovdqa		%ymm4,(%rdi)
vmovdqa		%ymm5,32(%rdi)
vmovdqa		%ymm6,64(%rdi)
vmovdqa		%ymm7,96(%rdi)
vmovdqa		%ymm8,256(%rdi)
vmovdqa		%ymm9,288(%rdi)
vmovdqa		%ymm10,320(%rdi)
vmovdqa		%ymm11,352(%rdi)

add		$128,%rdi

#second round
#load
vmovdqa		(%rdi),%ymm4
vmovdqa		32(%rdi),%ymm5
vmovdqa		64(%rdi),%ymm6
vmovdqa		96(%rdi),%ymm7
vmovdqa		256(%rdi),%ymm8
vmovdqa		288(%rdi),%ymm9
vmovdqa		320(%rdi),%ymm10
vmovdqa		352(%rdi),%ymm11

#zeta
vmovdqa		1472(%rdx),%ymm3

#level 7
#update
vmovdqa		%ymm4,%ymm12
vmovdqa		%ymm5,%ymm13
vmovdqa		%ymm6,%ymm14
vmovdqa		%ymm7,%ymm15
vpaddw		%ymm4,%ymm8,%ymm4
vpaddw		%ymm5,%ymm9,%ymm5
vpaddw		%ymm6,%ymm10,%ymm6
vpaddw		%ymm7,%ymm11,%ymm7
vpsubw		%ymm8,%ymm12,%ymm8
vpsubw		%ymm9,%ymm13,%ymm9
vpsubw		%ymm10,%ymm14,%ymm10
vpsubw		%ymm11,%ymm15,%ymm11

#mul
vpmullw		%ymm3,%ymm8,%ymm12
vpmullw		%ymm3,%ymm9,%ymm13
vpmullw		%ymm3,%ymm10,%ymm14
vpmullw		%ymm3,%ymm11,%ymm15
vpmulhw		%ymm3,%ymm8,%ymm8
vpmulhw		%ymm3,%ymm9,%ymm9
vpmulhw		%ymm3,%ymm10,%ymm10
vpmulhw		%ymm3,%ymm11,%ymm11

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpaddw		%ymm1,%ymm8,%ymm8
vpaddw		%ymm1,%ymm9,%ymm9
vpaddw		%ymm1,%ymm10,%ymm10
vpaddw		%ymm1,%ymm11,%ymm11
vpsubw		%ymm12,%ymm8,%ymm8
vpsubw		%ymm13,%ymm9,%ymm9
vpsubw		%ymm14,%ymm10,%ymm10
vpsubw		%ymm15,%ymm11,%ymm11

#f
vmovdqa		_f(%rip),%ymm3

#mul
vpmullw		%ymm3,%ymm4,%ymm12
vpmullw		%ymm3,%ymm5,%ymm13
vpmullw		%ymm3,%ymm6,%ymm14
vpmullw		%ymm3,%ymm7,%ymm15
vpmulhw		%ymm3,%ymm4,%ymm4
vpmulhw		%ymm3,%ymm5,%ymm5
vpmulhw		%ymm3,%ymm6,%ymm6
vpmulhw		%ymm3,%ymm7,%ymm7

#reduce
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmullw		%ymm0,%ymm15,%ymm15
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm15,%ymm15
vpaddw		%ymm1,%ymm4,%ymm4
vpaddw		%ymm1,%ymm5,%ymm5
vpaddw		%ymm1,%ymm6,%ymm6
vpaddw		%ymm1,%ymm7,%ymm7
vpsubw		%ymm12,%ymm4,%ymm4
vpsubw		%ymm13,%ymm5,%ymm5
vpsubw		%ymm14,%ymm6,%ymm6
vpsubw		%ymm15,%ymm7,%ymm7

#store
vmovdqa		%ymm4,(%rdi)
vmovdqa		%ymm5,32(%rdi)
vmovdqa		%ymm6,64(%rdi)
vmovdqa		%ymm7,96(%rdi)
vmovdqa		%ymm8,256(%rdi)
vmovdqa		%ymm9,288(%rdi)
vmovdqa		%ymm10,320(%rdi)
vmovdqa		%ymm11,352(%rdi)

add 		%r11,%rsp
ret
