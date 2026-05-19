#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/resource.h>

#include "../randombytes.h"
#include "../kem.h"

#define SERVER_PORT 5000
#define BUFFER_SIZE 8192
#define DEFAULT_TARGET_IP "192.168.4.85"
#define DEFAULT_CONCURRENT 10
#define DEFAULT_BATCHES 0
#define DEFAULT_BATCH_DELAY_SEC 0
#define CLIENT_SK_PATH "client_sk.bin"
#define CLIENT_PK_PATH "client_pk.bin"
#define CLIENT_LOG_PATH "client_stress.log"

static uint64_t get_time_ms(void) {
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static unsigned int parse_uint_env(const char *name, unsigned int def_value) {
    const char *val = getenv(name);
    if (!val || *val == '\0') {
        return def_value;
    }

    char *end = NULL;
    unsigned long parsed = strtoul(val, &end, 10);
    if (!end || *end != '\0' || parsed > UINT_MAX) {
        return def_value;
    }

    return (unsigned int)parsed;
}

static const char *get_env_or_default(const char *name, const char *def_value) {
    const char *val = getenv(name);
    if (!val || *val == '\0') {
        return def_value;
    }
    return val;
}

static int load_file_exact(const char *path, uint8_t *buf, size_t len) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        return -1;
    }

    size_t n = fread(buf, 1, len, f);
    fclose(f);

    if (n != len) {
        return -1;
    }

    return 0;
}

static int send_all(int sock, const uint8_t *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t sent = send(sock, (const char *)buf + total, (int)(len - total), 0);
        if (sent < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (sent == 0) {
            errno = ECONNRESET;
            return -1;
        }
        total += (size_t)sent;
    }
    return 0;
}

static int recv_all(int sock, uint8_t *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t recvd = recv(sock, (char *)buf + total, (int)(len - total), 0);
        if (recvd == 0) {
            errno = ECONNRESET;
            return -1;
        }
        if (recvd < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        total += (size_t)recvd;
    }
    return 0;
}

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

static int recv_blob(int sock, uint8_t *buffer, uint32_t buffer_size, uint32_t *out_len) {
    uint32_t len_net = 0;
    if (recv_all(sock, (uint8_t *)&len_net, sizeof(len_net)) < 0) {
        return -1;
    }

    uint32_t payload_len = ntohl(len_net);
    if (payload_len > buffer_size) {
        errno = EMSGSIZE;
        return -1;
    }

    if (payload_len > 0 && recv_all(sock, buffer, payload_len) < 0) {
        return -1;
    }

    *out_len = payload_len;
    return 0;
}

static void log_result(const char *log_path,
                       int status,
                       uint32_t ct_len,
                       size_t ss_len,
                       uint64_t elapsed_ms) {
    struct rusage ru;
    double user_ms = 0.0;
    double sys_ms = 0.0;
    long rss_kb = 0;

    if (getrusage(RUSAGE_SELF, &ru) == 0) {
        user_ms = (double)ru.ru_utime.tv_sec * 1000.0 + (double)ru.ru_utime.tv_usec / 1000.0;
        sys_ms = (double)ru.ru_stime.tv_sec * 1000.0 + (double)ru.ru_stime.tv_usec / 1000.0;
        rss_kb = ru.ru_maxrss;
    }

    FILE *f = fopen(log_path, "a");
    if (!f) {
        return;
    }

    fprintf(f,
            "pid=%ld status=%s elapsed_ms=%llu cpu_user_ms=%.3f cpu_sys_ms=%.3f rss_kb=%ld ct=%u ss=%zu\n",
            (long)getpid(),
            status == 0 ? "OK" : "FAIL",
            (unsigned long long)elapsed_ms,
            user_ms,
            sys_ms,
            rss_kb,
            ct_len,
            ss_len);
    fclose(f);
}

