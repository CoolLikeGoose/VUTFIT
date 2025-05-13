//
// Created by thekr on 11/21/2023.
//

#include "address_table.h"
#include "../../utils/logger.h"

#define FNV32_BASE ((unsigned int)0x811c9dc5)
#define FNV32_PRIME ((unsigned int)0x01000193)

//------------SYSTEM------------
unsigned int strhash(const char *str)
{
  unsigned int c, hash = FNV32_BASE;
  while ((c = (unsigned char)*str++))
    hash = (hash * FNV32_PRIME) ^ c;
  return hash;
}
void address_table_init(AddressTable *addressTable) {
  addressTable->args_added = 0;
  addressTable->resRegisters = 0;
  addressTable->resCycles = 0;

  addressTable->variablesContainer = malloc(sizeof (HashMapPointer2DVector));
  assert(addressTable->variablesContainer);
  addressTable->curCycle = malloc(sizeof (IntVector));
  assert(addressTable->curCycle);
  addressTable->functionsStack = malloc(sizeof (StringPointerVector));
  assert(addressTable->functionsStack);

  //init vectors
  hasmap2d_vector_init(addressTable->variablesContainer);
  HashMapPointerVector *hmPointer = malloc(sizeof (HashMapPointerVector));
  hashmap_vector_init(hmPointer);
  hasmap2d_vector_push_back(addressTable->variablesContainer, hmPointer);
//  hashmap_vector_init(addressTable->variables);
  int_vector_init(addressTable->curCycle);
  string_pointer_vector_init(addressTable->functionsStack);

  //Init first variable
  create_hashmap_inVector(addressTable);
}
int cmp_fn(const VarAddress *entryA, const VarAddress *entryB) {
  return strcmp(entryA->key->data, entryB->key->data);
}

void create_hashmap_inVector(AddressTable *addressTable) {
  HashMap *hashMap = malloc(sizeof(HashMap));
  hashmap_init(hashMap, (hashmap_cmp_fn)cmp_fn);

  int curFunc = (int)addressTable->variablesContainer->length-1;
  hashmap_vector_push_back(addressTable->variablesContainer->data[curFunc], hashMap);
}

void delete_hashmap_inVector(AddressTable *addressTable) {
  int curFunc = (int)addressTable->variablesContainer->length-1;
  HashMap *hashMap = hashmap_vector_pop(addressTable->variablesContainer->data[curFunc]);
  hashmap_free(hashMap);
}

//------------VARIABLES------------
//Temp reg
int get_reg_new(AddressTable *addressTable) {
  return addressTable->resRegisters++;
}

int get_reg_cur(AddressTable *addressTable) {
  return addressTable->resRegisters-1;
}

int AT_create(AddressTable *addressTable, String *var, bool *isGlobal) {
  int reg = get_reg_new(addressTable);
  reg = AT_create_withReg(addressTable, var, reg, isGlobal);
  return reg;
}

int AT_create_withReg(AddressTable *addressTable, String *var, int reg, bool *isGlobal) {
//  (*isGlobal) = addressTable->variables->length == 1 ? true : false;
  (*isGlobal) = true;  //TODO: bruh

  VarAddress *newEntry = malloc(sizeof(VarAddress));
  hashmap_entry_init(newEntry, strhash(var->data));
  newEntry->key = var;
  newEntry->value = reg;

  int curFunc = (int)addressTable->variablesContainer->length-1;
  HashMap *cur = addressTable->variablesContainer->data[curFunc]->data[addressTable->variablesContainer->data[curFunc]->length-1];
  VarAddress* oldEntry = hashmap_put(cur, newEntry); //IT RETURNS  OLD ENTITY

  //return newEntry != NULL ? newEntry->value : -1;
  return oldEntry == NULL ? newEntry->value : -1;
}

