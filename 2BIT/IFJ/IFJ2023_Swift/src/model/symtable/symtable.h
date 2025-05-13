//
// Created by burluiva on 11/30/2023.
//

#ifndef IFJ2023_SWIFT_SYMTABLE_H
#define IFJ2023_SWIFT_SYMTABLE_H

#include "../../structures/hashmap/hashmap.h"
#include "../../structures/string/string.h"
#include "../token/token.h"

//struct grammar_token_t;
typedef struct sym_value_t {
  TokenType returnType;
  bool isConstant;
  bool isInitialised;
//  GrammarToken* declaration;
} SymtableValue;

void symtable_value_init(SymtableValue* value);

typedef struct symtable_entry_t {
  HashMapEntry entry;
  char* key;
  SymtableValue value;
} SymtableEntry;

Error symtable_entry_create(SymtableEntry **entry, char *key, SymtableValue *value);


typedef HashMapPointerVector SymTable;

Error symtable_init(SymTable* symTable);

/// adds entry (throws error on duplicates)
/// \param symTable - the symtable itself
/// \param entry - pointer to the entry (pointer should not on the stuck)
/// \return NULL - could not put, problem
Error symtable_add(SymTable* symTable, SymtableEntry* entry);
SymtableEntry*symtable_get_by_key(SymTable * symTable, char* key);
SymtableEntry *symtable_get_by_hash(SymTable*symTable, char* key);

Error symtable_push_frame(SymTable* symTable);
void symtable_pop_frame(SymTable* symTable);

void symtable_free(SymTable* symTable);

#endif // IFJ2023_SWIFT_SYMTABLE_H
