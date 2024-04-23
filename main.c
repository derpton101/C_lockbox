#include <stdio.h>
#include <stdlib.h>

#include <openssl/sha.h>

#include "aesquick.h"

char* sha256_hash(char* string, int string_size);
int main() {
    char* string = "CauLk";
    char* hash = sha256_hash(string, 5);    
    printf("%s\n%s\n", hash, string);
    free(hash);

    return 0;
}



char* sha256_hash(char* string, int string_size) {

    SHA256_CTX* ctx = malloc(sizeof(SHA256_CTX));

    SHA256_Init(ctx);
    SHA256_Update(ctx, string, string_size);

    char* out = malloc(sizeof(char) * (32 + 1));
    out[32] = 0;

    SHA256_Final(out, ctx);

    free(ctx);
    return out;
}