# CMake minimum version and project details
cmake_minimum_required(VERSION 3.16)
project(PQCKyber VERSION 1.0 LANGUAGES C CXX ASM)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Option to enable/disable tests
option(ENABLE_TESTS "Enable Unit Tests" ON)

# Set the output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(OpenSSL REQUIRED)

add_subdirectory(ref)