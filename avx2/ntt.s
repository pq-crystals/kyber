.global ntt
ntt:
mov		%rsp,%r11
and		$31,%r11
sub		%r11,%rsp

vmovdqa		_16xqinv(%rip),%ymm0
vmovdqa		_16xq(%rip),%ymm1
vmovdqa		_16x4q(%rip),%ymm2

#first round
#zetas
vmovdqa		(%rsi),%ymm3

#load
vmovdqa		(%rdi),%ymm4
vmovdqa		64(%rdi),%ymm5
vmovdqa		128(%rdi),%ymm6
vmovdqa		192(%rdi),%ymm7
vmovdqa		256(%rdi),%ymm8
vmovdqa		320(%rdi),%ymm9
vmovdqa		384(%rdi),%ymm10
vmovdqa		448(%rdi),%ymm11

#level 0
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
vpsubw		%ymm12,%ymm8,%ymm12
vpsubw		%ymm13,%ymm9,%ymm13
vpsubw		%ymm14,%ymm10,%ymm14
vpsubw		%ymm15,%ymm11,%ymm15

#update
vpsubw		%ymm12,%ymm4,%ymm8
vpsubw		%ymm13,%ymm5,%ymm9
vpsubw		%ymm14,%ymm6,%ymm10
vpsubw		%ymm15,%ymm7,%ymm11
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm5,%ymm5
vpaddw		%ymm14,%ymm6,%ymm6
vpaddw		%ymm15,%ymm7,%ymm7

#level 1
#zetas
vmovdqa		32(%rsi),%ymm15
vmovdqa		64(%rsi),%ymm3

#mul
vpmullw		%ymm15,%ymm6,%ymm12
vpmullw		%ymm15,%ymm7,%ymm13
vpmullw		%ymm3,%ymm10,%ymm14
vpmulhw		%ymm15,%ymm6,%ymm6
vpmulhw		%ymm15,%ymm7,%ymm7
vpmulhw		%ymm3,%ymm10,%ymm10
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
vpsubw		%ymm12,%ymm6,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm10,%ymm14
vpsubw		%ymm15,%ymm11,%ymm15

#update
vpsubw		%ymm12,%ymm4,%ymm6
vpsubw		%ymm13,%ymm5,%ymm7
vpsubw		%ymm14,%ymm8,%ymm10
vpsubw		%ymm15,%ymm9,%ymm11
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm5,%ymm5
vpaddw		%ymm14,%ymm8,%ymm8
vpaddw		%ymm15,%ymm9,%ymm9

#level 2
#zetas
vmovdqa		96(%rsi),%ymm13
vmovdqa		128(%rsi),%ymm14
vmovdqa		160(%rsi),%ymm15
vmovdqa		192(%rsi),%ymm3

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
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14
vpsubw		%ymm15,%ymm11,%ymm15

#update
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpsubw		%ymm15,%ymm10,%ymm11
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8
vpaddw		%ymm15,%ymm10,%ymm10

#store
vmovdqa		%ymm4,(%rdi)
vmovdqa		%ymm5,64(%rdi)
vmovdqa		%ymm6,128(%rdi)
vmovdqa		%ymm7,192(%rdi)
vmovdqa		%ymm8,256(%rdi)
vmovdqa		%ymm9,320(%rdi)
vmovdqa		%ymm10,384(%rdi)
vmovdqa		%ymm11,448(%rdi)

add		$32,%rdi

#second round
#zetas
vmovdqa		(%rsi),%ymm3

#load
vmovdqa		(%rdi),%ymm4
vmovdqa		64(%rdi),%ymm5
vmovdqa		128(%rdi),%ymm6
vmovdqa		192(%rdi),%ymm7
vmovdqa		256(%rdi),%ymm8
vmovdqa		320(%rdi),%ymm9
vmovdqa		384(%rdi),%ymm10
vmovdqa		448(%rdi),%ymm11

#level 0
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
vpsubw		%ymm12,%ymm8,%ymm12
vpsubw		%ymm13,%ymm9,%ymm13
vpsubw		%ymm14,%ymm10,%ymm14
vpsubw		%ymm15,%ymm11,%ymm15

