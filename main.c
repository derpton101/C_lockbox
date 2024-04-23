#include <stdio.h>
#include <stdlib.h>

#include <openssl/sha.h>

#include "aesquick.h"


int main() {
    char* string = "Cats are some amazing creatures";

    for (int i = 0; i < 1; i++) {

        size_t new_len = 0;
        char* str = tiny_encrypt(_CBC, "meow", 4, string, 32, &new_len);
        printf("%s\n", str);
        fflush(stdout);


        size_t len = 0;
        char* str2 = tiny_decrypt(_CBC, "meow", 4, str, new_len, &len);
        if (str2 == 0 || len == 0) {
            printf("Fail to decrypt!");
            break;
        }
        
        printf("%s\n", str2);
        for (int i = 0; i < len; i++) putchar(str2[i]);


        free(str);
        free(str2);


    }
    return 0;
}