static int run_client_once(const char *ip, const uint8_t *sk, uint32_t *out_ct_len, size_t *out_ss_len, uint64_t *out_elapsed_ms) {
    int sock;
    struct sockaddr_in server_addr;
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
    uint8_t ss_client[CRYPTO_BYTES]; // Shared secret mà client tính được
    uint8_t verify_result[1];
    uint32_t ct_len = 0;
    uint32_t vr_len = 0;
    uint32_t temp_len = 0;

    // 1. Tải PK để gửi (Stage 1)
    if (load_file_exact(CLIENT_PK_PATH, pk, sizeof(pk)) < 0) return -1;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -2;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    uint64_t start_ms = get_time_ms();

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(sock);
        return -3;
    }

    // --- GIAO THỨC KYBER 4 BƯỚC ---

    // BƯỚC 1: Gửi PK cho Server
    if (send_blob(sock, pk, sizeof(pk)) < 0) { close(sock); return -4; }

    // BƯỚC 2: Nhận Ciphertext từ Server
    if (recv_blob(sock, ct, sizeof(ct), &ct_len) < 0) { close(sock); return -5; }

    // BƯỚC 3: Giải mã để lấy Shared Secret
    if (crypto_kem_dec(ss_client, ct, sk) != 0) { close(sock); return -6; }

    // BƯỚC QUAN TRỌNG NHẤT (BỊ THIẾU): Gửi Shared Secret về Server để xác minh
    // Server đang treo ở [STAGE 4] để đợi cái này
    if (send_blob(sock, ss_client, sizeof(ss_client)) < 0) {
        close(sock);
        return -7;
    }

    // BƯỚC 4: Nhận kết quả xác minh cuối cùng (0 = OK, 1 = FAIL)
    if (recv_blob(sock, verify_result, sizeof(verify_result), &vr_len) < 0) {
        close(sock);
        return -8;
    }

    uint64_t end_ms = get_time_ms();
    close(sock);

    *out_ct_len = ct_len;
    *out_ss_len = CRYPTO_BYTES;
    *out_elapsed_ms = (end_ms - start_ms);

    return (vr_len > 0 && verify_result[0] == 0) ? 0 : -9;
}

int main(void) {
    const char *ip = get_env_or_default("TARGET_IP", DEFAULT_TARGET_IP);
    unsigned int concurrent = parse_uint_env("CONCURRENT_SESSIONS", DEFAULT_CONCURRENT);
    unsigned int batches = parse_uint_env("BATCHES", DEFAULT_BATCHES);
    unsigned int batch_delay = parse_uint_env("BATCH_DELAY_SEC", DEFAULT_BATCH_DELAY_SEC);
    const char *log_path = get_env_or_default("CLIENT_LOG_PATH", CLIENT_LOG_PATH);

    if (concurrent == 0) {
        concurrent = 1;
    }

    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    if (load_file_exact(CLIENT_SK_PATH, sk, sizeof(sk)) < 0) {
        fprintf(stderr, "Missing %s. Run test_kyber_keygen first.\n", CLIENT_SK_PATH);
        return 1;
    }

    printf("[STRESS] target=%s concurrent=%u batches=%u delay=%u\n", ip, concurrent, batches, batch_delay);

    unsigned int batch = 1;
    while (batches == 0 || batch <= batches) {
        unsigned int spawned = 0;

        for (spawned = 0; spawned < concurrent; ++spawned) {
            pid_t pid = fork();
            if (pid == 0) {
                uint32_t ct_len = 0;
                size_t ss_len = 0;
                uint64_t elapsed_ms = 0;
                int status = run_client_once(ip, sk, &ct_len, &ss_len, &elapsed_ms);
                log_result(log_path, status, ct_len, ss_len, elapsed_ms);
                _exit(status);
            }

            if (pid < 0) {
                perror("fork failed");
                break;
            }
        }

        while (spawned > 0) {
            int wstatus = 0;
            if (wait(&wstatus) > 0) {
                --spawned;
            }
        }

        printf("[STRESS] Batch %u done\n", batch);
        if (batches != 0) {
            ++batch;
        }

        if (batch_delay > 0) {
            sleep(batch_delay);
        }
    }

    return 0;
}