#update
vpsubw		%ymm12,%ymm4,%ymm8
vpsubw		%ymm13,%ymm5,%ymm9
vpsubw		%ymm14,%ymm6,%ymm10
vpsubw		%ymm15,%ymm7,%ymm11
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm5,%ymm5
vpaddw		%ymm14,%ymm6,%ymm6
vpaddw		%ymm15,%ymm7,%ymm7

#level 1
#zetas
vmovdqa		32(%rsi),%ymm15
vmovdqa		64(%rsi),%ymm3

#mul
vpmullw		%ymm15,%ymm6,%ymm12
vpmullw		%ymm15,%ymm7,%ymm13
vpmullw		%ymm3,%ymm10,%ymm14
vpmulhw		%ymm15,%ymm6,%ymm6
vpmulhw		%ymm15,%ymm7,%ymm7
vpmulhw		%ymm3,%ymm10,%ymm10
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
vpsubw		%ymm12,%ymm6,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm10,%ymm14
vpsubw		%ymm15,%ymm11,%ymm15

#update
vpsubw		%ymm12,%ymm4,%ymm6
vpsubw		%ymm13,%ymm5,%ymm7
vpsubw		%ymm14,%ymm8,%ymm10
vpsubw		%ymm15,%ymm9,%ymm11
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm5,%ymm5
vpaddw		%ymm14,%ymm8,%ymm8
vpaddw		%ymm15,%ymm9,%ymm9

#level 2
#zetas
vmovdqa		96(%rsi),%ymm13
vmovdqa		128(%rsi),%ymm14
vmovdqa		160(%rsi),%ymm15
vmovdqa		192(%rsi),%ymm3

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
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14
vpsubw		%ymm15,%ymm11,%ymm15

#update
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpsubw		%ymm15,%ymm10,%ymm11
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8
vpaddw		%ymm15,%ymm10,%ymm10

#store
vmovdqa		%ymm4,(%rdi)
vmovdqa		%ymm5,64(%rdi)
vmovdqa		%ymm6,128(%rdi)
vmovdqa		%ymm7,192(%rdi)
vmovdqa		%ymm8,256(%rdi)
vmovdqa		%ymm9,320(%rdi)
vmovdqa		%ymm10,384(%rdi)
vmovdqa		%ymm11,448(%rdi)

add		$224,%rsi
sub		$32,%rdi

#first round
#zetas
vmovdqa		(%rsi),%ymm13
vmovdqa		32(%rsi),%ymm14
vmovdqa		64(%rsi),%ymm15
vmovdqa		96(%rsi),%ymm3

#load
vmovdqa		(%rdi),%ymm4
vmovdqa		32(%rdi),%ymm5
vmovdqa		64(%rdi),%ymm6
vmovdqa		96(%rdi),%ymm7
vmovdqa		128(%rdi),%ymm8
vmovdqa		160(%rdi),%ymm9
vmovdqa		192(%rdi),%ymm10
vmovdqa		224(%rdi),%ymm11

#level 3
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
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14
vpsubw		%ymm15,%ymm11,%ymm15

#reduce 2
vmovdqa		_low_mask(%rip),%ymm3
vpaddw		%ymm2,%ymm4,%ymm4
vpaddw		%ymm2,%ymm6,%ymm6
vpaddw		%ymm2,%ymm8,%ymm8
vpaddw		%ymm2,%ymm10,%ymm10
vpsrlw		$13,%ymm4,%ymm5
vpsrlw		$13,%ymm6,%ymm7
vpsrlw		$13,%ymm8,%ymm9
vpsrlw		$13,%ymm10,%ymm11
#vpmullw	%ymm1,%ymm5,%ymm5
#vpmullw	%ymm1,%ymm7,%ymm7
#vpmullw	%ymm1,%ymm9,%ymm9
#vpmullw	%ymm1,%ymm11,%ymm11
vpand		%ymm3,%ymm4,%ymm4
vpand		%ymm3,%ymm6,%ymm6
vpand		%ymm3,%ymm8,%ymm8
vpand		%ymm3,%ymm10,%ymm10
vpsubw		%ymm5,%ymm4,%ymm4
vpsubw		%ymm7,%ymm6,%ymm6
vpsubw		%ymm9,%ymm8,%ymm8
vpsubw		%ymm11,%ymm10,%ymm10
vpsllw		$9,%ymm5,%ymm5
vpsllw		$9,%ymm7,%ymm7
vpsllw		$9,%ymm9,%ymm9
vpsllw		$9,%ymm11,%ymm11
vpaddw		%ymm5,%ymm4,%ymm4
vpaddw		%ymm7,%ymm6,%ymm6
vpaddw		%ymm9,%ymm8,%ymm8
vpaddw		%ymm11,%ymm10,%ymm10
vpsubw		%ymm1,%ymm4,%ymm4
vpsubw		%ymm1,%ymm6,%ymm6
vpsubw		%ymm1,%ymm8,%ymm8
vpsubw		%ymm1,%ymm10,%ymm10

