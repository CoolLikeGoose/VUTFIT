////
//// Created by thekr on 11/20/2023.
////
//
//#ifndef IFJ2023_SWIFT_CODEGEN_H
//#define IFJ2023_SWIFT_CODEGEN_H
//
//#include "inter_code/inter_code.h"
//#include "../model/grammar/grammar.h"
//#include "AddressTAble/address_table.h"
//#include "../model/grammar/rule/grammar_rule.h"
//#include "../model/grammar/token/grammar_token.h"
//
//Error generate_inter_code(GrammarToken *token);
//void post_order_process(GrammarToken *token, AddressTable *addressTable);
//
//void factor_process(GrammarToken *token, AddressTable *addressTable);
//void expression_process(GrammarToken *token, AddressTable *addressTable);
//void declaration_process(GrammarToken *token, AddressTable *addressTable);
//void statement_process(GrammarToken*, AddressTable*);
//void token_preprocess(Token *token, AddressTable *addressTable);
//
//#endif // IFJ2023_SWIFT_CODEGEN_H

#ifndef IFJ2023_SWIFT_CODEGEN_H
#define IFJ2023_SWIFT_CODEGEN_H

#include "inter_code/inter_code.h"
#include "../model/grammar/grammar.h"
#include "AddressTable/address_table.h"
#include "../model/grammar/rule/grammar_rule.h"
#include "../model/grammar/token/grammar_token.h"

Error generate_inter_code(GrammarToken *token);
void post_order_process(GrammarToken *token, AddressTable *addressTable);
void enumerate_grammar_tokens(GrammarToken *grammarToken, char* variablePrefix,  int* gen);

#endif // IFJ2023_SWIFT_CODEGEN_H
