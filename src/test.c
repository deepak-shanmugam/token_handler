/*
 * test.c
 *
 * created on: 2025-02-23
 *     author: deepaks
 *
 * copyright (C) 2025 Deepak Shanmugam
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token_handler.h"

#define STRING_LEN 1024
#define MAX_TOKEN 5

static int init();
static void close_all();
static int custom_separator(char ch);
static int custom_delimiter(char ch);
static int custom_escape_character(char ch);

char *string;
token_handler tok_handler;
FILE *input;
FILE *output;

static int 
init()
{
    string = calloc(STRING_LEN, sizeof(*string));
    tok_handler = create_token_handler(STRING_LEN, MAX_TOKEN);
    input = fopen("input.txt","r");
    output = fopen("output.txt","w");
    if (string == NULL || tok_handler == NULL || input == NULL || output == NULL) {
        close_all();
        return 1;
    }
    return 0;
}

static void 
close_all()
{
    free(string);
    free_token_handler(tok_handler);
    fclose(input);
    fclose(output);
    string = NULL;
    tok_handler = NULL;
    input = NULL;
    output = NULL;
}

int 
main(int argc, char *argv[])
{
    char *token;
    int index;
    if (init() != 0) {
        fprintf(stderr,"failed to init!\n");
        return 1;
    }
    //change_token_separator(tok_handler, custom_separator);
    //change_token_delimiter(tok_handler, custom_delimiter);
    //change_escape_character(tok_handler, custom_escape_character);
    while (fgets(string, STRING_LEN, input) != NULL) {
        if (set_token_handler(tok_handler, string) < 0) {
            fprintf(stderr,"failed to set token handler!\n");
            puts("");
            continue;
        }
        index = 0;
        while ((token = get_token_from_handler(tok_handler, index)) != NULL) {
            fprintf(output,"token: %s, length: %lu\n",token,strlen(token));
            index++;
        }
        fprintf(output,"\n");
    }
    close_all();
    puts("end of program!");
    return 0;
}

static int 
custom_separator(char ch)
{
    return (ch == ',' || ch == '\n') ? 1 : 0;
}

static int 
custom_delimiter(char ch)
{
    return (ch == '\0') ? 1 : 0;
}

static int 
custom_escape_character(char ch)
{
    return (ch == '\\') ? 1 : 0;
}