#update
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpsubw		%ymm15,%ymm10,%ymm11
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8
vpaddw		%ymm15,%ymm10,%ymm10

#level 4
#zetas
vmovdqa		256(%rsi),%ymm12
vmovdqa		288(%rsi),%ymm13
vmovdqa		320(%rsi),%ymm14
vmovdqa		352(%rsi),%ymm15

#shuffle
vperm2i128	$0x02,%ymm4,%ymm5,%ymm3
vperm2i128	$0x13,%ymm4,%ymm5,%ymm4
vperm2i128	$0x02,%ymm6,%ymm7,%ymm5
vperm2i128	$0x13,%ymm6,%ymm7,%ymm6
vperm2i128	$0x02,%ymm8,%ymm9,%ymm7
vperm2i128	$0x13,%ymm8,%ymm9,%ymm8
vperm2i128	$0x02,%ymm10,%ymm11,%ymm9
vperm2i128	$0x13,%ymm10,%ymm11,%ymm10

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
vpsubw		%ymm11,%ymm4,%ymm11
vpsubw		%ymm12,%ymm6,%ymm12
vpsubw		%ymm13,%ymm8,%ymm13
vpsubw		%ymm14,%ymm10,%ymm14

#update
vpsubw		%ymm11,%ymm3,%ymm4
vpsubw		%ymm12,%ymm5,%ymm6
vpsubw		%ymm13,%ymm7,%ymm8
vpsubw		%ymm14,%ymm9,%ymm10
vpaddw		%ymm11,%ymm3,%ymm3
vpaddw		%ymm12,%ymm5,%ymm5
vpaddw		%ymm13,%ymm7,%ymm7
vpaddw		%ymm14,%ymm9,%ymm9

#level 5
#zetas 1
vmovdqa		512(%rsi),%ymm12
vmovdqa		544(%rsi),%ymm13
vmovdqa		576(%rsi),%ymm14
vmovdqa		608(%rsi),%ymm15

#shuffle
vshufpd		$0x00,%ymm4,%ymm3,%ymm11
vshufpd		$0x0F,%ymm4,%ymm3,%ymm3
vshufpd		$0x00,%ymm6,%ymm5,%ymm4
vshufpd		$0x0F,%ymm6,%ymm5,%ymm5
vshufpd		$0x00,%ymm8,%ymm7,%ymm6
vshufpd		$0x0F,%ymm8,%ymm7,%ymm7
vshufpd		$0x00,%ymm10,%ymm9,%ymm8
vshufpd		$0x0F,%ymm10,%ymm9,%ymm9
#vmovdqa	%ymm11,%ymm10

#mul
vpmullw		%ymm12,%ymm3,%ymm10
vpmulhw		%ymm12,%ymm3,%ymm3
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9

#reduce
vpmullw		%ymm0,%ymm10,%ymm10
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm10,%ymm10
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm10,%ymm3,%ymm10
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14

#update
vpsubw		%ymm10,%ymm11,%ymm3
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpaddw		%ymm10,%ymm11,%ymm10
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8

#level 6
#shuffle
vpshufd		$0xB1,%ymm10,%ymm12
vpshufd		$0xB1,%ymm3,%ymm13
vpshufd		$0xB1,%ymm4,%ymm14
vpshufd		$0xB1,%ymm5,%ymm15
vpblendd	$0x55,%ymm10,%ymm13,%ymm10
vpblendd	$0xAA,%ymm3,%ymm12,%ymm3
vpblendd	$0x55,%ymm4,%ymm15,%ymm4
vpblendd	$0xAA,%ymm5,%ymm14,%ymm5
vpshufd		$0xB1,%ymm6,%ymm12
vpshufd		$0xB1,%ymm7,%ymm13
vpshufd		$0xB1,%ymm8,%ymm14
vpshufd		$0xB1,%ymm9,%ymm15
vpblendd	$0x55,%ymm6,%ymm13,%ymm6
vpblendd	$0xAA,%ymm7,%ymm12,%ymm7
vpblendd	$0x55,%ymm8,%ymm15,%ymm8
vpblendd	$0xAA,%ymm9,%ymm14,%ymm9

