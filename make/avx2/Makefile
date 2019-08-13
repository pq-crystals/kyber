# This Makefile can be used with GNU Make or BSD Make

LIB=libSCHEME_NAME_clean.a
HEADERS=params.h poly.h polyvec.h reduce.h fq.inc cbd.h ntt.h shuffle.inc verify.h indcpa.h rejsample.h symmetric.h randombytes.h fips202.h fips202x4.h
OBJECTS=kem.o poly.o polyvec.o fq.o shuffle.o cbd.o ntt.o invntt.o basemul.o consts.o \
		verify.o indcpa.o rejsample.o fips202x4.o symmetric-fips202.o

CFLAGS=-mavx2 -mbmi2 -mpopcnt -O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
