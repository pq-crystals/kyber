set(SRCS kem.c indcpa.c polyvec.c poly.c reduce.c ntt.c cbd.c verify.c)
set(TEST_SRCS test_kyber.c randombytes.c)
set(TESTKEX_SRCS test_kex.c randombytes.c kex.c)
set(SPEED_SRCS test_speed.c speed_print.c cpucycles.c randombytes.c kex.c)
set(VECTOR_SRCS test_vectors.c)
set(PQCKAT_SRCS PQCgenKAT_kem.c rng.c)
set(AES_FILES aes256ctr.c)
set(KECCAK_FILES fips202.c sha256.c sha512.c)
set(AES_SRCS ${SRCS} symmetric-aes.c)

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
	add_compile_options(-g)
	add_compile_options(-Wall)
	add_compile_options(-Wno-unused-result)
	add_compile_options(-Wextra)
	add_compile_options(-Wpedantic)
	add_compile_options(-Wmissing-prototypes)
	add_compile_options(-Wredundant-decls)
	add_compile_options(-Wshadow)
	add_compile_options(-Wpointer-arith)

elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	#add_compile_options(-Werror)
	add_compile_options(-Wall)
	add_compile_options(-Wextra)
	add_compile_options(-Wno-unused-result)
	add_compile_options(-Wpedantic)
	add_compile_options(-Wmissing-prototypes)
	add_compile_options(-Wredundant-decls)
	add_compile_options(-Wshadow)
	add_compile_options(-Wpointer-arith)
	add_compile_options(-O3)

elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
	# Warning C4146 is raised when a unary minus operator is applied to an
	# unsigned type; this has nonetheless been standard and portable for as
	# long as there has been a C standard, and we need it for constant-time
	# computations. Thus, we disable that spurious warning.
	add_compile_options(/wd4146)
endif()


# First, do libraries:
add_library(kyber_fips_ref OBJECT ${KECCAK_FILES} ${AES_FILES})
set(_REF_OBJS $<TARGET_OBJECTS:kyber_fips_ref>)

