/*
 * token_handler.c
 *
 * created on: 2025-02-23
 *     author: deepaks
 *
 * copyright (c) 2025 Deepak Shanmugam
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token_handler.h"

struct token_separator {
    char *string;
    char **token;
    int  size;
    int  max_token;
    SeparatorIdentFunc is_separator;
    DelimiterIdentFunc is_delimiter;
    EscapeIdentFunc    is_escape_char;
};

/* static functions */
static int seperate_string_token(token_handler handler);
static int set_token(token_handler handler);

static int is_space(char ch);
static int is_double_quote(char ch);
static int is_back_slash(char ch);


token_handler create_token_handler(int string_size, int max_token)
{
    token_handler handler;
    if (string_size <= 0 || max_token <= 0 || max_token > string_size) {
        fprintf(stderr,"create_token_handler: argument error!\n");
        return NULL;
    }
    handler = calloc(1, sizeof(*handler));
    if (handler == NULL) {
        fprintf(stderr,"create_token_handler: memory allocation error!\n");
        return NULL;
    }
    handler->string = calloc(string_size, sizeof(*handler->string));
    handler->token = calloc(max_token, sizeof(*handler->token));
    if (handler->string == NULL || handler->token == NULL) {
        fprintf(stderr,"create_token_handler: initialization failed!\n");
        free_token_handler(handler);
        return NULL;
    }
    handler->size           = string_size;
    handler->max_token      = max_token;
    handler->is_separator   = is_space;
    handler->is_delimiter   = is_double_quote;
    handler->is_escape_char = is_back_slash;
    return handler;
}

void free_token_handler(token_handler handler)
{
    if (handler == NULL) {
        return;
    }
    free(handler->string);
    free(handler->token);
    handler->string         = NULL;
    handler->token          = NULL;
    handler->size           = 0;
    handler->max_token      = 0;
    handler->is_separator   = NULL;
    handler->is_delimiter   = NULL;
    handler->is_escape_char = NULL;
    free(handler);
}

int set_token_handler(token_handler handler, const char *string)
{
    int index;
    if (handler == NULL || string == NULL) {
        fprintf(stderr,"set_token_handler: argument error!\n");
        return -1;
    }
    memset(handler->string, '\0', handler->size);
    strncpy(handler->string, string, handler->size);
    handler->string[handler->size - 1] = '\0';
    for (index = 0; index < handler->max_token; index++) {
        handler->token[index] = NULL;
    }
    if (seperate_string_token(handler) != 0) {
        fprintf(stderr,"seperate_string_token: missing delimiter enclosure!\n");
        return -2;
    }
    return set_token(handler);
}

char* get_token_from_handler(token_handler handler, int token_index)
{
    if (handler == NULL || token_index < 0) {
        fprintf(stderr,"get_token_from_handler: argument error!\n");
        return NULL;
    }
    if (token_index >= handler->max_token) {
        return NULL;
    }
    return handler->token[token_index];
}

int get_size_from_token_handler(token_handler handler)
{
    if (handler == NULL) {
        return 0;
    }
    return handler->size;
}

int get_max_token_from_handler(token_handler handler)
{
    if (handler == NULL) {
        return 0;
    }
    return handler->max_token;
}

void change_token_separator(token_handler handler, SeparatorIdentFunc is_separator_fn)
{
    if (handler == NULL || is_separator_fn == NULL) {
        fprintf(stderr,"change_token_separator: argument error!\n");
        return;
    }
    handler->is_separator = is_separator_fn;
}

void change_token_delimiter(token_handler handler, DelimiterIdentFunc is_delimiter_fn)
{
    if (handler == NULL || is_delimiter_fn == NULL) {
        fprintf(stderr,"change_token_delimiter: argument error!\n");
        return;
    }
    handler->is_delimiter = is_delimiter_fn;
}

void change_escape_character(token_handler handler, EscapeIdentFunc is_escape_fn)
{
    if (handler == NULL || is_escape_fn == NULL) {
        fprintf(stderr,"change_escape_in_delimiter: argument error!\n");
        return;
    }
    handler->is_escape_char = is_escape_fn;
}

/*
 * to seperate a single string into individual tokens
 */
static int seperate_string_token(token_handler handler)
{
    char *str = handler->string;
    int inside_delimiter = 0;
    while (*str) {
        if (!inside_delimiter && handler->is_separator(*str)) {
            *str = '\0';
            str++;
            continue;
        }
        if (handler->is_delimiter(*str)) {
            if (inside_delimiter && handler->is_escape_char(*(str-1))) {
                str++;
                continue;
            }
            inside_delimiter = !inside_delimiter;
        }
        str++;
    }
    if (inside_delimiter) {
        return 1;
    }
    return 0;
}

/*
 * to set token pointers to point individual seperated tokens
 */
static int set_token(token_handler handler)
{
    int index = 0;
    int token_index = 0;
    int token_found = 0;
    for ( ; index < handler->size; index++) {
        if (handler->string[index] == '\0') {
            token_found = 0;
        } else {
            if (token_found == 0) {
                if (token_index >= handler->max_token) {
                    token_index++;
                    return token_index;
                }
                handler->token[token_index++] = &handler->string[index];
            }
            token_found = 1;
        }
    }
    if (token_index == 0) {
        handler->token[0] = handler->string;
    }
    return token_index;
}

static int is_space(char ch)
{
    return isspace((unsigned char)ch);
}

static int is_double_quote(char ch)
{
    return (ch == '\"') ? 1 : 0;
}

static int is_back_slash(char ch)
{
    return (ch == '\\') ? 1 : 0;
}
