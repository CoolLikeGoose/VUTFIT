//
// Created by burluiva on 11/18/2023.
//

#ifndef IFJ2023_SWIFT_LL_PARSER_H
#define IFJ2023_SWIFT_LL_PARSER_H

#include "../../model/grammar/grammar.h"

typedef struct ll_parser_t {
  Grammar *llGrammar;
  int llTable[MAX_TOKEN_TYPES_NUMBER][MAX_TOKEN_TYPES_NUMBER];
} LLParser;

void ll_parser_init(LLParser *llParser);
void ll_parser_configure(LLParser *llParser);

/// get by which rule we can get specified token
/// \param llParser parser
/// \param tokenToParse token to parse
/// \param inputToken if no token left parse UNDEFINED
/// \return the number of rule
int ll_parser_get_rule(LLParser *llParser, TokenType tokenToParse,
                       TokenType inputToken);


#endif // IFJ2023_SWIFT_LL_PARSER_H