#zetas
vmovdqa		768(%rsi),%ymm12
vmovdqa		800(%rsi),%ymm13
vmovdqa		832(%rsi),%ymm14
vmovdqa		864(%rsi),%ymm15

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
vpsubw		%ymm11,%ymm3,%ymm11
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14

#reduce 2
vmovdqa		_low_mask(%rip),%ymm15
vpaddw		%ymm2,%ymm10,%ymm10
vpaddw		%ymm2,%ymm4,%ymm4
vpaddw		%ymm2,%ymm6,%ymm6
vpaddw		%ymm2,%ymm8,%ymm8
vpsrlw		$13,%ymm10,%ymm3
vpsrlw		$13,%ymm4,%ymm5
vpsrlw		$13,%ymm6,%ymm7
vpsrlw		$13,%ymm8,%ymm9
#vpmullw	%ymm1,%ymm3,%ymm3
#vpmullw	%ymm1,%ymm5,%ymm5
#vpmullw	%ymm1,%ymm7,%ymm7
#vpmullw	%ymm1,%ymm9,%ymm9
vpand		%ymm15,%ymm10,%ymm10
vpand		%ymm15,%ymm4,%ymm4
vpand		%ymm15,%ymm6,%ymm6
vpand		%ymm15,%ymm8,%ymm8
vpsubw		%ymm3,%ymm10,%ymm10
vpsubw		%ymm5,%ymm4,%ymm4
vpsubw		%ymm7,%ymm6,%ymm6
vpsubw		%ymm9,%ymm8,%ymm8
vpsllw		$9,%ymm3,%ymm3
vpsllw		$9,%ymm5,%ymm5
vpsllw		$9,%ymm7,%ymm7
vpsllw		$9,%ymm9,%ymm9
vpaddw		%ymm3,%ymm10,%ymm10
vpaddw		%ymm5,%ymm4,%ymm4
vpaddw		%ymm7,%ymm6,%ymm6
vpaddw		%ymm9,%ymm8,%ymm8
vpsubw		%ymm1,%ymm10,%ymm10
vpsubw		%ymm1,%ymm4,%ymm4
vpsubw		%ymm1,%ymm6,%ymm6
vpsubw		%ymm1,%ymm8,%ymm8

#update
vpsubw		%ymm11,%ymm10,%ymm3
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpaddw		%ymm11,%ymm10,%ymm10
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8

#level 7
vmovdqa		_vpshufb_idx(%rip),%ymm15
vpshufb		%ymm15,%ymm10,%ymm11
vpshufb		%ymm15,%ymm3,%ymm12
vpshufb		%ymm15,%ymm4,%ymm13
vpshufb		%ymm15,%ymm5,%ymm14
vpblendw	$0x55,%ymm10,%ymm12,%ymm10
vpblendw	$0xAA,%ymm3,%ymm11,%ymm3
vpblendw	$0x55,%ymm4,%ymm14,%ymm4
vpblendw	$0xAA,%ymm5,%ymm13,%ymm5
vpshufb		%ymm15,%ymm6,%ymm11
vpshufb		%ymm15,%ymm7,%ymm12
vpshufb		%ymm15,%ymm8,%ymm13
vpshufb		%ymm15,%ymm9,%ymm14
vpblendw	$0x55,%ymm6,%ymm12,%ymm6
vpblendw	$0xAA,%ymm7,%ymm11,%ymm7
vpblendw	$0x55,%ymm8,%ymm14,%ymm8
vpblendw	$0xAA,%ymm9,%ymm13,%ymm9

#zetas
vmovdqa		1024(%rsi),%ymm12
vmovdqa		1056(%rsi),%ymm13
vmovdqa		1088(%rsi),%ymm14
vmovdqa		1120(%rsi),%ymm15

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
vpsubw		%ymm11,%ymm3,%ymm11
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14

#update
vpsubw		%ymm11,%ymm10,%ymm3
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpaddw		%ymm11,%ymm10,%ymm10
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8

