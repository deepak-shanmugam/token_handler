/*
 * token_handler.h
 */

#ifndef TOKEN_HANDLER_H
#define TOKEN_HANDLER_H

/*
 * function pointer template:
 * to check if the function argument string starts with a delimiter/enclosure/escape
 * if it is, then return the length of the delimiter/enclosure/escape
 * else return 0
 */
typedef int (*checkFunction)(char*);

int initTokenHandler();
void destroyTokenHandler();

int setTokenHandler(char *str);
char* getTokenFromHandler(int index);

void setDelimiterCheck(checkFunction check_fn);
void setEnclosureCheck(checkFunction check_fn);
void setEscapeCheck(checkFunction check_fn);

void resetDelimiterCheck();
void resetEnclosureCheck();
void resetEscapeCheck();

#endif
