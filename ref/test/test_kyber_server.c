#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

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
#define CLIENT_PK_PATH "client_pk.bin"
#define SERVER_SK_PATH "server_sk.bin"
#define SERVER_LOG_PATH "server_kyber.log"

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

/* Handle single client connection */
static int handle_client(int client_sock, const struct sockaddr_in *client_addr,
                        const uint8_t *client_pk, const uint8_t *server_sk) {
    uint8_t received_pk[CRYPTO_PUBLICKEYBYTES];
    uint32_t pk_len = 0;
    uint8_t ciphertext[CRYPTO_CIPHERTEXTBYTES];
    uint8_t shared_secret_server[CRYPTO_BYTES];
    uint8_t shared_secret_client[BUFFER_SIZE];
    uint32_t ss_client_len = 0;
    uint8_t verify_result[1];

    uint64_t start_us = get_time_us();

    printf("[*] Client connected from %s:%d\n", inet_ntoa(client_addr->sin_addr),
           ntohs(client_addr->sin_port));

    printf("\n[STAGE 1] Receiving client's public key...\n");
    if (recv_blob(client_sock, received_pk, sizeof(received_pk), &pk_len) < 0) {
        fprintf(stderr, "[!] Failed to receive client PK\n");
        log_result(SERVER_LOG_PATH, -1, 0, get_time_us() - start_us);
        return -1;
    }

    if (pk_len != CRYPTO_PUBLICKEYBYTES) {
        fprintf(stderr, "[!] Client PK size mismatch: expected %u, got %u\n",
                CRYPTO_PUBLICKEYBYTES, pk_len);
        log_result(SERVER_LOG_PATH, -2, 0, get_time_us() - start_us);
        return -1;
    }
    printf("[✓] Received client PK (%u bytes)\n", pk_len);

    printf("\n[STAGE 2] Encapsulating using client's public key...\n");
    uint64_t t1 = get_time_us();
    if (crypto_kem_enc(ciphertext, shared_secret_server, received_pk) != 0) {
        fprintf(stderr, "[!] Encapsulation failed\n");
        log_result(SERVER_LOG_PATH, -3, 0, get_time_us() - start_us);
        return -1;
    }
    uint64_t t2 = get_time_us();
    printf("[✓] Encapsulation successful (Time: %llu us = %.3f ms)\n",
           (unsigned long long)(t2 - t1), (t2 - t1) / 1000.0);
    printf("    Ciphertext:      %u bytes\n", CRYPTO_CIPHERTEXTBYTES);
    printf("    Shared Secret:   %u bytes\n", CRYPTO_BYTES);

    printf("\n[STAGE 3] Sending ciphertext to client...\n");
    if (send_blob(client_sock, ciphertext, CRYPTO_CIPHERTEXTBYTES) < 0) {
        fprintf(stderr, "[!] Failed to send ciphertext\n");
        log_result(SERVER_LOG_PATH, -4, CRYPTO_BYTES, get_time_us() - start_us);
        return -1;
    }
    printf("[✓] Sent ciphertext (%u bytes)\n", CRYPTO_CIPHERTEXTBYTES);

    printf("\n[STAGE 4] Receiving client's shared secret...\n");
    if (recv_blob(client_sock, shared_secret_client, sizeof(shared_secret_client),
                  &ss_client_len) < 0) {
        fprintf(stderr, "[!] Failed to receive client shared secret\n");
        log_result(SERVER_LOG_PATH, -5, CRYPTO_BYTES, get_time_us() - start_us);
        return -1;
    }

    if (ss_client_len != CRYPTO_BYTES) {
        fprintf(stderr, "[!] Client SS size mismatch: expected %u, got %u\n",
                CRYPTO_BYTES, ss_client_len);
        log_result(SERVER_LOG_PATH, -6, CRYPTO_BYTES, get_time_us() - start_us);
        return -1;
    }
    printf("[✓] Received client shared secret (%u bytes)\n", ss_client_len);

    printf("\n[STAGE 5] Verifying shared secrets...\n");
    int match = memcmp(shared_secret_server, shared_secret_client, CRYPTO_BYTES);

    if (match == 0) {
        printf("[✓] Verification PASSED: Shared secrets MATCH!\n");
        verify_result[0] = 0;
    } else {
        printf("[✗] Verification FAILED: Shared secrets DO NOT match\n");
        verify_result[0] = 1;
    }

    printf("\n[STAGE 6] Sending verification result to client...\n");
    if (send_blob(client_sock, verify_result, 1) < 0) {
        fprintf(stderr, "[!] Failed to send verification result\n");
        log_result(SERVER_LOG_PATH, -7, CRYPTO_BYTES, get_time_us() - start_us);
        return -1;
    }
    printf("[✓] Sent verification result\n");

    uint64_t end_us = get_time_us();
    uint64_t elapsed_us = end_us - start_us;

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              KYBER ENCAPSULATION TEST RESULT               ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("Client Public Key:      %u bytes\n", CRYPTO_PUBLICKEYBYTES);
    printf("Ciphertext:             %u bytes\n", CRYPTO_CIPHERTEXTBYTES);
    printf("Shared Secret:          %u bytes\n", CRYPTO_BYTES);
    printf("Verification Result:    %s\n", match == 0 ? "✓ PASS" : "✗ FAIL");
    printf("Total Time:             %.3f ms (%llu us)\n", elapsed_us / 1000.0, (unsigned long long)elapsed_us);
    printf("════════════════════════════════════════════════════════════\n\n");

    log_result(SERVER_LOG_PATH, match == 0 ? 0 : 1, CRYPTO_BYTES, elapsed_us);
    return match == 0 ? 0 : 1;
}

