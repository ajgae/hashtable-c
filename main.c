#include "main.h"
#include "hashtable.h"
#include "prompt.h"

int main(int argc, char **argv) {
  printf("Hello! Welcome to the hashtable prompt program.\n");
  HashTable ht;
  hashtable_init(&ht);
  hashtable_adjust_capacity(&ht, 4);
  char *user_input;

#ifdef _USE_READLINE
  init_readline();
#else
  // must allocate manually since readline doesn't do it for us
  user_input = malloc(sizeof(char) * MAX_INPUT_LENGTH);
#endif
  
  for (;;) {
    prompt(user_input);

    if (user_input == NULL) {
      continue;
    }
    char *token = strtok(user_input, INPUT_DELIMITERS);
    if (token == NULL) {
      continue;
    }

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
                "Error: put command requires key and value arguments\n");
      } else {
        printf("Inserting key-value pair: %s => %s\n", key, value);
        hashtable_put(&ht, key, value);
      }
    }

    else if (!strncmp("remove", token, MAX_INPUT_LENGTH)) {
      char *key = strtok(NULL, INPUT_DELIMITERS);
      if (key == NULL) {
        fprintf(stderr, "Error: remove command requires key argument\n");
      } else {
        bool success = hashtable_remove(&ht, key);
        if (success) {
          printf("Successfully removed entry.\n");
        } else {
          printf("Could not remove entry. Maybe an entry with key '%s' does "
                 "not exist?\n",
                 key);
        }
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
  // must free either way, if readline allocated it or if we did
  free(user_input);
  return 0;
}

