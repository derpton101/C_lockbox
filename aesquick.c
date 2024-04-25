#include "aesquick.h"

#include <stdio.h>

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

//#define _DEBUG_MODE_ 1
void debug(char* str) {
#ifdef _DEBUG_MODE_

    printf("%s\n", str);
    fflush(stdout);
#endif
}

char* sha256_hash(char* string, size_t string_size) {
    debug("Started sha256!");

    SHA256_CTX* ctx = malloc(sizeof(SHA256_CTX));



    SHA256_Init(ctx);
    SHA256_Update(ctx, string, string_size);

    char* out = malloc(sizeof(char) * (32 + 1));
    out[32] = 0;

    SHA256_Final(out, ctx);

    
    free(ctx);
    debug("Finished sha256!");
    return out;
}
// INTERMEDIATE
EVP_CIPHER* step_init_1(enum Algo a) {

    debug("Started algo routing!");

    const EVP_CIPHER* ciph;

    switch (a)
    {
    case _ECB:
        ciph = EVP_aes_256_ecb();
        break;
    case _OFB:
        ciph = EVP_aes_256_ofb();
        break;
    case _CBC:
        ciph = EVP_aes_256_cbc();
        break;
    case _CFB:
        ciph = EVP_aes_256_cfb128();
        break;
    default:
        ciph = NULL;
        break;
    }

    debug("Finished algo routing!");

    return ciph;
}





char* tiny_encrypt(enum Algo a, char* key, size_t key_len, char* text, size_t text_len, size_t* out_len) {
    char* k_in = sha256_hash(key, key_len);

    char* _iv = sha256_hash(k_in, sizeof (char) * 32);
    char* iv = malloc(sizeof (char) * AES_BLOCK_SIZE);

    debug("Setting IV!");
    for (int i = 0; i < 16; i++) {
        iv[i] = _iv[i];
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    
    debug("EVP init!");
    EVP_EncryptInit(ctx, step_init_1(a), k_in, iv);
    debug("Finished EVP init!");
    debug("Freeing IV!");
    free(_iv);
    free(iv);
    debug("Freed IV!");
    //Pad the text

    int dif = AES_BLOCK_SIZE - (text_len % AES_BLOCK_SIZE);
    size_t padded_ = text_len + dif;

    char* padded = malloc(sizeof(char) * padded_);
    for (int i = 0; i < text_len; i++) {
        padded[i] = text[i];
    }

    srand((*(int*)k_in));
    free(k_in);
    for (int i = (text_len); i < padded_; i++) {
        if (i == text_len) {
            padded[i] = 0;
            continue;
        }
        char c = rand() % 254 + 1;
        padded[i] = c;
    }

    int out_ = padded_;
    char* out = malloc(sizeof (char) * (padded_ + AES_BLOCK_SIZE));

    EVP_EncryptUpdate(ctx, out, &out_, padded, padded_);
    free(padded);

    int f_ = 0;
    EVP_EncryptFinal_ex(ctx, out+out_, &f_);
    EVP_CIPHER_CTX_free(ctx);

    *out_len = out_ + f_;
    return out;
}

char* tiny_decrypt(enum Algo a, char* key, size_t key_len, char* text, size_t text_len, size_t* out_len) {
    char* k_in = sha256_hash(key, key_len);

    char* _iv = sha256_hash(k_in, sizeof(char) * 32);
    char* iv = malloc(sizeof (char) * AES_BLOCK_SIZE);

    debug("Setting IV!");
    for(int i = 0; i < 16; i++) {
        iv[i] = _iv[i];
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    debug("EVP init!");
    EVP_DecryptInit(ctx, step_init_1(a), k_in, iv);
    free(k_in);
    debug("Finished EVP init!");
    debug("Freeing IV!");
    free(_iv);
    free(iv);
    debug("Freed IV!");


    debug("Making output buffer!");
    char* _output = malloc(sizeof (char) * (text_len + AES_BLOCK_SIZE));
    debug("Output buffer created!");

    debug("Doing final!");
    int u_ = text_len;
    int ret = EVP_DecryptUpdate(ctx, _output, &u_, text, text_len);
    debug("Finished update!");
    int f_ = 0;
    int ret2 = EVP_DecryptFinal(ctx, _output+u_, &f_);
    debug("Finished!");

    *out_len = f_ + u_;

    size_t back = *out_len - 1;
    while (_output[back] && back >= 0) (back)--;

    if (back <= 0 || back == -1) {
        //printf("No zero found: returning NULL\n");

        EVP_CIPHER_CTX_free(ctx);
        free(_output);
        *out_len = 0;
        return NULL;
    }

    *out_len = back;
    char* output = malloc(sizeof(char) * (*out_len + 1));
    output[*out_len] = 0;
    for (int i = 0; i < *out_len; i++)
        output[i] = _output[i]; 
    free(_output);

    EVP_CIPHER_CTX_free(ctx);

    return output;
}