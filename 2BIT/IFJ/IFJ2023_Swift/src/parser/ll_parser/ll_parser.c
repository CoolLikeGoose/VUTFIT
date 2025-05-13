//
// Created by burluiva on 11/18/2023.
//

#include "ll_parser.h"

/// get by which rule we can get specified token
/// \param llParser parser
/// \param tokenToParse token to parse
/// \param inputToken if no token left parse UNDEFINED
/// \return the number of rule
int ll_parser_get_rule(LLParser *llParser, TokenType tokenToParse,
                       TokenType inputToken);

void ll_parser_init(LLParser *llParser) {
  llParser->llGrammar = NULL;
  for (int i = 0; i < MAX_TOKEN_TYPES_NUMBER; i++)
    for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
      llParser->llTable[i][j] = -1;
    }
}

void ll_parser_configure(LLParser *llParser) {
  assert(llParser && llParser->llGrammar);
  Grammar* llGrammar = llParser->llGrammar;
  bool first[MAX_TOKEN_TYPES_NUMBER];
  for (int i = 0; i < llGrammar->rulesNumber; i++) {
    GrammarRule *grammarRule = &(llGrammar->grammarRules[i]);
    memset(first, 0, sizeof(first));
    calculate_first(llGrammar, grammarRule->productions,
                    grammarRule->productionsNumber, first);
    if (llGrammar->empty[i] == 0) {
      // First(x) || Follow(x)
      for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
        first[j] |= llGrammar->follow[grammarRule->resultTokenType][j];
      }
    }

    for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
      if (first[j] == true) {
        assert(llParser->llTable[grammarRule->resultTokenType][j] == -1);
        llParser->llTable[grammarRule->resultTokenType][j] = i;
      }
    }
  }
}

/// get by which rule we can get specified token
/// \param llParser parser
/// \param tokenToParse token to parse
/// \param inputToken if no token left parse UNDEFINED
/// \return the number of rule
int ll_parser_get_rule(LLParser *llParser, TokenType tokenToParse,
                       TokenType inputToken) {
  if (inputToken == UNDEFINED &&
      llParser->llGrammar->nullable[tokenToParse] == true) {
    for (int i = 0; i < llParser->llGrammar->rulesNumber; i++) {
      if (llParser->llGrammar->grammarRules[i].resultTokenType ==
              tokenToParse &&
          llParser->llGrammar->grammarRules[i].productionsNumber == 0)
        return i;
    }
  }
  return llParser->llTable[tokenToParse][inputToken];
}

//Error ll_parser_eat(GrammarToken *grammarToken, TokenType tokenType,
//          TokenVector *tokenVector, int *curOffset) {
//  if (tokenType == UNDEFINED)
//    return error_create(NONE, NULL);
//
//  if (tokenVector->data[*curOffset].type == tokenType) {
//
//    Token *token = malloc(sizeof(Token));
//    if (token == NULL)
//      return error_create(FATAL, "Out of memory!");
//    token_init(token);
//
//    *token = tokenVector->data[*curOffset];
//    grammar_token_add(grammarToken, token);
//
//    *curOffset = *curOffset + 1;
//    return error_create(NONE, NULL);
//  }
//  return error_create(
//      PARSER_ERROR,
//      "Missing token!"); // TODO fix error message plus other fixes
//}
