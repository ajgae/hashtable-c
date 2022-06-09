#ifndef _PROMPT_H
#define _PROMPT_H

#include <stdio.h>
#ifdef _USE_READLINE
#include <readline/readline.h>
#else
#include <string.h>
#endif

#define MAX_INPUT_LENGTH 128

char *prompt();

#ifdef _USE_READLINE
void init_readline();
char **completion_function(const char *text, int start, int end);
char *command_generator(const char *text, int state);
#endif

#endif // _PROMPT_H
