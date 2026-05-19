CC ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wvla -Wpointer-arith -O3 -fomit-frame-pointer

ROOT := ..

SOURCES = $(ROOT)/kem.c $(ROOT)/indcpa.c $(ROOT)/polyvec.c $(ROOT)/poly.c \
  $(ROOT)/ntt.c $(ROOT)/cbd.c $(ROOT)/reduce.c $(ROOT)/verify.c \
  $(ROOT)/symmetric-shake.c
HEADERS = $(ROOT)/api.h $(ROOT)/kem.h $(ROOT)/indcpa.h $(ROOT)/polyvec.h \
  $(ROOT)/poly.h $(ROOT)/ntt.h $(ROOT)/cbd.h $(ROOT)/reduce.h \
  $(ROOT)/verify.h $(ROOT)/symmetric.h $(ROOT)/randombytes.h
KECCAK_SOURCES = $(SOURCES) $(ROOT)/fips202.c
KECCAK_HEADERS = $(HEADERS) $(ROOT)/fips202.h

.PHONY: all keygen run-server run-client clean

MODE ?= 2
TARGET_IP ?= 127.0.0.1

CLIENT_BIN := test_kyber_client$(MODE)
SERVER_BIN := test_kyber_server$(MODE)
KEYGEN_BIN := test_kyber_keygen$(MODE)

all: \
  test_kyber_keygen2 \
  test_kyber_keygen3 \
  test_kyber_keygen4 \
  test_kyber_client2 \
  test_kyber_client3 \
  test_kyber_client4 \
  test_kyber_server2 \
  test_kyber_server3 \
	test_kyber_server4 \
	test_kyber_stress2 \
	test_kyber_stress3 \
	test_kyber_stress4

test_kyber_keygen2: test_kyber_keygen.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=2 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_keygen3: test_kyber_keygen.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=3 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_keygen4: test_kyber_keygen.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=4 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_client2: test_kyber_client.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=2 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_client3: test_kyber_client.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=3 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_client4: test_kyber_client.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=4 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_server2: test_kyber_server.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=2 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_server3: test_kyber_server.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=3 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_server4: test_kyber_server.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=4 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_stress2: test_kyber_stress.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=2 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_stress3: test_kyber_stress.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=3 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

test_kyber_stress4: test_kyber_stress.c $(ROOT)/randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DKYBER_K=4 \
	  -o $@ $< $(ROOT)/randombytes.c $(KECCAK_SOURCES)

keygen: $(KEYGEN_BIN)
	@echo "[KEYGEN] MODE=$(MODE)"
	@./$(KEYGEN_BIN)

run-server: $(SERVER_BIN)
	@echo "[RUN] Server MODE=$(MODE) on port 5000"
	@./$(SERVER_BIN)

run-client: $(CLIENT_BIN)
	@echo "[RUN] Client MODE=$(MODE) -> $(TARGET_IP)"
	@./$(CLIENT_BIN) $(TARGET_IP)

clean:
	rm -f test_kyber_keygen2 test_kyber_keygen3 test_kyber_keygen4
	rm -f test_kyber_client2 test_kyber_client3 test_kyber_client4
	rm -f test_kyber_server2 test_kyber_server3 test_kyber_server4
	rm -f test_kyber_stress2 test_kyber_stress3 test_kyber_stress4
	rm -f *.bin *.log