#store
//vmovdqa		%ymm10,(%rdi)
//vmovdqa		%ymm3,32(%rdi)
//vmovdqa		%ymm4,64(%rdi)
//vmovdqa		%ymm5,96(%rdi)
//vmovdqa		%ymm6,128(%rdi)
//vmovdqa		%ymm7,160(%rdi)
//vmovdqa		%ymm8,192(%rdi)
//vmovdqa		%ymm9,224(%rdi)

vpunpcklwd %ymm3, %ymm10, %ymm13
vpunpckhwd %ymm3, %ymm10, %ymm14
vmovdqa		_16x2q(%rip),%ymm3
vperm2i128 $0x20,%ymm14,%ymm13,%ymm15
vperm2i128 $0x31,%ymm14,%ymm13,%ymm14
vpaddw    %ymm3,%ymm15,%ymm15
vpaddw    %ymm3,%ymm14,%ymm14
vmovdqu		%ymm15,(%rdi)
vmovdqu		%ymm14,32(%rdi)

vpunpcklwd %ymm5, %ymm4, %ymm13
vpunpckhwd %ymm5, %ymm4, %ymm14
vperm2i128 $0x20,%ymm14,%ymm13,%ymm15
vperm2i128 $0x31,%ymm14,%ymm13,%ymm14
vpaddw    %ymm3,%ymm15,%ymm15
vpaddw    %ymm3,%ymm14,%ymm14
vmovdqu		%ymm15,64(%rdi)
vmovdqu		%ymm14,96(%rdi)

vpunpcklwd %ymm7, %ymm6, %ymm13
vpunpckhwd %ymm7, %ymm6, %ymm14
vperm2i128 $0x20,%ymm14,%ymm13,%ymm15
vperm2i128 $0x31,%ymm14,%ymm13,%ymm14
vpaddw    %ymm3,%ymm15,%ymm15
vpaddw    %ymm3,%ymm14,%ymm14
vmovdqu		%ymm15,128(%rdi)
vmovdqu		%ymm14,160(%rdi)

vpunpcklwd %ymm9, %ymm8, %ymm13
vpunpckhwd %ymm9, %ymm8, %ymm14
vperm2i128 $0x20,%ymm14,%ymm13,%ymm15
vperm2i128 $0x31,%ymm14,%ymm13,%ymm14
vpaddw    %ymm3,%ymm15,%ymm15
vpaddw    %ymm3,%ymm14,%ymm14
vmovdqu		%ymm15,192(%rdi)
vmovdqu		%ymm14,224(%rdi)

add		$128,%rsi
add		$256,%rdi

#second round
#zetas
vmovdqa		(%rsi),%ymm13
vmovdqa		32(%rsi),%ymm14
vmovdqa		64(%rsi),%ymm15
vmovdqa		96(%rsi),%ymm3

#load
vmovdqa		(%rdi),%ymm4
vmovdqa		32(%rdi),%ymm5
vmovdqa		64(%rdi),%ymm6
vmovdqa		96(%rdi),%ymm7
vmovdqa		128(%rdi),%ymm8
vmovdqa		160(%rdi),%ymm9
vmovdqa		192(%rdi),%ymm10
vmovdqa		224(%rdi),%ymm11

#level 3
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
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14
vpsubw		%ymm15,%ymm11,%ymm15

