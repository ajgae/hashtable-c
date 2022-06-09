#include "hashtable.h"

void hashtable_init(HashTable *ht) {
  ht->count = 0;
  ht->capacity = 0;
  ht->entries = NULL;
}

void hashtable_free(HashTable *ht) {
  if (ht == NULL) return;
  if (ht->entries != NULL) {
    for (int i = 0; i < ht->capacity; ++i) {
      Entry entry = ht->entries[i];
      if (entry.key != NULL) {
        free(entry.key);
        entry.key = NULL;
        free(entry.value);
        entry.value = NULL;
      }
    }
    free(ht->entries);
    ht->entries = NULL;
  }
}

void hashtable_adjust_capacity(HashTable *ht, int new_capacity) {
  Entry *new_entries = calloc(sizeof(Entry), new_capacity);

  // iterate over old entries; ignore NULL-keyed old entries; find new
  // entry corresponding to each old entry; assign correspondingly
  for (int i = 0; i < ht->capacity; ++i) {
    Entry *entry = &(ht->entries[i]);
    if (entry->key == NULL)
      continue;

    Entry *new_entry = find_entry(new_entries, new_capacity, entry->key);
    new_entry->key = entry->key;
    new_entry->value = entry->value;
  }

  ht->entries = new_entries;
  ht->capacity = new_capacity;
}

// Add the `key`-`value` pair to `ht`.
bool hashtable_put(HashTable *ht, char *key, char *value) {
  if (hashtable_get_load_factor(ht) >= HT_MAX_LOAD_FACTOR) {
    hashtable_adjust_capacity(ht, ht->capacity * 2);
  }

  // trim the key (and value bc why not) before calling find_entry so
  // that the hash is computed on the trimmed key; otherwise, if `key`
  // is longer than 127 characters, the hash value used for inserting
  // the key-value pair won't match the actual key being inserted
  // (which is trimmed)
  char *key_trimmed = calloc(sizeof(char), HT_MAX_KEY_LEN);
  strncpy(key_trimmed, key, HT_MAX_KEY_LEN);
  char *value_trimmed = calloc(sizeof(char), HT_MAX_VAL_LEN);
  strncpy(value_trimmed, value, HT_MAX_VAL_LEN);

  Entry *entry = find_entry(ht->entries, ht->capacity, key_trimmed);
  bool is_new_key = entry->key == NULL;
  if (is_new_key) {
    ht->count++;
  }

  entry->key = key_trimmed;
  entry->value = value_trimmed;

  return is_new_key;
}

// Remove the key-value pair given by `key` in `ht`.
bool hashtable_remove(HashTable *ht, char *key) {
  // TODO
}

Entry *find_entry(Entry *entries, int capacity, char *key) {
  uint32_t index = strhash(key) % capacity;
  for (int i = 0; i < capacity; ++i) {
    Entry *entry = &(entries[index]);
    if (entry->key == key || entry->key == NULL)
      return entry;
    index = (index + 1) % capacity;
  }
  fprintf(stderr, "Error: could not find entry after iterating over all entries, aborting.");
  exit(1);
}

float hashtable_get_load_factor(HashTable *ht) {
  if (ht->capacity == 0)
    return -1;
  return (float)ht->count / (float)ht->capacity;
}

uint32_t strhash(char *s) {
  uint32_t hash = 2166136261u;
  for (; *s != '\0'; ++s) {
    hash ^= (uint8_t)*s;
    hash *= 16777619;
  }
  return hash;
}

