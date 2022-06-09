#include "hashtable.h"

// Initialize the hashtable. It must then be given some initial
// capacity using `hashtable_adjust_capacity`. Failing to do so will
// lead to undefined behaviour.
void hashtable_init(HashTable *ht) {
  ht->count = 0;
  ht->capacity = 0;
  ht->entries = NULL;
}

// Free the memory internally allocated by the hashtable functions.
// This function must be called before freeing the struct itself.
// Does nothing and returns immediately if `ht == NULL`.
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

// Adjust the capacity of `ht` to `new_capacity`. Does nothing and
// returns immediately if `new_capacity` is lower than or equal to the
// current capacity, or if `ht == NULL`. This function must be called
// after `hashtable_init` on a new hashtable struct to actually give
// it a capacity, before calling any other functions on the struct.
// Otherwise, its capacity is 0 and behaviour is undefined.
void hashtable_adjust_capacity(HashTable *ht, int new_capacity) {
  if (ht == NULL || new_capacity <= ht->capacity) return;
  
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

// Add the given `key`-`value` pair to `ht`.
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

// Remove the key-value pair given by `key` in `ht`. Returns `true` if
// an entry with such a `key` existed and was removed succesfully, or
// `false` otherwise or if `ht == NULL || key == NULL`.
bool hashtable_remove(HashTable *ht, char *key) {
  if (ht == NULL || key == NULL) {
    return false;
  }
  Entry *entry = find_entry(ht->entries, ht->capacity, key);
  if (entry->key == NULL) {
    return false;
  } else {
    free(entry->key);
    entry->key = NULL;
    free(entry->value);
    entry->value = NULL;
    ht->count -= 1;
    return true;
  }
}

// Find the entry with key `key` in an array of `entries` of length
// `capacity`, using linear probing. Returns `NULL` if either of
// `entries` or `key` is NULL, or if `capacity <= 0`.
Entry *find_entry(Entry *entries, int capacity, char *key) {
  if (entries == NULL || capacity <= 0 || key == NULL) {
    return NULL;
  }

  uint32_t index = strhash(key) % capacity;

  for (int i = 0; i < capacity; ++i) {
    Entry *entry = &(entries[index]);
    if (entry->key == NULL || !strncmp(entry->key, key, HT_MAX_KEY_LEN)) {
      return entry;
    }
    index = (index + 1) % capacity;
  }

  // should never happen
  fprintf(stderr, "Error: could not find entry after iterating over all "
                  "entries. Aborting.");
  exit(1);
}

// Return the load factor of `ht`, that is, count / capacity.
float hashtable_get_load_factor(HashTable *ht) {
  // FIXME should a hashtable with 0 capacity be considered full?
  if (ht->capacity == 0) {
    return 1;
  }
  return (float)ht->count / (float)ht->capacity;
}

// Return the 32-bit FNV-1a hash of the given `s`tring.
uint32_t strhash(char *str) {
  uint32_t hash = 2166136261u;
  for (; *str != '\0'; ++str) {
    hash ^= (uint8_t)*str;
    hash *= 16777619;
  }
  return hash;
}

