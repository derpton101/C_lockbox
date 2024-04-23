#include "aesquick.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>


char* sha256_hash(char* string, size_t string_size) {

    SHA256_CTX* ctx = malloc(sizeof(SHA256_CTX));

    SHA256_Init(ctx);
    SHA256_Update(ctx, string, string_size);

    char* out = malloc(sizeof(char) * (32 + 1));
    out[32] = 0;

    SHA256_Final(out, ctx);

    free(ctx);
    return out;
}
// INTERMEDIATE
struct EVP_CIPHER_I {
    EVP_CIPHER* ciph;
    EVP_CIPHER_CTX* ctx;
};


struct EVP_CIPHER_I* step_init_1(enum Algo a) {
    EVP_CIPHER* ciph;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

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

    if (ciph == NULL) {
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    struct EVP_CIPHER_I* out = malloc(sizeof (struct EVP_CIPHER_I));

    out->ciph = ciph;
    out->ctx = ctx;

    return ctx;
}





char* tiny_encrypt(enum Algo a, char* key, size_t key_len, char* text, size_t text_len) {
    struct EVP_CIPHER_I* ciph = step_init_1(a);
    char* IV = malloc (sizeof(char) * 16);

    char* k_in = sha256_hash(key, key_len);

    char* _iv = sha256_hash(k_in, sizeof (char) * 32);
    char* iv = malloc(sizeof (char) * 16);

    for (int i = 0; i < 16; i++) {
        iv[i] = _iv;
    }

    EVP_EncryptInit_ex(ciph->ctx, ciph->ciph, NULL, k_in, iv);
    free(iv);
    
    //Pad the text

    int dif = 16 - (text_len % 16);
    size_t padded_ = text_len + dif;

    char* padded = malloc(sizeof(char) * padded_);

    srand((*(int*)k_in));
    for (int i = (text_len); i < padded_; i++) {
        if (i == text_len) {
            padded[i] = 0;
            continue;
        }
        char c = rand() % 254 + 1;
        padded[i] = c;
    }
    free(k_in);

    EVP_EncryptUpdate(ciph->ctx, padded, )
    
}




char* tiny_encrypt(enum Algo a, char* key, size_t key_len, char* text, size_t text_len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
}