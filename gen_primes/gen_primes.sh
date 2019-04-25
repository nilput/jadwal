#!/bin/sh
#this requires openssl
( make -f genprimes.mk && \
    python3 gen_primes.py --mode=funcs --bits=32 --tries=70 > div_32_funcs_tmp.h && \
    mv div_32_funcs_tmp.h div_32_funcs.h && echo 'generated div_32_funcs.h') || echo 'failed to generate header' && exit 1
