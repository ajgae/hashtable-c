#include "hashtable.h"

void hashtable_init(HashTable *ht) {
  ht->count = 0;
  ht->capacity = 0;
  ht->entries = NULL;
}

void hashtable_free(HashTable *ht) {
  // TODO
}

bool hashtable_put(HashTable *ht, char *key, char *value) {
  if (hashtable_get_load_factor(ht) >= HASHTABLE_MAX_LOAD_FACTOR) {
    hashtable_adjust_capacity(ht, ht->capacity * 2);
  }
  Entry *entry = find_entry(ht->entries, ht->capacity, key);
  bool is_new_key = entry->key == NULL;
  if (is_new_key)
    ht->count++;

  entry->key = key;
  entry->value = value;
  return is_new_key;
}

Entry *find_entry(Entry *entries, int capacity, char *key) {
  uint32_t index = strhash(key) % capacity;
  for (;;) {
    Entry *entry = &(entries[index]);
    if (entry->key == key || entry->key == NULL)
      return entry;
    index = (index + 1) % capacity;
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

int main(int argc, char **argv) {
  HashTable ht;
  hashtable_init(&ht);
  hashtable_adjust_capacity(&ht, 4);
  hashtable_put(&ht, "an apple a day", "keeps the doctor away");
  hashtable_put(&ht, "hello", "world");
  return 0;
}