#reduce 2
vmovdqa		_low_mask(%rip),%ymm3
vpaddw		%ymm2,%ymm4,%ymm4
vpaddw		%ymm2,%ymm6,%ymm6
vpaddw		%ymm2,%ymm8,%ymm8
vpaddw		%ymm2,%ymm10,%ymm10
vpsrlw		$13,%ymm4,%ymm5
vpsrlw		$13,%ymm6,%ymm7
vpsrlw		$13,%ymm8,%ymm9
vpsrlw		$13,%ymm10,%ymm11
#vpmullw	%ymm1,%ymm5,%ymm5
#vpmullw	%ymm1,%ymm7,%ymm7
#vpmullw	%ymm1,%ymm9,%ymm9
#vpmullw	%ymm1,%ymm11,%ymm11
vpand		%ymm3,%ymm4,%ymm4
vpand		%ymm3,%ymm6,%ymm6
vpand		%ymm3,%ymm8,%ymm8
vpand		%ymm3,%ymm10,%ymm10
vpsubw		%ymm5,%ymm4,%ymm4
vpsubw		%ymm7,%ymm6,%ymm6
vpsubw		%ymm9,%ymm8,%ymm8
vpsubw		%ymm11,%ymm10,%ymm10
vpsllw		$9,%ymm5,%ymm5
vpsllw		$9,%ymm7,%ymm7
vpsllw		$9,%ymm9,%ymm9
vpsllw		$9,%ymm11,%ymm11
vpaddw		%ymm5,%ymm4,%ymm4
vpaddw		%ymm7,%ymm6,%ymm6
vpaddw		%ymm9,%ymm8,%ymm8
vpaddw		%ymm11,%ymm10,%ymm10
vpsubw		%ymm1,%ymm4,%ymm4
vpsubw		%ymm1,%ymm6,%ymm6
vpsubw		%ymm1,%ymm8,%ymm8
vpsubw		%ymm1,%ymm10,%ymm10

#update
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpsubw		%ymm15,%ymm10,%ymm11
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8
vpaddw		%ymm15,%ymm10,%ymm10

#level 4
#zetas
vmovdqa		256(%rsi),%ymm12
vmovdqa		288(%rsi),%ymm13
vmovdqa		320(%rsi),%ymm14
vmovdqa		352(%rsi),%ymm15

#shuffle
vperm2i128	$0x02,%ymm4,%ymm5,%ymm3
vperm2i128	$0x13,%ymm4,%ymm5,%ymm4
vperm2i128	$0x02,%ymm6,%ymm7,%ymm5
vperm2i128	$0x13,%ymm6,%ymm7,%ymm6
vperm2i128	$0x02,%ymm8,%ymm9,%ymm7
vperm2i128	$0x13,%ymm8,%ymm9,%ymm8
vperm2i128	$0x02,%ymm10,%ymm11,%ymm9
vperm2i128	$0x13,%ymm10,%ymm11,%ymm10

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
vpsubw		%ymm11,%ymm4,%ymm11
vpsubw		%ymm12,%ymm6,%ymm12
vpsubw		%ymm13,%ymm8,%ymm13
vpsubw		%ymm14,%ymm10,%ymm14

#update
vpsubw		%ymm11,%ymm3,%ymm4
vpsubw		%ymm12,%ymm5,%ymm6
vpsubw		%ymm13,%ymm7,%ymm8
vpsubw		%ymm14,%ymm9,%ymm10
vpaddw		%ymm11,%ymm3,%ymm3
vpaddw		%ymm12,%ymm5,%ymm5
vpaddw		%ymm13,%ymm7,%ymm7
vpaddw		%ymm14,%ymm9,%ymm9

#level 5
#zetas 1
vmovdqa		512(%rsi),%ymm12
vmovdqa		544(%rsi),%ymm13
vmovdqa		576(%rsi),%ymm14
vmovdqa		608(%rsi),%ymm15

#shuffle
vshufpd		$0x00,%ymm4,%ymm3,%ymm11
vshufpd		$0x0F,%ymm4,%ymm3,%ymm3
vshufpd		$0x00,%ymm6,%ymm5,%ymm4
vshufpd		$0x0F,%ymm6,%ymm5,%ymm5
vshufpd		$0x00,%ymm8,%ymm7,%ymm6
vshufpd		$0x0F,%ymm8,%ymm7,%ymm7
vshufpd		$0x00,%ymm10,%ymm9,%ymm8
vshufpd		$0x0F,%ymm10,%ymm9,%ymm9
#vmovdqa	%ymm11,%ymm10

#mul
vpmullw		%ymm12,%ymm3,%ymm10
vpmulhw		%ymm12,%ymm3,%ymm3
vpmullw		%ymm13,%ymm5,%ymm12
vpmulhw		%ymm13,%ymm5,%ymm5
vpmullw		%ymm14,%ymm7,%ymm13
vpmulhw		%ymm14,%ymm7,%ymm7
vpmullw		%ymm15,%ymm9,%ymm14
vpmulhw		%ymm15,%ymm9,%ymm9

