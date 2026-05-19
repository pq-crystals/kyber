# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.html).

### Summary
This version introduces modifications to the `ref` implementation for performance analysis and benchmarking purposes. The core cryptographic logic of the original Kyber algorithm remains unchanged. All modifications are confined to the `ref` directory and supplementary testing scripts.

### Added
- **Performance Benchmarking Framework:**
  - Introduced a `timing_info_t` struct in `ref/kem.h` to capture execution time for key generation, encapsulation, and decapsulation steps.
  - Integrated `clock_gettime` with `CLOCK_MONOTONIC` in `ref/kem.c` to precisely measure the duration of `crypto_kem_keypair`, `crypto_kem_enc`, and `crypto_kem_dec`. This required adding `#include <time.h>` and defining `_POSIX_C_SOURCE`.
  - Added `print_timing_info()` function to display aggregated timing results.
- **Automated Testing Support:**
  - Added `run_test` function prototype in `ref/kem.h` to facilitate running tests multiple times for stable performance metrics.

### Changed
- **Disabled Debug Outputs:**
  - The extensive `printf` statements previously used for tracing the algorithm's flow in `ref/kem.c` and `ref/indcpa.c` have been disabled to allow for clean performance measurement. The original versions with these outputs are preserved in `kem.c.old` and `indcpa.c.old` for reference.
- **Code Formatting:**
  - Applied consistent code formatting (e.g., spacing, newlines) across multiple files in the `ref` directory to improve readability.
- **Makefile Adjustments:**
  - Modified `ref/Makefile` to disable the compilation of `test_vectors` targets, streamlining the build process for performance testing.

### Removed
- (No features removed from the core implementation)

### Acknowledgements
This project is a fork of the official [pq-crystals/kyber](https://github.com/pq-crystals/kyber) repository. The modifications, available at [quannguyen247/kyber-dev](https://github.com/quannguyen247/kyber-dev), are focused on performance analysis and benchmarking. The core cryptographic logic of the original public domain implementation of CRYSTALS-Kyber remains unchanged.