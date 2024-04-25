#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "aesquick.h"

int file_en(char* file, char* key, size_t key_, char* out_name, enum Algo a) {
    FILE* f = fopen(file, "rb");

    size_t len = 0;
    while (fgetc(f) != -1) len++;
    fseek(f, 0, SEEK_SET);
    
    char* data = malloc(sizeof (char) * len);
    size_t read = fread(data, 1, len, f);

    if (read != len) {
    
        printf("Failed read!\n");
        fflush(stdout);

        free(data);
        fclose(f);

        return -1;
    }
    
    fclose(f);

    size_t out_ = 0;
    char* output = tiny_encrypt(a, key, key_, data, len, &out_);    

    FILE* out_f = fopen(out_name, "wb");

    if (fwrite(output, 1, out_, out_f) != out_) {
        printf("Failed write!");
        fflush(stdout);
        fclose(out_f);
        free(data);
        return -1;
    } 
    free(data);
    fclose(out_f);
    free(output);
    return 0;
}
int file_dec(char* file, char* key, int key_, char* out_name, enum Algo a) {
    FILE* f = fopen(file, "rb");

    size_t len = 0;
    while (fgetc(f) != -1) len++;
    fseek(f, 0, SEEK_SET);
    
    char* data = malloc(sizeof (char) * len);

    if (fread(data, 1, len, f) != len) {
        printf("Failed read!\n");
        fflush(stdout);

        free(data);
        fclose(f);

        return -1;
    }
    
    fclose(f);

    size_t out_ = 0;
    char* output = tiny_decrypt(a, key, key_, data, len, &out_);    

    FILE* out_f = fopen(out_name, "wb");

    if (fwrite(output, 1, out_, out_f) != out_) {
        printf("Failed write!");
        fflush(stdout);
        fclose(out_f);
        free(data);
        return -1;
    } 
    free(data);
    fclose(out_f);
    free(output);
    return 0;
}




int main() {

    

    return 0;
}
