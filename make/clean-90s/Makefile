# This Makefile can be used with GNU Make or BSD Make

LIB=libSCHEME_NAME_clean.a
HEADERS=api.h cbd.h indcpa.h ntt.h params.h poly.h polyvec.h reduce.h verify.h symmetric.h aes256ctr.h
OBJECTS=cbd.o indcpa.o kem.o ntt.o poly.o polyvec.o reduce.o verify.o aes256ctr.o

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
