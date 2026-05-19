#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/resource.h>

#include "../randombytes.h"
#include "../kem.h"

#define SERVER_PORT 5000
#define BUFFER_SIZE 8192
#define DEFAULT_TARGET_IP "127.0.0.1"
#define CLIENT_PK_PATH "client_pk.bin"
#define CLIENT_SK_PATH "client_sk.bin"
#define SERVER_PK_PATH "server_pk.bin"
#define CLIENT_LOG_PATH "client_kyber.log"

/* Get current time in microseconds */
static uint64_t get_time_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

/* Load binary file */
static int load_file_exact(const char *path, uint8_t *buf, size_t len) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "[!] Failed to open %s\n", path);
        return -1;
    }

    size_t n = fread(buf, 1, len, f);
    fclose(f);

    if (n != len) {
        fprintf(stderr, "[!] File %s: expected %zu bytes, got %zu\n", path, len, n);
        return -1;
    }

    return 0;
}

/* Send all data over socket */
static int send_all(int sock, const uint8_t *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t sent = send(sock, (const char *)(buf + total), len - total, 0);
        if (sent < 0) {
            perror("[!] send");
            return -1;
        }
        if (sent == 0) {
            fprintf(stderr, "[!] send: connection closed\n");
            return -1;
        }
        total += sent;
    }
    return 0;
}

/* Receive all data from socket */
static int recv_all(int sock, uint8_t *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t recvd = recv(sock, (char *)(buf + total), len - total, 0);
        if (recvd < 0) {
            perror("[!] recv");
            return -1;
        }
        if (recvd == 0) {
            fprintf(stderr, "[!] recv: connection closed\n");
            return -1;
        }
        total += recvd;
    }
    return 0;
}

/* Send blob with 4-byte length prefix */
static int send_blob(int sock, const uint8_t *data, uint32_t data_len) {
    uint32_t len_net = htonl(data_len);
    if (send_all(sock, (const uint8_t *)&len_net, sizeof(len_net)) < 0) {
        return -1;
    }
    if (data_len == 0) {
        return 0;
    }
    return send_all(sock, data, data_len);
}

/* Receive blob with 4-byte length prefix */
static int recv_blob(int sock, uint8_t *buffer, uint32_t buffer_size, uint32_t *out_len) {
    uint32_t len_net = 0;
    if (recv_all(sock, (uint8_t *)&len_net, sizeof(len_net)) < 0) {
        return -1;
    }

    uint32_t payload_len = ntohl(len_net);
    if (payload_len > buffer_size) {
        fprintf(stderr, "[!] recv_blob: payload too large: %u > %u\n", payload_len, buffer_size);
        return -1;
    }

    if (payload_len > 0 && recv_all(sock, buffer, payload_len) < 0) {
        return -1;
    }

    *out_len = payload_len;
    return 0;
}

/* Log result */
static void log_result(const char *log_path, int status, size_t ss_len, uint64_t elapsed_us) {
    struct rusage ru;
    double user_ms = 0.0;
    double sys_ms = 0.0;
    long rss_kb = 0;

    if (getrusage(RUSAGE_SELF, &ru) == 0) {
        user_ms = ru.ru_utime.tv_sec * 1000.0 + ru.ru_utime.tv_usec / 1000.0;
        sys_ms = ru.ru_stime.tv_sec * 1000.0 + ru.ru_stime.tv_usec / 1000.0;
        rss_kb = ru.ru_maxrss;
    }

    FILE *f = fopen(log_path, "a");
    if (!f) {
        fprintf(stderr, "[!] Failed to open %s for logging\n", log_path);
        return;
    }

    fprintf(f, "status=%d,ss_len=%zu,elapsed_us=%llu,user_ms=%.2f,sys_ms=%.2f,rss_kb=%ld\n",
            status, ss_len, (unsigned long long)elapsed_us, user_ms, sys_ms, rss_kb);
    fclose(f);
}