int main(void) {
    int server_sock = -1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    int client_sock;
    uint8_t client_pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t server_sk[CRYPTO_SECRETKEYBYTES];

    signal(SIGPIPE, SIG_IGN);

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║             KYBER SERVER (Encapsulation)                   ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\nListening on port %d\n", SERVER_PORT);
    printf("Mode: KYBER_K=%d\n", CRYPTO_PUBLICKEYBYTES);
    printf("PK: %d bytes | SK: %d bytes | CT: %d bytes | SS: %d bytes\n\n",
           CRYPTO_PUBLICKEYBYTES, CRYPTO_SECRETKEYBYTES,
           CRYPTO_CIPHERTEXTBYTES, CRYPTO_BYTES);

    printf("[LOADING] Reading keypairs from files...\n");
    if (load_file_exact(CLIENT_PK_PATH, client_pk, sizeof(client_pk)) < 0) {
        fprintf(stderr, "[!] Failed to load client PK\n");
        return 1;
    }
    printf("[✓] Loaded %s (%zu bytes)\n", CLIENT_PK_PATH, sizeof(client_pk));

    if (load_file_exact(SERVER_SK_PATH, server_sk, sizeof(server_sk)) < 0) {
        fprintf(stderr, "[!] Failed to load server SK\n");
        return 1;
    }
    printf("[✓] Loaded %s (%zu bytes)\n", SERVER_SK_PATH, sizeof(server_sk));

    printf("\n[NETWORK] Creating listening socket...\n");
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[!] socket");
        return 1;
    }

    int reuse = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("[!] setsockopt");
        close(server_sock);
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[!] bind");
        close(server_sock);
        return 1;
    }

    if (listen(server_sock, 128) < 0) {
        perror("[!] listen");
        close(server_sock);
        return 1;
    }

    printf("[✓] Server listening on 0.0.0.0:%d\n", SERVER_PORT);
    printf("[*] Waiting for client connections...\n\n");

    while (1) {
        client_addr_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("[!] accept");
            continue;
        }

        handle_client(client_sock, &client_addr, client_pk, server_sk);
        close(client_sock);

        printf("[*] Waiting for next client connection...\n\n");
    }

    close(server_sock);
    return 0;
}