#reduce
vpmullw		%ymm0,%ymm10,%ymm10
vpmullw		%ymm0,%ymm12,%ymm12
vpmullw		%ymm0,%ymm13,%ymm13
vpmullw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm1,%ymm10,%ymm10
vpmulhw		%ymm1,%ymm12,%ymm12
vpmulhw		%ymm1,%ymm13,%ymm13
vpmulhw		%ymm1,%ymm14,%ymm14
vpsubw		%ymm10,%ymm3,%ymm10
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14

#update
vpsubw		%ymm10,%ymm11,%ymm3
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpaddw		%ymm10,%ymm11,%ymm10
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8

#level 6
#shuffle
vpshufd		$0xB1,%ymm10,%ymm12
vpshufd		$0xB1,%ymm3,%ymm13
vpshufd		$0xB1,%ymm4,%ymm14
vpshufd		$0xB1,%ymm5,%ymm15
vpblendd	$0x55,%ymm10,%ymm13,%ymm10
vpblendd	$0xAA,%ymm3,%ymm12,%ymm3
vpblendd	$0x55,%ymm4,%ymm15,%ymm4
vpblendd	$0xAA,%ymm5,%ymm14,%ymm5
vpshufd		$0xB1,%ymm6,%ymm12
vpshufd		$0xB1,%ymm7,%ymm13
vpshufd		$0xB1,%ymm8,%ymm14
vpshufd		$0xB1,%ymm9,%ymm15
vpblendd	$0x55,%ymm6,%ymm13,%ymm6
vpblendd	$0xAA,%ymm7,%ymm12,%ymm7
vpblendd	$0x55,%ymm8,%ymm15,%ymm8
vpblendd	$0xAA,%ymm9,%ymm14,%ymm9

#zetas
vmovdqa		768(%rsi),%ymm12
vmovdqa		800(%rsi),%ymm13
vmovdqa		832(%rsi),%ymm14
vmovdqa		864(%rsi),%ymm15

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
vpsubw		%ymm11,%ymm3,%ymm11
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14

#reduce 2
vmovdqa		_low_mask(%rip),%ymm15
vpaddw		%ymm2,%ymm10,%ymm10
vpaddw		%ymm2,%ymm4,%ymm4
vpaddw		%ymm2,%ymm6,%ymm6
vpaddw		%ymm2,%ymm8,%ymm8
vpsrlw		$13,%ymm10,%ymm3
vpsrlw		$13,%ymm4,%ymm5
vpsrlw		$13,%ymm6,%ymm7
vpsrlw		$13,%ymm8,%ymm9
#vpmullw	%ymm1,%ymm3,%ymm3
#vpmullw	%ymm1,%ymm5,%ymm5
#vpmullw	%ymm1,%ymm7,%ymm7
#vpmullw	%ymm1,%ymm9,%ymm9
vpand		%ymm15,%ymm10,%ymm10
vpand		%ymm15,%ymm4,%ymm4
vpand		%ymm15,%ymm6,%ymm6
vpand		%ymm15,%ymm8,%ymm8
vpsubw		%ymm3,%ymm10,%ymm10
vpsubw		%ymm5,%ymm4,%ymm4
vpsubw		%ymm7,%ymm6,%ymm6
vpsubw		%ymm9,%ymm8,%ymm8
vpsllw		$9,%ymm3,%ymm3
vpsllw		$9,%ymm5,%ymm5
vpsllw		$9,%ymm7,%ymm7
vpsllw		$9,%ymm9,%ymm9
vpaddw		%ymm3,%ymm10,%ymm10
vpaddw		%ymm5,%ymm4,%ymm4
vpaddw		%ymm7,%ymm6,%ymm6
vpaddw		%ymm9,%ymm8,%ymm8
vpsubw		%ymm1,%ymm10,%ymm10
vpsubw		%ymm1,%ymm4,%ymm4
vpsubw		%ymm1,%ymm6,%ymm6
vpsubw		%ymm1,%ymm8,%ymm8

#update
vpsubw		%ymm11,%ymm10,%ymm3
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpaddw		%ymm11,%ymm10,%ymm10
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8

