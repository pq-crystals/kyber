#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../randombytes.h"
#include "../kem.h"

#define CLIENT_PK_PATH "client_pk.bin"
#define CLIENT_SK_PATH "client_sk.bin"
#define SERVER_PK_PATH "server_pk.bin"
#define SERVER_SK_PATH "server_sk.bin"

/* Get current time in milliseconds */
static uint64_t get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/* Write binary file */
static int write_file(const char *path, const uint8_t *buf, size_t len) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        fprintf(stderr, "[!] Failed to open %s for writing\n", path);
        return -1;
    }
    
    if (fwrite(buf, 1, len, f) != len) {
        fprintf(stderr, "[!] Failed to write %s\n", path);
        fclose(f);
        return -1;
    }
    
    fclose(f);
    return 0;
}

int main(void) {
    uint8_t client_pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t client_sk[CRYPTO_SECRETKEYBYTES];
    uint8_t server_pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t server_sk[CRYPTO_SECRETKEYBYTES];

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              KYBER KEY GENERATION UTILITY                  ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\nGenerating keypairs for client and server...\n");
    printf("Mode: KYBER_K=%d\n", CRYPTO_PUBLICKEYBYTES);
    printf("PK: %d bytes | SK: %d bytes\n\n", CRYPTO_PUBLICKEYBYTES, CRYPTO_SECRETKEYBYTES);

    /* Generate client keypair */
    printf("[1/2] Generating client keypair...\n");
    uint64_t t1 = get_time_ms();
    if (crypto_kem_keypair(client_pk, client_sk) != 0) {
        fprintf(stderr, "[!] Client keypair generation failed\n");
        return 1;
    }
    uint64_t t2 = get_time_ms();
    printf("      Time: %llu ms\n", (unsigned long long)(t2 - t1));

    /* Generate server keypair */
    printf("[2/2] Generating server keypair...\n");
    t1 = get_time_ms();
    if (crypto_kem_keypair(server_pk, server_sk) != 0) {
        fprintf(stderr, "[!] Server keypair generation failed\n");
        return 1;
    }
    t2 = get_time_ms();
    printf("      Time: %llu ms\n", (unsigned long long)(t2 - t1));

    /* Save keys to files */
    printf("\nSaving keys to files...\n");
    
    if (write_file(CLIENT_PK_PATH, client_pk, sizeof(client_pk)) < 0) {
        return 1;
    }
    printf("[✓] Saved %s (%zu bytes)\n", CLIENT_PK_PATH, sizeof(client_pk));

    if (write_file(CLIENT_SK_PATH, client_sk, sizeof(client_sk)) < 0) {
        return 1;
    }
    printf("[✓] Saved %s (%zu bytes)\n", CLIENT_SK_PATH, sizeof(client_sk));

    if (write_file(SERVER_PK_PATH, server_pk, sizeof(server_pk)) < 0) {
        return 1;
    }
    printf("[✓] Saved %s (%zu bytes)\n", SERVER_PK_PATH, sizeof(server_pk));

    if (write_file(SERVER_SK_PATH, server_sk, sizeof(server_sk)) < 0) {
        return 1;
    }
    printf("[✓] Saved %s (%zu bytes)\n", SERVER_SK_PATH, sizeof(server_sk));

    printf("\n[✓] All keypairs generated successfully\n");
    printf("    Copy %s to the server machine\n", CLIENT_PK_PATH);
    printf("    Copy %s to the server machine\n\n", SERVER_SK_PATH);

    return 0;
}