int AT_get(AddressTable *addressTable, String *var, bool* isGlobal) {
  *isGlobal = true;
  //search for current function
  int curFuncInd = (int)addressTable->variablesContainer->length-1;
  HashMapPointerVector *curFunc = addressTable->variablesContainer->data[curFuncInd];

  for (int i = (int)curFunc->length-1; i >= 0; i--) {
    HashMap *cur = hashmap_vector_at(curFunc, i);
    VarAddress entryToFind;
    hashmap_entry_init(&entryToFind, strhash(var->data));
    entryToFind.key = var;

    VarAddress *varAddress = hashmap_get(cur, &entryToFind);

    if (varAddress != NULL) {
      *isGlobal = (curFuncInd == 0);
      return varAddress->value;
    }
  }

  if (curFuncInd == 0)    //we are not in function and still var wasn't found
    return -1;

  //now search in global vars
  curFunc = addressTable->variablesContainer->data[0];

  for (int i = (int)curFunc->length-1; i >= 0; i--) {
    HashMap *cur = hashmap_vector_at(curFunc, i);
    VarAddress entryToFind;
    hashmap_entry_init(&entryToFind, strhash(var->data));
    entryToFind.key = var;

    VarAddress *varAddress = hashmap_get(cur, &entryToFind);

    if (varAddress != NULL) {
//      *isGlobal = true;
      return varAddress->value;
    }
  }

  return -1;  //not found
}

//int AT_put(AddressTable *addressTable, String *var, int reg) {
//  VarAddress *varAddress = hashmap_get(addressTable->hashMap, var);
//  varAddress = hashmap_put(addressTable->hashMap, varAddress);
//  return varAddress != NULL ? 1 : 0;
//}


//------------WHILE------------
int init_cycle(AddressTable *addressTable) {
  int cur_while = addressTable->resCycles++;
  int_vector_push_back(addressTable->curCycle, cur_while);
  create_hashmap_inVector(addressTable);
  return cur_while;
}

int end_cycle(AddressTable *addressTable) {
  int cur_while = get_cur_cycle(addressTable);
  int_vector_pop(addressTable->curCycle);
  delete_hashmap_inVector(addressTable);
  return cur_while;
}

int get_cur_cycle(AddressTable *addressTable) {
  return int_vector_at(addressTable->curCycle, addressTable->curCycle->length - 1);
}

//------------FUNCTION------------
String *get_cur_function(AddressTable *addressTable) {
  return string_pointer_vector_at(addressTable->functionsStack, addressTable->functionsStack->length-1);
}

void init_function(AddressTable *addressTable, String *funcName) {
  string_pointer_vector_push_back(addressTable->functionsStack, funcName);  //push curr func name

  HashMapPointerVector *hmPointer = malloc(sizeof (HashMapPointerVector));  //create new fn global field of vars
  hashmap_vector_init(hmPointer);
  hasmap2d_vector_push_back(addressTable->variablesContainer, hmPointer);
  create_hashmap_inVector(addressTable);                                    //init first local field
}

void end_function(AddressTable *addressTable) {
  string_pointer_vector_pop(addressTable->functionsStack);

  int curFunc = (int)addressTable->variablesContainer->length-1;
  while (addressTable->variablesContainer->data[curFunc]->length != 0) {    //delete all local fields
    delete_hashmap_inVector(addressTable);
  }
  hashmap_vector_free(addressTable->variablesContainer->data[curFunc]);
  hasmap2d_vector_pop(addressTable->variablesContainer);                    //delete fn global field
}

void add_arg(AddressTable *addressTable) {
  addressTable->args_added++;
}

//Be careful this function zeroes the args_added!!!
int get_args(AddressTable *addressTable) {
  int args = addressTable->args_added;
  addressTable->args_added = 0;
  return args;
}

int get_temp_label(AddressTable *addressTable) { return addressTable->tempLabelRegisters++; }
int get_temp_label_cur(AddressTable *addressTable) { return addressTable->tempLabelRegisters-1; }
