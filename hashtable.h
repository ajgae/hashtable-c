#ifndef _hashtable_h
#define _hashtable_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HASHTABLE_MAX_LOAD_FACTOR 0.75

typedef struct {
  char *key;
  char *value;
} Entry;

typedef struct {
  int capacity;
  int count;
  Entry *entries;
} HashTable;

void hashtable_init(HashTable *ht);
void hashtable_free(HashTable *ht);
bool hashtable_put(HashTable *ht, char *key, char *value);
Entry *find_entry(Entry *entries, int capacity, char *key);
void hashtable_adjust_capacity(HashTable *ht, int new_capacity);
float hashtable_get_load_factor(HashTable *ht);
uint32_t strhash(char *s);

// _hashtable_h
#endif
