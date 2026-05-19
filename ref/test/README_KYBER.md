# Kyber Client-Server TCP Implementation

## Overview

This is a TCP socket-based implementation of the Kyber key encapsulation mechanism (KEM) with client-server architecture. The implementation demonstrates the complete KEM protocol:

1. **Key Generation** - Generate keypairs for both client and server
2. **Send Client PK** - Client sends its public key to server
3. **Encapsulation** - Server encapsulates using client's public key, generates ciphertext and shared secret
4. **Decapsulation** - Client decapsulates using its secret key to recover shared secret
5. **Verification** - Server verifies both sides derived the same shared secret

## Architecture

### Three Main Programs

#### 1. `test_kyber_keygen` - Keypair Generation
Generates and saves keypairs to binary files:
- `client_sk.bin` - Client secret key (1632 bytes for Kyber512, 2400 for Kyber768, 3168 for Kyber1024)
- `client_pk.bin` - Client public key (800 bytes for Kyber512, 1184 for Kyber768, 1568 for Kyber1024)
- `server_sk.bin` - Server secret key (same sizes as client)
- `server_pk.bin` - Server public key (same sizes as client)

**Run**: `./test_kyber_keygen2` (for Kyber512)

#### 2. `test_kyber_server` - TCP Server
- Listens on port 5000
- Accepts client connections
- Performs encapsulation using client's public key
- Sends ciphertext to client
- Receives and verifies client's shared secret
- Logs results to `server_kyber.log`

**Run**: `./test_kyber_server2`

#### 3. `test_kyber_client` - TCP Client
- Loads pre-generated keys from files
- Connects to server (localhost:5000 or custom IP)
- Sends client public key to server
- Receives ciphertext from server
- Decapsulates to get shared secret
- Sends shared secret to server for verification
- Logs results to `client_kyber.log`

**Run**: `./test_kyber_client2 [server_ip] [port]`

## Kyber Modes

Three variants are supported:

| Mode | Name | PK Size | SK Size | CT Size | SS Size | Security |
|------|------|---------|---------|---------|---------|----------|
| 2 | Kyber512 | 800 B | 1632 B | 768 B | 32 B | AES128 |
| 3 | Kyber768 | 1184 B | 2400 B | 1088 B | 32 B | AES192 |
| 4 | Kyber1024 | 1568 B | 3168 B | 1568 B | 32 B | AES256 |

## Compilation

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install build-essential libssl-dev

# Verify gcc
gcc --version
```

### Compile All Variants
```bash
cd kyber-dev/ref/test/
make all
```

This creates 9 executables:
- `test_kyber_keygen2`, `test_kyber_keygen3`, `test_kyber_keygen4`
- `test_kyber_client2`, `test_kyber_client3`, `test_kyber_client4`
- `test_kyber_server2`, `test_kyber_server3`, `test_kyber_server4`

### Compile Single Variant
```bash
# Kyber512 only
make test_kyber_keygen2 test_kyber_client2 test_kyber_server2

# Kyber768 only
make test_kyber_keygen3 test_kyber_client3 test_kyber_server3

# Kyber1024 only
make test_kyber_keygen4 test_kyber_client4 test_kyber_server4
```

## Usage

### Step 1: Generate Keypairs
```bash
./test_kyber_keygen2
```

Output:
```
[*] Generating client Kyber keypair...
[OK] Client keypair generated
[*] Generating server Kyber keypair...
[OK] Server keypair generated
[OK] Wrote client_sk.bin
[OK] Wrote client_pk.bin
[OK] Wrote server_sk.bin
[OK] Wrote server_pk.bin

[OK] All keypairs generated successfully
```

### Step 2: Start Server (Terminal 1)
```bash
./test_kyber_server2
```

Output:
```
========== Kyber Server ==========
Listening on port 5000
==================================

[*] Loading keys...
[OK] Keys loaded

[*] Waiting for client connection...
```

### Step 3: Run Client (Terminal 2)
```bash
# Connect to localhost
./test_kyber_client2

# Or connect to remote server
./test_kyber_client2 192.168.1.100 5000
```

Output:
```
[*] Loading keys...
[OK] Keys loaded

[*] Connecting to server at 127.0.0.1:5000
[+] Connected to server

[STAGE 1] Sending client public key to server...
[+] Client PK sent (800 bytes)

[STAGE 2] Receiving ciphertext from server...
[+] Ciphertext received (768 bytes)

[STAGE 3] Decapsulating to get shared secret...
[+] Shared secret obtained (32 bytes)

[STAGE 4] Sending shared secret to server for verification...
[+] Shared secret sent (32 bytes)

[STAGE 5] Receiving verification result from server...
[+] Server verified shared secret: MATCH

========== KYBER ENCAPSULATION TEST SUCCESSFUL ==========
Client Shared Secret:   32 bytes
Ciphertext:             768 bytes
Total Time:             5 ms
=========================================================
```

### Step 4: View Logs
```bash
# Client log
cat client_kyber.log

