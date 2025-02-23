/*
 * token_handler.h
 *
 * created on: 2025-02-23
 *     author: deepaks
 *
 * copyright (c) 2025 Deepak Shanmugam
 */

#ifndef TOKEN_HANDLER_H
#define TOKEN_HANDLER_H

typedef struct token_separator* token_handler;

typedef int (*SeparatorIdentFunc)(char ch); /* function to identify the token seperator */
typedef int (*DelimiterIdentFunc)(char ch); /* function to identify the token delimiter */
typedef int (*EscapeIdentFunc)(char ch);    /* function to identify the escape character within a delimiter */


/*
 * function to create token handler based on two parameters
 * 1. string size: to set the maximum size of the string that can be supported
 * 2. max_token: to set the maximum number of tokens that can be formed out of string
 * Note: once parameter is set, it cannot be changed.
 *
 * default token seperator: all the spaces defined in isspace() function of <ctype.h>
 * default token delimiter: double quotes (")
 * default escape character within a demiliter: back slash '\'
 */
token_handler create_token_handler(int string_size, int max_token);

/*
 * function to free the token handler
 */
void free_token_handler(token_handler handler);

/*
 * function to copy the source string and then tokenize the copy
 * Note: this function can be called 'n' number of times with 'n' number of source strings
 *
 * Returns < 0, indicates error or tokenizing the string failed
 * Returns = 0, indicates empty string or no tokens are present
 * Returns > 0, indicates no of tokens present in the string
 * Note: If return value is max_token + 1, then only first max_tokens are considered
 */
int set_token_handler(token_handler handler, const char *string);

/*
 * function to get the token based on the token index
 * Note: before this, one has to set the token_handler based on the source string
 */
char* get_token_from_handler(token_handler handler, int token_index);

/*
 * function to get the maximum supported string size of a token handler
 */
int get_size_from_token_handler(token_handler handler);

/*
 * function to get the maximum supported tokens of a token handler
 */
int get_max_token_from_handler(token_handler handler);

/*
 * function to change the identification of the token seperator
 */
void change_token_separator(token_handler handler, SeparatorIdentFunc is_separator_fn);

/*
 * function to change the identification of the token delimiter
 */
void change_token_delimiter(token_handler handler, DelimiterIdentFunc is_delimiter_fn);

/*
 * function to change the identification of the escape character within a delimiter
 * Note: currently escape character only works inside delimiter
 */
void change_escape_character(token_handler handler, EscapeIdentFunc is_escape_fn);

#endif
