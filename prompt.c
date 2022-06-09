#include "prompt.h"

#ifdef _USE_READLINE
const int num_commands = 7;
const char *commands[] = {
    "exit", "hash", "put", "q", "quit", "remove", "show",
};
#endif

char *prompt(char* user_input) {
#ifdef _USE_READLINE
  user_input = readline("> ");
#else // alternate prompt, ugly when input > MAX_INPUT_LENGTH but
      // shouldn't cause actual issues
  fputs("> ", stdout);

  fgets(user_input, MAX_INPUT_LENGTH, stdin);
  int len = strlen(user_input);
  if (len > 0 && user_input[len - 1] == '\n') {
    user_input[len - 1] = '\0';
  }
#endif
  return user_input;
}

#ifdef _USE_READLINE
void init_readline() {
  rl_completion_entry_function = command_generator;
  rl_attempted_completion_function = completion_function;
}

char **completion_function(const char *text, int start, int end) {
  char **matches = NULL;
  if (start == 0) {
    matches = rl_completion_matches(text, command_generator);
  }
  return matches;
}

char *command_generator(const char *text, int state) {
  static int i;
  static int len; // cached for efficiency

  if (state == 0) {
    // initialize command generator
    i = 0;
    len = strlen(text);
  }

  for (; i < num_commands; ++i) {
    if (!strncmp(commands[i], text, len)) {
      char *res = strdup(commands[i]);
      ++i;
      return res;
    }
  }

  // if no command names matched
  return NULL;
}
#endif

