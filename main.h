#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#ifdef _USE_READLINE
#include <readline/readline.h>
#endif // _USE_READLINE

#define MAX_INPUT_LENGTH 128
#define INPUT_DELIMITERS " \t"

#ifdef _USE_READLINE
char **completion_function(const char *text, int start, int end);
char *command_generator(const char *text, int state);
#endif

// _MAIN_H
#endif