#level 7
vmovdqa		_vpshufb_idx(%rip),%ymm15
vpshufb		%ymm15,%ymm10,%ymm11
vpshufb		%ymm15,%ymm3,%ymm12
vpshufb		%ymm15,%ymm4,%ymm13
vpshufb		%ymm15,%ymm5,%ymm14
vpblendw	$0x55,%ymm10,%ymm12,%ymm10
vpblendw	$0xAA,%ymm3,%ymm11,%ymm3
vpblendw	$0x55,%ymm4,%ymm14,%ymm4
vpblendw	$0xAA,%ymm5,%ymm13,%ymm5
vpshufb		%ymm15,%ymm6,%ymm11
vpshufb		%ymm15,%ymm7,%ymm12
vpshufb		%ymm15,%ymm8,%ymm13
vpshufb		%ymm15,%ymm9,%ymm14
vpblendw	$0x55,%ymm6,%ymm12,%ymm6
vpblendw	$0xAA,%ymm7,%ymm11,%ymm7
vpblendw	$0x55,%ymm8,%ymm14,%ymm8
vpblendw	$0xAA,%ymm9,%ymm13,%ymm9

#zetas
vmovdqa		1024(%rsi),%ymm12
vmovdqa		1056(%rsi),%ymm13
vmovdqa		1088(%rsi),%ymm14
vmovdqa		1120(%rsi),%ymm15

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
vpsubw		%ymm11,%ymm3,%ymm11
vpsubw		%ymm12,%ymm5,%ymm12
vpsubw		%ymm13,%ymm7,%ymm13
vpsubw		%ymm14,%ymm9,%ymm14

#update
vpsubw		%ymm11,%ymm10,%ymm3
vpsubw		%ymm12,%ymm4,%ymm5
vpsubw		%ymm13,%ymm6,%ymm7
vpsubw		%ymm14,%ymm8,%ymm9
vpaddw		%ymm11,%ymm10,%ymm10
vpaddw		%ymm12,%ymm4,%ymm4
vpaddw		%ymm13,%ymm6,%ymm6
vpaddw		%ymm14,%ymm8,%ymm8

#store
//vmovdqa		%ymm10,(%rdi)
//vmovdqa		%ymm3,32(%rdi)
//vmovdqa		%ymm4,64(%rdi)
//vmovdqa		%ymm5,96(%rdi)
//vmovdqa		%ymm6,128(%rdi)
//vmovdqa		%ymm7,160(%rdi)
//vmovdqa		%ymm8,192(%rdi)
//vmovdqa		%ymm9,224(%rdi)

vpunpcklwd %ymm3, %ymm10, %ymm13
vpunpckhwd %ymm3, %ymm10, %ymm14
vmovdqa		_16x2q(%rip),%ymm3
vperm2i128 $0x20,%ymm14,%ymm13,%ymm15
vperm2i128 $0x31,%ymm14,%ymm13,%ymm14
vpaddw    %ymm3,%ymm15,%ymm15
vpaddw    %ymm3,%ymm14,%ymm14
vmovdqu		%ymm15,(%rdi)
vmovdqu		%ymm14,32(%rdi)

vpunpcklwd %ymm5, %ymm4, %ymm13
vpunpckhwd %ymm5, %ymm4, %ymm14
vperm2i128 $0x20,%ymm14,%ymm13,%ymm15
vperm2i128 $0x31,%ymm14,%ymm13,%ymm14
vpaddw    %ymm3,%ymm15,%ymm15
vpaddw    %ymm3,%ymm14,%ymm14
vmovdqu		%ymm15,64(%rdi)
vmovdqu		%ymm14,96(%rdi)

vpunpcklwd %ymm7, %ymm6, %ymm13
vpunpckhwd %ymm7, %ymm6, %ymm14
vperm2i128 $0x20,%ymm14,%ymm13,%ymm15
vperm2i128 $0x31,%ymm14,%ymm13,%ymm14
vpaddw    %ymm3,%ymm15,%ymm15
vpaddw    %ymm3,%ymm14,%ymm14
vmovdqu		%ymm15,128(%rdi)
vmovdqu		%ymm14,160(%rdi)

vpunpcklwd %ymm9, %ymm8, %ymm13
vpunpckhwd %ymm9, %ymm8, %ymm14
vperm2i128 $0x20,%ymm14,%ymm13,%ymm15
vperm2i128 $0x31,%ymm14,%ymm13,%ymm14
vpaddw    %ymm3,%ymm15,%ymm15
vpaddw    %ymm3,%ymm14,%ymm14
vmovdqu		%ymm15,192(%rdi)
vmovdqu		%ymm14,224(%rdi)


add 		%r11,%rsp
ret
