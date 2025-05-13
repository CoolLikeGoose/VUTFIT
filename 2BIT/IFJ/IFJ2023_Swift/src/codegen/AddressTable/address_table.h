//
// Created by thekr on 11/21/2023.
//

#ifndef IFJ2023_SWIFT_ADDRESS_TABLE_H
#define IFJ2023_SWIFT_ADDRESS_TABLE_H

#include "../../structures/hashmap/hashmap.h"
#include "../../structures/string/string.h"
#include "../../structures/vector/vector.h"
#include <stdbool.h>


typedef struct address_table_t {
  // vars
//  HashMap *hashMap;
  HashMapPointer2DVector *variablesContainer;
  int resRegisters;
  int tempLabelRegisters;

  // cycles
  int resCycles;
  IntVector* curCycle;

  // functions
  int args_added;
  StringPointerVector *functionsStack;
} AddressTable;

typedef struct var_address_t {
  struct hashmap_entry entry; // hash holder
  String *key;                // key of entry
  int value;                  // value of entry
} VarAddress;

unsigned int strhash(const char *str);
void address_table_init(AddressTable *addressTable);
int cmp_fn(const VarAddress *entryA, const VarAddress *entryB);

void create_hashmap_inVector(AddressTable *addressTable);
void delete_hashmap_inVector(AddressTable *addressTable);

//------------VARIABLES------------
//get new register for use
int get_reg_new(AddressTable *addressTable);
//get last created register
int get_reg_cur(AddressTable *addressTable);

//create new ID (returns new register)
int AT_create(AddressTable *addressTable, String *var, bool *isGlobal);
//create new ID with existing register
int AT_create_withReg(AddressTable *addressTable, String *var, int reg, bool *isGlobal);
//Put new register to ID (returns 1 if successful otherwise returns 0)
int AT_put(AddressTable *addressTable, String *var, int reg);
//get ID from table (Returns -1 if not found)
int AT_get(AddressTable *addressTable, String *var, bool* isGlobal);

//------------WHILE------------
int get_cur_cycle(AddressTable *addressTable);
int init_cycle(AddressTable *addressTable);
int end_cycle(AddressTable *addressTable);

//------------FUNCTION------------
String *get_cur_function(AddressTable *addressTable);
void init_function(AddressTable *addressTable, String *funcName);
void end_function(AddressTable *addressTable);

void add_arg(AddressTable *addressTable);
int get_args(AddressTable *addressTable);

int get_temp_label(AddressTable *addressTable);
int get_temp_label_cur(AddressTable *addressTable);

#endif // IFJ2023_SWIFT_ADDRESS_TABLE_H
