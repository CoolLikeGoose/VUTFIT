//
// Created by burluiva on 10/10/2023.
//

#include "hashmap.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>
#include "../../utils/logger.h"

/// inspiration
/// https://opensource.apple.com/source/Git/Git-98/src/git/hashmap.c.auto.html

#define FNV32_BASE ((unsigned int)0x811c9dc5)
#define FNV32_PRIME ((unsigned int)0x01000193)

#define HASHMAP_INITIAL_SIZE 64

#define HASHMAP_RESIZE_BITS 1

#define HASHMAP_LOAD_FACTOR 80

void alloc_table(HashMap *map, unsigned int size) {
  assert(map);
  map->tableSize = size;
  map->table = calloc(size, sizeof(struct hashmap_entry *));
  map->growAt = (unsigned int)(size * HASHMAP_LOAD_FACTOR / 100);
}

int entry_equals(const HashMap *map,const HashMapEntry *e1, const HashMapEntry *e2) {
  assert(map);
  return (e1 == e2) || (e1->hash == e2->hash && !map->cmpFn(e1, e2));
}

unsigned int get_bucket_id(const HashMap *map, const HashMapEntry *entry) {
  assert(map && entry);
  return map->tableSize == 0 ? 0 : map->tableSize - 1 & entry->hash;
}

void rehash(HashMap *map, unsigned int newSize) {
  unsigned int i, oldSize = map->tableSize;
  HashMapEntry **oldTable = map->table;

  alloc_table(map, newSize);
  for (i = 0; i < oldSize; i++) {
    HashMapEntry *ePtr = oldTable[i];
    while (ePtr != NULL) {
      HashMapEntry *next = ePtr->next;
      unsigned int bId = map->tableSize - 1 && ePtr->hash;
      ePtr->next = map->table[bId];
      map->table[bId] = ePtr;
      ePtr = next;
    }
  }

  free(oldTable);
  oldTable = NULL;
}

/// find entry_prt in table context
HashMapEntry **find_entry_ptr(const HashMap *map, const HashMapEntry *key) {
  struct hashmap_entry **e = &map->table[get_bucket_id(map, key)];
  while (*e && !entry_equals(map, *e, key))
    e = &(*e)->next;
  return e;
}

void hashmap_init(HashMap *map, hashmap_cmp_fn equals_function) {
  assert(map && equals_function);

  map->table = NULL;
  map->tableSize = 0;
  map->growAt = 0;
  map->size = 0;

  map->cmpFn = equals_function;

  alloc_table(map, HASHMAP_INITIAL_SIZE);
}

void hashmap_free(HashMap *map) {
  if (!map || !map->table)
    return;
  free(map->table);
  memset(map, 0, sizeof(*map));
}

void hashmap_entry_init(void *entry, unsigned int hash) {
  struct hashmap_entry *e = entry;
  e->hash = hash;
  e->next = NULL;
}

void *hashmap_get(const HashMap *map, const void *key) {
  return *find_entry_ptr(map, key);
}

void *hashmap_get_bucket(const HashMap *map, const void *key) {
  struct hashmap_entry **e = &map->table[get_bucket_id(map, key)];
  return *e;
}

void hashmap_add(HashMap *map, void *entry) {
  unsigned int b = get_bucket_id(map, entry);

  ((HashMapEntry *)entry)->next = map->table[b];
  map->table[b] = entry;

  map->size++;
  if (map->size > map->growAt)
    rehash(map, map->tableSize << HASHMAP_RESIZE_BITS);
}

void *hashmap_remove(HashMap *map, const void *key) {
  struct hashmap_entry *old;
  struct hashmap_entry **e = find_entry_ptr(map, key);

  if (*e == NULL)
    return NULL;

  old = *e;
  *e = old->next;
  old->next = NULL;

  map->size--;
  return old;
}

void *hashmap_put(struct hashmap *map, void *entry) {
  struct hashmap_entry *old = hashmap_remove(map, entry);
  hashmap_add(map, entry);
  return old;
}

void hashmap_iter_init(struct hashmap *map, struct hashmap_iter *iter) {
  iter->map = map;
  iter->tablePos = 0;
  iter->next = NULL;
}

void *hashmap_iter_next(struct hashmap_iter *iter) {
  struct hashmap_entry *curPtr = iter->next;
  while (1) {
    if (curPtr != NULL) {
      iter->next = curPtr->next;
      return curPtr;
    }

    if (iter->tablePos >= iter->map->tableSize)
      return NULL;

    curPtr = iter->map->table[iter->tablePos++];
  }
}