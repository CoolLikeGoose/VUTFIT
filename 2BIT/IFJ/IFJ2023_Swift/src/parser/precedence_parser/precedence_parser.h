//
// Created by burluiva on 11/18/2023.
//

#ifndef IFJ2023_SWIFT_PRECEDENCE_PARSER_H
#define IFJ2023_SWIFT_PRECEDENCE_PARSER_H

#include "../../model/grammar/grammar.h"
#include "../../model/token/token.h"
#include "../ll_parser/ll_parser.h"
#include "../../structures/vector/vector.h"

typedef struct pparser_item_t {
  bool closed, opened;
  Token *token;
} PParserItem;

vector_def(PParserItem, PParserItem, pparser_item_);

void pparser_item_init(PParserItem *item);

typedef struct p_parser_t {
  /// 0 - no rule, throw error; 1 - opened; -1 - closed;
  int priorityTable[MAX_TOKEN_TYPES_NUMBER][MAX_TOKEN_TYPES_NUMBER];
  /// define whether current symbol is operator
  bool isOperator[MAX_TOKEN_TYPES_NUMBER];
  TokenType idRepresentative;
  TokenType resultTokenType;
  /// should contain only binary operations
  Grammar *pGrammar;
} PParser;

void precedence_parser_init(PParser *pParser);
void precedence_parser_configure(
    PParser *parser, Grammar *pGrammar,
    const int operatorPriority[MAX_TOKEN_TYPES_NUMBER],
    const int operatorAssociation[MAX_TOKEN_TYPES_NUMBER]);
bool precedence_parser_could_process(PParser *parser, TokenType type);

#endif // IFJ2023_SWIFT_PRECEDENCE_PARSER_H
