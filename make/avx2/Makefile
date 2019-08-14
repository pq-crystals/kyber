# This Makefile can be used with GNU Make or BSD Make

LIB=libSCHEME_NAME_avx2.a
HEADERS=params.h poly.h polyvec.h reduce.h fq.inc cbd.h ntt.h shuffle.inc verify.h indcpa.h rejsample.h symmetric.h randombytes.h fips202.h fips202x4.h
OBJECTS=kem.o poly.o polyvec.o fq.o shuffle.o cbd.o ntt.o invntt.o basemul.o consts.o \
		verify.o indcpa.o rejsample.o fips202x4.o symmetric-fips202.o

KECCAK4XDIR=../../../common/keccak4x
KECCAK4XOBJ=KeccakP-1600-times4-SIMD256.o
KECCAK4X=$(KECCAK4XDIR)/$(KECCAK4XOBJ)

CFLAGS=-mavx2 -mbmi2 -mpopcnt -O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) $(KECCACK4X)
	$(AR) -r $@ $(OBJECTS) $(KECCAK4X)

$(KECCAK4X):
	$(MAKE) -C $(KECCAK4XDIR) $(KECCAK4XOBJ)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