# Server log
cat server_kyber.log
```

Log format:
```
status=0,ss_len=32,elapsed_ms=5,user_ms=0.12,sys_ms=0.08,rss_kb=1024
```

Status codes:
- `0` - Success
- `-1` to `-8` - Various error conditions

## Protocol

### Network Protocol

```
                CLIENT                          SERVER

1. KEYPAIR GENERATION
   (offline)                                    (offline)
   - Generate (pk_c, sk_c)                      - Generate (pk_s, sk_s)
   - Save to files                              - Save to files

2. TCP HANDSHAKE
   [SYN]  ───────────────────────────────────>
   <──────────────────────────────────── [SYN-ACK]
   [ACK]  ───────────────────────────────────>

3. SEND CLIENT PK
              [pk_c]
   ───────────────────────────────────────────>
              (size: 4 bytes + payload)

4. ENCAPSULATION & SEND CIPHERTEXT
                                    [Encapsulate]
                                    Generate:
                                    (ct, ss_s)
                                    <── [ct]
   [ct]  <───────────────────────────────────
   (size: 4 bytes + payload)

5. DECAPSULATION
   [Decapsulate]
   Using sk_c and ct
   Generate: ss_c

6. SEND SHARED SECRET
              [ss_c]
   ───────────────────────────────────────────>
              (size: 4 bytes + payload)

7. VERIFICATION
                                    [Compare ss_s == ss_c]
                                    <── [result: 1 byte]
   [result]  <───────────────────────────────────

8. TCP CLOSE
   [FIN]  ───────────────────────────────────>
   <──────────────────────────────────── [FIN-ACK]
```

## Data Sizes (Kyber512)

| Component | Size |
|-----------|------|
| Client Public Key | 800 bytes |
| Client Secret Key | 1632 bytes |
| Ciphertext | 768 bytes |
| Shared Secret | 32 bytes |
| Network overhead (per message) | 4 bytes (length field) |

## Environment Variables

### Client
```bash
# Custom log path
export CLIENT_LOG_PATH=/path/to/custom.log
./test_kyber_client2
```

### Server
```bash
# Custom port (modify source code, default is 5000)
# Logs to server_kyber.log by default
```

## Testing Multiple Modes

### Kyber512 Test
```bash
./test_kyber_keygen2
./test_kyber_server2 &
./test_kyber_client2
```

### Kyber768 Test
```bash
./test_kyber_keygen3
./test_kyber_server3 &
./test_kyber_client3
```

### Kyber1024 Test
```bash
./test_kyber_keygen4
./test_kyber_server4 &
./test_kyber_client4
```

## Performance Characteristics

Typical timings on modern CPU (single run):

| Operation | Kyber512 | Kyber768 | Kyber1024 |
|-----------|----------|----------|-----------|
| KeyGen | 0.2 ms | 0.3 ms | 0.4 ms |
| Encapsulation | 0.3 ms | 0.5 ms | 0.7 ms |
| Decapsulation | 0.4 ms | 0.7 ms | 1.0 ms |
| Total (client) | 1-2 ms | 2-3 ms | 3-4 ms |

## Error Codes

Client errors:
- `-1`: Send PK failed
- `-2`: Receive ciphertext failed
- `-3`: Ciphertext size mismatch
- `-4`: Decapsulation failed
- `-5`: Send shared secret failed
- `-6`: Receive verification result failed
- `-7`: Verification failed (SS mismatch)
- `-8`: Invalid verification result

Server errors (similar range with different stages)

## Troubleshooting

### Connection refused
```
Error: connect() failed: Connection refused
```
**Solution**: Make sure server is running on same machine or correct IP

### File not found
```
Failed to open client_sk.bin
```
**Solution**: Run keygen first to generate key files

### Port already in use
```
bind: Address already in use
```
**Solution**: Wait a few seconds or change port in source code

### Compilation errors
```
error: undefined reference to `crypto_kem_dec'
```
**Solution**: Check that Kyber library files are in parent directory

## Files Generated

After running tests, you'll have:

```
test/
├── test_kyber_keygen2
├── test_kyber_keygen3
├── test_kyber_keygen4
├── test_kyber_client2
├── test_kyber_client3
├── test_kyber_client4
├── test_kyber_server2
├── test_kyber_server3
├── test_kyber_server4
├── client_sk.bin       (regenerated each run)
├── client_pk.bin       (regenerated each run)
├── server_sk.bin       (regenerated each run)
├── server_pk.bin       (regenerated each run)
├── client_kyber.log    (appended to)
└── server_kyber.log    (appended to)
```

## Remote Testing

### Test across network
```bash
# On server machine (192.168.1.100)
ssh user@192.168.1.100
cd kyber-dev/ref/test/
./test_kyber_keygen2
./test_kyber_server2

# On client machine
./test_kyber_client2 192.168.1.100 5000
```

### Test with OpenVPN
```bash
# First establish OpenVPN tunnel (e.g., 10.8.0.1 is server)
./test_kyber_client2 10.8.0.1 5000
```

## References

- **Kyber Specification**: https://pq-crystals.org/kyber/
- **NIST FIPS 203**: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.203.pdf
- **Socket Programming**: man socket, man inet_pton

## License

Same as the Kyber reference implementation (Public domain)
