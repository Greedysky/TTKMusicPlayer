//
// Created by qtfreet on 2016/11/29.
//

#ifndef MYAPPLICATION_CBC128_H
#define MYAPPLICATION_CBC128_H

#include <cstddef>

typedef void (*block128_f)(const unsigned char in[16],
                           unsigned char out[16], const void *key);

void CRYPTO_cbc128_encrypt(const unsigned char *in, unsigned char *out,
                           size_t len, const void *key,
                           unsigned char ivec[16], block128_f block);

void CRYPTO_cbc128_decrypt(const unsigned char *in, unsigned char *out,
                           size_t len, const void *key,
                           unsigned char ivec[16], block128_f block);

#endif //MYAPPLICATION_CBC128_H
