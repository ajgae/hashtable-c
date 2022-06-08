#include "main.h"
#include "hashtable.h"
#include <string.h>

int main(int argc, char **argv) {
  printf("Hello! Welcome to the hashtable prompt program.\n");
  HashTable ht;
  hashtable_init(&ht);
  hashtable_adjust_capacity(&ht, 4);
  
  char *user_input = malloc(MAX_INPUT_LENGTH);

  for (;;) {
    printf("> ");

    fgets(user_input, MAX_INPUT_LENGTH, stdin);
    int len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
      user_input[len - 1] = '\0';
    }

    char *token = strtok(user_input, INPUT_DELIMITERS);

    if (!strncmp("exit", token, MAX_INPUT_LENGTH)) {
      printf("Exiting...\n");
      break;
    }

    else if (!strncmp("put", token, MAX_INPUT_LENGTH)) {
      char *key = strtok(NULL, INPUT_DELIMITERS);
      char *value = strtok(NULL, INPUT_DELIMITERS);
      if (key == NULL || value == NULL) {
        fprintf(stderr,
                "Error: key and value arguments required for puts command\n");
      } else {
        printf("Inserting key-value pair: %s => %s\n", key, value);
        hashtable_put(&ht, key, value);
      }
    }

    else if (!strncmp("show", token, MAX_INPUT_LENGTH)) {
      printf("Capacity: %d\n", ht.capacity);
      printf("Load factor: %f\n", hashtable_get_load_factor(&ht));
      printf("Key-value pairs:\n");
      for (int i = 0; i < ht.capacity; ++i) {
        Entry entry = ht.entries[i];
        if (entry.key != NULL) {
          printf("  %s => %s\n", entry.key, entry.value);
        }
      }
    }

    else {
      fprintf(stderr, "Error: unrecognized command: %s\n", token);
    }
  }

  // free and exit
  hashtable_free(&ht);
  free(user_input);
  return 0;
}

