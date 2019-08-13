import os
import shutil
import subprocess
from pathlib import Path
TARGET_FOLDER = "../PQClean-kyber/crypto_kem/"

KYBER_FILES = [
    'LICENSE',
    'api.h',
    'cbd.c',
    'cbd.h',
    'indcpa.c',
    'indcpa.h',
    'kem.c',
    'ntt.c',
    'ntt.h',
    'params.h',
    'poly.c',
    'poly.h',
    'polyvec.c',
    'polyvec.h',
    'reduce.c',
    'reduce.h',
    'symmetric-fips202.c',
    'symmetric.h',
    'verify.c',
    'verify.h',
]

KYBER_AVX2_FILES = [
    'LICENSE',
    'api.h',
    'basemul.S',
    'cbd.h',
    'cbd.c',
    'consts.c',
    'fips202x4.c',
    'fips202x4.h',
    'fq.inc',
    'fq.s',
    'indcpa.c',
    'indcpa.h',
    'invntt.s',
    'kem.c',
    'ntt.h',
    'ntt.s',
    'params.h',
    'poly.c',
    'poly.h',
    'polyvec.c',
    'polyvec.h',
    'reduce.h',
    'rejsample.c',
    'rejsample.h',
    'shuffle.inc',
    'shuffle.s',
    'symmetric-fips202.c',
    'symmetric.h',
    'verify.c',
    'verify.h',
]

params = [
#    {'name': 'kyber512', 'impl': 'clean', 'def': ['KYBER_K=2'],
#     'src': 'ref',
#     'undef': ['KYBER_90S'],
#     'files': KYBER_FILES},
    {'name': 'kyber512', 'impl': 'avx2', 'def': ['KYBER_K=2'],
     'src': 'avx2',
     'undef': ['KYBER_90S'],
     'files': KYBER_AVX2_FILES},
#    {'name': 'kyber768', 'impl': 'clean', 'def': ['KYBER_K=3'],
#     'src': 'ref',
#     'undef': ['KYBER_90S'],
#     'files': KYBER_FILES},
#    {'name': 'kyber1024', 'impl': 'clean', 'def': ['KYBER_K=4'],
#     'src': 'ref',
#     'undef': ['KYBER_90S'],
#     'files': KYBER_FILES},
]

for param in params:
    parameterSet = param['name']
    pqcleanDir = f"{TARGET_FOLDER}/{parameterSet}/{param['impl']}/"

    # delete old files
    if Path(pqcleanDir).exists():
        shutil.rmtree(pqcleanDir)
    os.makedirs(pqcleanDir)

    nmspc = (f"PQCLEAN_{parameterSet.upper().replace('-','')}"
             f"_{param['impl'].upper()}")
    for f in param['files']:
        # copy over common source files
        shutil.copyfile(f"{param['src']}/{f}", f"{pqcleanDir}/{f}")

        # namespace source files
        cmd = f"sed -i 's/PQCLEAN_NAMESPACE/{nmspc}/g' '{pqcleanDir}/{f}'"
        subprocess.check_call(cmd, shell=True)

    for f in param['files']:
        # remove preprocessor conditionals
        cmd = ("unifdef -m " + " ".join(["-D"+d for d in param['def']]) + " "
               + " ".join(['-U'+d for d in param['undef']])
               + f" -f params/params-{param['name']}.h"
               + f" {pqcleanDir}/{f}")
        print(cmd)
        subprocess.call(cmd, shell=True)
    # copy over param specific files

    # copy over Makefiles
    for f in os.listdir(f"make/{param['impl']}"):
        shutil.copyfile(f"make/{param['impl']}/{f}", f"{pqcleanDir}/{f}")

        # replace lib name
        cmd = f"sed -i 's/SCHEME_NAME/{parameterSet}/g' {pqcleanDir}/{f}"
        subprocess.call(cmd, shell=True)

    # run astyle to fix formatting due to namespace
    cmd = f"astyle --project {pqcleanDir}/*.[ch]"
    subprocess.call(cmd, shell=True)
