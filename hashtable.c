#include "hashtable.h"

void hashtable_init(HashTable *ht) {
  ht->count = 0;
  ht->capacity = 0;
  ht->entries = NULL;
}

// TODO free entry key and value char*s
void hashtable_free(HashTable *ht) {
  if (ht == NULL) return;
  if (ht->entries != NULL)
    free(ht->entries);
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

bool hashtable_put(HashTable *ht, char *key, char *value) {
  if (hashtable_get_load_factor(ht) >= HT_MAX_LOAD_FACTOR) {
    hashtable_adjust_capacity(ht, ht->capacity * 2);
  }

  Entry *entry = find_entry(ht->entries, ht->capacity, key);
  bool is_new_key = entry->key == NULL;
  if (is_new_key) {
    ht->count++;
  }

  entry->key = calloc(sizeof(char), HT_MAX_KEY_LEN);
  strncpy(entry->key, key, HT_MAX_KEY_LEN);
  entry->value = calloc(sizeof(char), HT_MAX_VAL_LEN);
  strncpy(entry->value, value, HT_MAX_VAL_LEN);

  return is_new_key;
}

// TODO make it so that key is trimmed to HT_MAX_KEY_LEN before
// being hashed; otherwise the hash might not match the key that was
// actually inserted into the table
Entry *find_entry(Entry *entries, int capacity, char *key) {
  uint32_t index = strhash(key) % capacity;
  for (;;) { // FIXME maybe add condition that it must not iterate
             // more than capacity times? just to be safe in something
             // breaks somewhere else
    Entry *entry = &(entries[index]);
    if (entry->key == key || entry->key == NULL)
      return entry;
    index = (index + 1) % capacity;
  }
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

