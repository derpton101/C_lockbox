#pragma once


//ECB
//CBC
//CFB
//OFB
//CTR

enum Algo {
    _ECB,
    _CBC,
    _CFB,
    _OFB,
};

char* sha256_hash(char* s, size_t s_);

char* tiny_encrypt(enum Algo a, char* k, size_t k_, char* t, size_t t_);
char* tiny_decrypt(enum Algo a, char* k, size_t k_, char* t, size_t t_);
