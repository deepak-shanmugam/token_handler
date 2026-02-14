/*
 * client.c
 */

#include <stdio.h>
#include <string.h>

#include <token_handler.h>

int main()
{
    int return_code;
    char buffer[256];
    char *token;
    initTokenHandler();
    do {
        printf("Enter string: ");
        fgets(buffer, 256, stdin);
        buffer[strlen(buffer) - 1] = '\0';
        return_code = setTokenHandler(buffer);
        printf("return code: %d\n", return_code);
        for (int i=0; ;i++) {
            token = getTokenFromHandler(i);
            if (token == NULL) {
                break;
            }
            printf("%d. %s", i+1, token);
            printf("\n");
        }
    } while (strcmp(buffer,"exit") != 0);
    destroyTokenHandler();
    return 0;
}
