/*
 * token_handler.c
 *
 * created on: 2025-02-14
 * created by: deepaks
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <token_handler.h>

// data definition
struct token_handler {
    char *string;
    char **token;
    unsigned int str_size;
    unsigned int tok_size;
    checkFunction delimiter_fn;
    checkFunction enclosure_fn;
    checkFunction escape_fn;
};

// static functions
static int setTokens();
static void clearTokenPointers();
static unsigned int getIncreasedSizeToAccumulate(unsigned int string_length);

static int default_delimiter_check(char *str);
static int default_enclosure_check(char *str);
static int default_escape_check(char *str);

// global variable
static struct token_handler handler = {
    NULL, NULL, 0, 0,
    default_delimiter_check,
    default_enclosure_check,
    default_escape_check
};


int
initTokenHandler()
{
    if (handler.string == NULL) {
        handler.str_size = 1;
        handler.string = calloc(handler.str_size, sizeof(*handler.string));
        if (handler.string == NULL) {
            return -1;
        }
    }
    if (handler.token == NULL) {
        handler.tok_size = 1;
        handler.token = calloc(handler.tok_size, sizeof(*handler.token));
        if (handler.token == NULL) {
            destroyTokenHandler();
            return -1;
        }
        clearTokenPointers();
    }
    return 0;
}

void
destroyTokenHandler()
{
    if (handler.string != NULL) {
        free(handler.string);
        handler.string = NULL;
        handler.str_size = 0;
    }
    if (handler.token != NULL) {
        free(handler.token);
        handler.token = NULL;
        handler.tok_size = 0;
    }
    resetDelimiterCheck();
    resetEnclosureCheck();
    resetEscapeCheck();
}

void
setDelimiterCheck(checkFunction check_fn)
{
    handler.delimiter_fn = check_fn;
}

void
setEnclosureCheck(checkFunction check_fn)
{
    handler.enclosure_fn = check_fn;
}

void
setEscapeCheck(checkFunction check_fn)
{
    handler.escape_fn = check_fn;
}

void
resetDelimiterCheck()
{
    handler.delimiter_fn = default_delimiter_check;
}

void
resetEnclosureCheck()
{
    handler.enclosure_fn = default_enclosure_check;
}

void
resetEscapeCheck()
{
    handler.escape_fn = default_escape_check;
}

int
setTokenHandler(char *str)
{
    unsigned int length;
    unsigned int increased_size;
    void *ptr;
    if (handler.string == NULL || str == NULL) {
        return -1;
    }
    length = strlen(str);
    if (handler.str_size <= length) {
        increased_size = getIncreasedSizeToAccumulate(length);
        ptr = realloc(handler.string, increased_size * sizeof(*handler.string));
        if (ptr == NULL) {
            return -2;
        }
        memset(ptr, 0, increased_size);
        handler.string = ptr;
        handler.str_size = increased_size;
    }
    strncpy(handler.string, str, handler.str_size);
    clearTokenPointers();
    if (setTokens() != 0) {
        return -3;
    }
    return 0;
}

char*
getTokenFromHandler(int index)
{
    if (index < 0 || index >= handler.tok_size) {
        return NULL;
    }
    return handler.token[index];
}

/*
 * pending implementation for Escape string in token handler
 */
static int
setTokens()
{
    int index;
    int jump;
    int token_starts = 1;
    int token_index = 0;
    int inside_enclosure = 0;
    void *ptr;
    if (handler.token == NULL) {
        return -1;
    }
    index = 0;
    while (1) {
        // when the handler's current string index is the start of the new token
        if (token_starts) {
            // when the current no.of tokens exceeded the handler's token size
            if (token_index >= handler.tok_size) {
                ptr = realloc(handler.token, handler.tok_size * 2 * sizeof(*handler.token));
                if (ptr == NULL) {
                    return -3;
                }
                handler.token = ptr;
                for (int i=0; i<handler.tok_size; i++) {
                    handler.token[handler.tok_size + i] = NULL;
                }
                handler.tok_size *= 2;
            }
            token_starts = 0;
            handler.token[token_index] = &handler.string[index];
            token_index++;
        }
        // when the end of the token reached
        if (handler.string[index] == '\0') {
            break;
        }
        // when the enclosure string comes
        if ((jump = handler.enclosure_fn(&handler.string[index])) > 0) {
            inside_enclosure = (!inside_enclosure);
            index += jump;
            continue;
        }
        // when the delimiter string comes and it is not inside enclosure
        if (!inside_enclosure && (jump = handler.delimiter_fn(&handler.string[index])) > 0) {
            memset(&handler.string[index], 0, jump);
            index += jump;
            token_starts = 1;
            continue;
        }
        index++;
    }
    if (inside_enclosure) {
        return -4;
    }
    return 0;
}

static unsigned int
getIncreasedSizeToAccumulate(unsigned int string_length)
{
    unsigned int str_size = handler.str_size;
    if (handler.str_size == 0) {
        return 0;
    }
    while (str_size <= string_length) {
        str_size *= 2;
    }
    return str_size;
}

static void
clearTokenPointers()
{
    for (int i = 0; i < handler.tok_size; i++) {
        handler.token[i] = NULL;
    }
}

static int
default_delimiter_check(char *str)
{
    if (str == NULL) {
        return 0;
    }
    if (str[0] == ',') {
        return 1;
    }
    return 0;
}

static int
default_enclosure_check(char *str)
{
    if (str == NULL) {
        return 0;
    }
    if (str[0] == '\"') {
        return 1;
    }
    return 0;
}

static int
default_escape_check(char *str)
{
    if (str == NULL) {
        return 0;
    }
    if (str[0] == '\\') {
        return 1;
    }
    return 0;
}