int main(int argc, char *argv[]) {
    int sock = -1;
    struct sockaddr_in server_addr;
    uint8_t client_pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t client_sk[CRYPTO_SECRETKEYBYTES];
    uint8_t server_pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t ciphertext[CRYPTO_CIPHERTEXTBYTES];
    uint32_t ct_len = 0;
    uint8_t shared_secret_client[CRYPTO_BYTES];
    uint32_t ss_len = 0;

    const char *ip = (argc > 1) ? argv[1] : DEFAULT_TARGET_IP;
    const char *log_path = getenv("CLIENT_LOG_PATH");
    if (!log_path || *log_path == '\0') {
        log_path = CLIENT_LOG_PATH;
    }

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              KYBER CLIENT (Decapsulation)                  ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    printf("\n[LOADING] Reading keypairs from files...\n");
    if (load_file_exact(CLIENT_PK_PATH, client_pk, sizeof(client_pk)) < 0) {
        fprintf(stderr, "[!] Failed to load client PK\n");
        return 1;
    }
    printf("[✓] Loaded %s (%zu bytes)\n", CLIENT_PK_PATH, sizeof(client_pk));

    if (load_file_exact(CLIENT_SK_PATH, client_sk, sizeof(client_sk)) < 0) {
        fprintf(stderr, "[!] Failed to load client SK\n");
        return 1;
    }
    printf("[✓] Loaded %s (%zu bytes)\n", CLIENT_SK_PATH, sizeof(client_sk));

    if (load_file_exact(SERVER_PK_PATH, server_pk, sizeof(server_pk)) < 0) {
        fprintf(stderr, "[!] Failed to load server PK\n");
        return 1;
    }
    printf("[✓] Loaded %s (%zu bytes)\n", SERVER_PK_PATH, sizeof(server_pk));

    printf("\n[NETWORK] Connecting to server at %s:%d\n", ip, SERVER_PORT);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[!] socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) != 1) {
        fprintf(stderr, "[!] inet_pton failed\n");
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[!] connect");
        close(sock);
        return 1;
    }
    printf("[✓] Connected to server\n");

    uint64_t start_us = get_time_us();

    printf("\n[STAGE 1] Sending client public key to server...\n");
    if (send_blob(sock, client_pk, CRYPTO_PUBLICKEYBYTES) < 0) {
        fprintf(stderr, "[!] Failed to send client PK\n");
        close(sock);
        log_result(log_path, -1, 0, get_time_us() - start_us);
        return 1;
    }
    printf("[✓] Sent client PK (%u bytes)\n", CRYPTO_PUBLICKEYBYTES);

    printf("\n[STAGE 2] Receiving ciphertext from server...\n");
    if (recv_blob(sock, ciphertext, sizeof(ciphertext), &ct_len) < 0) {
        fprintf(stderr, "[!] Failed to receive ciphertext\n");
        close(sock);
        log_result(log_path, -2, 0, get_time_us() - start_us);
        return 1;
    }

    if (ct_len != CRYPTO_CIPHERTEXTBYTES) {
        fprintf(stderr, "[!] Ciphertext size mismatch: expected %u, got %u\n",
                CRYPTO_CIPHERTEXTBYTES, ct_len);
        close(sock);
        log_result(log_path, -3, 0, get_time_us() - start_us);
        return 1;
    }
    printf("[✓] Received ciphertext (%u bytes)\n", ct_len);

    printf("\n[STAGE 3] Decapsulating ciphertext with client secret key...\n");
    uint64_t t1 = get_time_us();
    if (crypto_kem_dec(shared_secret_client, ciphertext, client_sk) != 0) {
        fprintf(stderr, "[!] Decapsulation failed\n");
        close(sock);
        log_result(log_path, -4, 0, get_time_us() - start_us);
        return 1;
    }
    uint64_t t2 = get_time_us();
    ss_len = CRYPTO_BYTES;
    printf("[✓] Decapsulation successful (Time: %llu us = %.3f ms)\n", 
           (unsigned long long)(t2 - t1), (t2 - t1) / 1000.0);
    printf("    Shared Secret: %u bytes\n", ss_len);

    printf("\n[STAGE 4] Sending shared secret to server for verification...\n");
    if (send_blob(sock, shared_secret_client, ss_len) < 0) {
        fprintf(stderr, "[!] Failed to send shared secret\n");
        close(sock);
        log_result(log_path, -5, ss_len, get_time_us() - start_us);
        return 1;
    }
    printf("[✓] Sent shared secret (%u bytes)\n", ss_len);

    printf("\n[STAGE 5] Receiving verification result from server...\n");
    uint32_t verify_result_len = 0;
    uint8_t verify_result[4];
    if (recv_blob(sock, verify_result, sizeof(verify_result), &verify_result_len) < 0) {
        fprintf(stderr, "[!] Failed to receive verification result\n");
        close(sock);
        log_result(log_path, -6, ss_len, get_time_us() - start_us);
        return 1;
    }

    uint64_t end_us = get_time_us();
    uint64_t elapsed_us = end_us - start_us;

    if (verify_result_len >= 1) {
        int result = verify_result[0];
        if (result == 0) {
            printf("[✓] Server verification: PASSED (Shared secrets MATCH)\n");
            printf("\n");
            printf("╔════════════════════════════════════════════════════════════╗\n");
            printf("║                  KYBER TEST SUCCESSFUL                     ║\n");
            printf("╚════════════════════════════════════════════════════════════╝\n");
            printf("Ciphertext:             %u bytes\n", ct_len);
            printf("Shared Secret:          %u bytes\n", ss_len);
            printf("Total Time:             %.3f ms (%llu us)\n", elapsed_us / 1000.0, (unsigned long long)elapsed_us);
            printf("════════════════════════════════════════════════════════════\n\n");
            close(sock);
            log_result(log_path, 0, ss_len, elapsed_us);
            return 0;
        } else {
            printf("[✗] Server verification: FAILED (Shared secrets DO NOT match)\n");
            close(sock);
            log_result(log_path, -7, ss_len, elapsed_us);
            return 1;
        }
    }

    printf("[!] Invalid verification result from server\n");
    close(sock);
    log_result(log_path, -8, ss_len, elapsed_us);
    return 1;
}
