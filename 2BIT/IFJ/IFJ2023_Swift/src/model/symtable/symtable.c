//
// Created by burluiva on 11/30/2023.
//

#include "symtable.h"
#include "../../utils/logger.h"
#include "../token/token.h"
#include "limits.h"


void symtable_value_init(SymtableValue* value) {
  value->returnType = UNDEFINED;
  value->isConstant = false;
  value->isInitialised = false;
}


#define FNV32_BASE ((unsigned int)0x811c9dc5)
#define FNV32_PRIME ((unsigned int)0x01000193)

//FENVICK hashing
unsigned int symtable_strhash(const char *str)
{
  unsigned int c, hash = FNV32_BASE;
  while ((c = (unsigned char)*str++)) {
    hash = (hash * FNV32_PRIME) ^ c;
    if (c == '$') break;
  }
  return hash;
}

Error symtable_entry_create(SymtableEntry **entry, char *key,
                            SymtableValue *value) {
  *entry = malloc(sizeof(SymtableEntry));
  if (*entry == NULL)
    return error_create(FATAL, "Out of memory!");

  hashmap_entry_init(&((*entry)->entry), symtable_strhash(key));

  int keyLen = strlen(key);
  (*entry)->key  = malloc(sizeof(char) * (keyLen + 1));
  if ((*entry)->key==NULL)
    return error_create(FATAL, "Out of memory");
  strcpy((*entry)->key, key);
  (*entry)->key[keyLen] = '\0';


  (*entry)->value = *value;

  return error_create(NONE, NULL);
}

int cmp_symtable_entry(SymtableEntry *entryA, SymtableEntry *entryB) {
  if (entryA == entryB) return 0;
  if (entryA == NULL) return -1;
  if (entryB == NULL) return 1;

  int lenA = strlen(entryA->key);
  int lenB = strlen(entryB->key);

  char* strA = entryA->key;
  char* strB = entryB->key;

  if (lenA != lenB) return 1;

  for (int i=0;i<=lenA;i++) {
    if (strA[i]!=strB[i]) {
      if (strA[i] == NIL_TYPE && isNullableType(strB[i])) continue;
      if (strB[i] == NIL_TYPE && isNullableType(strA[i])) continue;
    }
  }

  return 0;
}

Error symtable_init(SymTable *symTable) {
  hashmap_vector_init(symTable);
  Error err = symtable_push_frame(symTable); // push global frame
  if (err.errorType != NONE)
    return err;
  return error_create(NONE, NULL);
}

Error symtable_push_frame(SymTable *symTable) {
  HashMap *frame = malloc(sizeof(HashMap));
  if (frame == NULL)
    return error_create(FATAL, "Out of memory!");
  hashmap_init(frame,
               (hashmap_cmp_fn)cmp_symtable_entry); // add cmp and hash function
  hashmap_vector_push_back(symTable, frame);
  return error_create(NONE, NULL);
}

void symtable_pop_frame(SymTable *symTable) {
  if (symTable->length >= 1) {
    hashmap_free(hashmap_vector_at(symTable, symTable->length - 1));
    hashmap_vector_pop(symTable);
  } else {
    LOG_WARN("SymTable stack was empty could not pop_frame...");
  }
}

/// add declaration to last stack frame of our symtable
/// \param symTable - symbolic table
/// \param entry - entry must be created using symtable_entry_create function
/// \param override - whether to override previous declaration (symbol)
/// \return
Error symtable_add(SymTable *symTable, SymtableEntry *entry) {
  assert(symTable && entry);
  if (symTable->length == 0) {
    return error_create(FATAL, "No frame was found during semantic analysis!");//TODO add better error
  }
  HashMap* hashMap = hashmap_vector_at(symTable, symTable->length - 1);
  SymtableEntry *previous = hashmap_put(hashMap, entry);
  if (previous != NULL) {
    fprintf(stderr, "Redefinition of id[%s] was caught", entry->key);
    exit(3);
  }
  return error_create(NONE, NULL);
}
SymtableEntry *symtable_get_by_key(SymTable *symTable, char *key) {
  assert(symTable && key);
  if (symTable->length == 0)
    return NULL;

  SymtableEntry searchEntry;
  //symtable_entry_init(&searchEntry);

  int keyLen = (int) strlen(key);
  searchEntry.key  = malloc(sizeof(char) * (keyLen + 1));
  if (searchEntry.key == NULL) return NULL;
  strcpy(searchEntry.key, key);
  searchEntry.key[keyLen] = '\0';

  hashmap_entry_init(&searchEntry.entry, symtable_strhash(key));
  for (int i=(int) symTable->length-1;i>=0;i--) {
    HashMap* hashMap = hashmap_vector_at(symTable, i);
    SymtableEntry* found = (SymtableEntry*) hashmap_get(hashMap, &searchEntry);
    if (found != NULL)
      return found;
  }
  return NULL;
}

/// Converts key to hash and look for the bucket
/// \param symTable
/// \param key
/// \return
SymtableEntry *symtable_get_by_hash(SymTable*symTable, char *key) {
  assert(symTable && key);
  if (symTable->length == 0)
    return NULL;

  SymtableEntry searchEntry;
  //symtable_entry_init(&searchEntry);

  int keyLen = (int) strlen(key);
  searchEntry.key  = malloc(sizeof(char) * (keyLen + 1));
  if (searchEntry.key == NULL) return NULL;
  strcpy(searchEntry.key, key);
  searchEntry.key[keyLen] = '\0';

  hashmap_entry_init(&searchEntry.entry, symtable_strhash(key));
  for (int i=(int) symTable->length-1;i>=0;i--) {
    HashMap* hashMap = hashmap_vector_at(symTable, i);
    SymtableEntry* found = (SymtableEntry*) hashmap_get_bucket(hashMap, &searchEntry);
    if (found != NULL)
      return found;
  }
  return NULL;
}

void symtable_free(SymTable *symTable) {
  for (int i=0;i<symTable->length;i++) {
    //TODO add strings free (using iteration over hashmap)
    hashmap_free(hashmap_vector_at(symTable, i));
  }
  hashmap_vector_free(symTable);
}


