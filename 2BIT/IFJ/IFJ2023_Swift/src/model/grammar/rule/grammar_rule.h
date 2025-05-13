//
// Created by burluiva on 10/28/2023.
//

#ifndef IFJ2023_SWIFT_GRAMMAR_RULE_H
#define IFJ2023_SWIFT_GRAMMAR_RULE_H

#include "../../../codegen/AddressTable/address_table.h"
#include "../../../parser/sem_analys/sem_analys.h"
#include "../../error/error.h"
#include "../../token/token.h"
#include <stdbool.h>

#define MAX_GRAMMAR_RULE_PRODUCTIONS_SIZE 128

struct grammar_token_t;

//TODO add symtable
typedef void (*AssemblerFn)(struct grammar_token_t *grToken, AddressTable *addressTable);

struct sem_analyser_t;
typedef void (*SemanticAnalysisFn)(struct grammar_token_t* grammarToken,
                                  struct sem_analyser_t* addressTable);

typedef struct grammar_rule_t {
  /// non-terminal result token type like S,E,T,F
  TokenType resultTokenType;
  ///applies semantic check on rules
  SemanticAnalysisFn semanticAnalysisFn;
  /// function for post order assembler code gen
  AssemblerFn assemblerFnPostOrd;
  /// function for pre order assembler code gen
  AssemblerFn assemblerFnPreOrd;
  /// number of productions in grammar rule
  int productionsNumber;
  /// token types needed to build the specified \p resultTokenType
  TokenType productions[MAX_GRAMMAR_RULE_PRODUCTIONS_SIZE];
} GrammarRule;

void grammar_rule_init(GrammarRule *grammarRule);

GrammarRule grammar_rule_create(TokenType resultTokenType,
                                SemanticAnalysisFn sematicAnalysisFn,
                                AssemblerFn assemblerFnPostOrder,
                                AssemblerFn assemblerFnPreOrder,
                                const TokenType *tokenProductions,
                                int productionsNumber);

#endif // IFJ2023_SWIFT_GRAMMAR_RULE_H
