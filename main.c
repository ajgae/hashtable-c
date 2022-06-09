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

    if (!strncmp("exit", token, MAX_INPUT_LENGTH) ||
        !strncmp("quit", token, MAX_INPUT_LENGTH) ||
        !strncmp("q", token, MAX_INPUT_LENGTH)) {
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
          printf("  %d: %s => %s\n", i, entry.key, entry.value);
        }
      }
    }

    else if (!strncmp("hash", token, MAX_INPUT_LENGTH)) {
      char *str = strtok(NULL, INPUT_DELIMITERS);
      if (str == NULL) {
        fprintf(stderr, "Error: string argument required for hash command\n");
      } else {
        uint32_t hash = strhash(str);
        char *mod_str = strtok(NULL, INPUT_DELIMITERS);
        if (mod_str == NULL) {
          printf("Hash value: 0x%x\n", hash);
        } else {
          char *endptr;
          int mod = strtol(mod_str, &endptr, 10);
          if (*endptr != '\0' || mod <= 0) {
            fprintf(stderr, "Error: the second argument to the hash command "
                            "must be a valid positive non-zero integer.\n");
          } else {
            uint32_t hash_mod = hash % mod;
            printf("Hash value mod %d: %d\n", mod, hash_mod);
          }
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