# Iterate over parameter sets:
foreach(X RANGE 2 4)
   # Plain Kyber
   add_library(kyber${X}lib_ref OBJECT ${SRCS} symmetric-shake.c)
   target_compile_options(kyber${X}lib_ref PUBLIC -DKYBER_K=${X})
   set(_REF_OBJS ${_REF_OBJS} $<TARGET_OBJECTS:kyber${X}lib_ref>)

   # Plain test
   add_executable(test_kyber${K${X}}_ref ${TEST_SRCS})
   target_link_libraries(test_kyber${K${X}}_ref PUBLIC kyber ${OPENSSL_CRYPTO_LIBRARY})
   #add_test(test_kyber${K${X}}_ref test_kyber${K${X}}_ref)

   # Plain KEX
   add_executable(test_kex${K${X}}_ref ${TESTKEX_SRCS})
   target_link_libraries(test_kex${K${X}}_ref PUBLIC kyber ${OPENSSL_CRYPTO_LIBRARY})
   #add_test(test_kex${K${{X}}_ref test_kex${K${X}}_ref)

   # Plain speed
   if(NOT WIN32)
   # Plain speed - not yet supported in Windows: TBD
   add_executable(test_speed${K${X}}_ref ${SPEED_SRCS})
   target_compile_options(test_speed${K${X}}_ref PUBLIC -DKYBER_K=${X})
   target_link_libraries(test_speed${K${X}}_ref kyber ${OPENSSL_CRYPTO_LIBRARY})
   #add_test(test_speed${K${X}}_ref test_speed${K${X}}_ref)
   endif()

   # Plain test vectors
   add_executable(test_vectors${K${X}}_ref ${VECTOR_SRCS})
   target_compile_options(test_vectors${K${X}}_ref PUBLIC -DKYBER_K=${X})
   target_link_libraries(test_vectors${K${X}}_ref PRIVATE kyber ${OPENSSL_CRYPTO_LIBRARY})
   if (WIN32) 
      add_test(NAME test_vectors${X}_ref COMMAND ${CMAKE_COMMAND} -E chdir $<TARGET_FILE_DIR:test_vectors${K${X}}_ref> $ENV{ComSpec} /c "$<TARGET_FILE_NAME:test_vectors${K${X}}_ref> | dos2unix > ../tvecs${K${X}}")
   else()
      add_test(NAME test_vectors${K${X}}_ref COMMAND sh -c "$<TARGET_FILE:test_vectors${K${X}}_ref> > tvecs${K${X}}")
   endif()

   # AES Kyber ('90s')
   add_library(kyber${X}aeslib_ref OBJECT ${SRCS} symmetric-aes.c)
   target_compile_options(kyber${X}aeslib_ref PUBLIC -DKYBER_K=${X} -DKYBER_90S)
   set(_REF_OBJS ${_REF_OBJS} $<TARGET_OBJECTS:kyber${X}aeslib_ref>)

   # AES test
   add_executable(test_kyber${K${X}}-90s_ref ${TEST_SRCS})
   target_link_libraries(test_kyber${K${X}}-90s_ref PUBLIC kyber ${OPENSSL_CRYPTO_LIBRARY})
   #add_test(test_kyber${K${X}}-90s_ref test_kyber${K${X}}-90s_ref)

   # AES KEX
   add_executable(test_kex${K${X}}-90s_ref ${TESTKEX_SRCS})
   target_link_libraries(test_kex${K${X}}-90s_ref PUBLIC kyber ${OPENSSL_CRYPTO_LIBRARY})
   #add_test(test_kex${K${{X}}-90s_ref test_kex${K${X}}-90s_ref)

   # AES speed
   if(NOT WIN32)
   # AES speed - not yet supported in Windows: TBD
   add_executable(test_speed${K${X}}-90s_ref ${SPEED_SRCS})
   target_compile_options(test_speed${K${X}}-90s_ref PUBLIC -DKYBER_K=${X} -DKYBER_90S)
   target_link_libraries(test_speed${K${X}}-90s_ref kyber ${OPENSSL_CRYPTO_LIBRARY})
   #add_test(test_speed${K${X}}-90s_ref test_speed${K${X}}-90s_ref)
   endif()

   # AES test vectors
   add_executable(test_vectors${K${X}}-90s_ref ${VECTOR_SRCS})
   target_compile_options(test_vectors${K${X}}-90s_ref PUBLIC -DKYBER_K=${X} -DKYBER_90S)
   target_link_libraries(test_vectors${K${X}}-90s_ref PRIVATE kyber ${OPENSSL_CRYPTO_LIBRARY})
   if (WIN32) 
      add_test(NAME test_vectors${X}-90s_ref COMMAND ${CMAKE_COMMAND} -E chdir $<TARGET_FILE_DIR:test_vectors${K${X}}-90s_ref> $ENV{ComSpec} /c "$<TARGET_FILE_NAME:test_vectors${K${X}}-90s_ref> | dos2unix > ../tvecs${K${X}}-90s")
   else()
      add_test(NAME test_vectors${K${X}}-90s_ref COMMAND sh -c "$<TARGET_FILE:test_vectors${K${X}}-90s_ref> > tvecs${K${X}}-90s")
   endif()

endforeach()

# PQCKATs
add_executable(PQCgenKAT_kem_ref ${PQCKAT_SRCS})
target_compile_options(PQCgenKAT_kem_ref PUBLIC)
target_link_libraries(PQCgenKAT_kem_ref PRIVATE kyber ${OPENSSL_CRYPTO_LIBRARY})
if (WIN32) 
      # Necessary cludge to make hashes be ignorant of Windows CRLF file formatting:
      add_test(NAME PQCgenKAT_kem_ref COMMAND ${CMAKE_COMMAND} -E chdir $<TARGET_FILE_DIR:PQCgenKAT_kem_ref> $ENV{ComSpec} /c "$<TARGET_FILE_NAME:PQCgenKAT_kem_ref> && dos2unix -n PQCsignKAT_KEM.rsp ../PQCsignKAT_KEM.rsp && dos2unix -n PQCsignKAT_KEM.req ../PQCsignKAT_KEM.req")
else()
      add_test(PQCgenKAT_kem_ref PQCgenKAT_kem_ref)
endif()

add_test(Hashes_test_ref sha256sum -c ../../SHA256SUMS)

set(REF_OBJS ${_REF_OBJS} PARENT_SCOPE)
