//
// Created by burluiva on 10/28/2023.
//

#include "grammar_token.h"
#include "../../../model/token/token.h"

void grammar_token_init(GrammarToken *grammarToken) {
  assert(grammarToken);
  grammarToken->grammarRuleId = -1;
  grammarToken->grammarRule = NULL;
  grammarToken->reg = -1;
  grammarToken->returnType = UNDEFINED;
  grammarToken->isGlobal = false;
  grammarToken->tokensHolderSize = 0;
  for (int i = 0; i < MAX_GRAMMAR_TOKEN_TOKENS_HOLDER_SIZE; i++) {
    grammarToken->tokensHolder[i] = NULL;
  }
  grammarToken->returnType = UNDEFINED;
  grammarToken->isCastableType = false;
}

void grammar_token_add(GrammarToken *grammarToken, Token *token) {
  assert(grammarToken->tokensHolderSize < MAX_GRAMMAR_TOKEN_TOKENS_HOLDER_SIZE);
  grammarToken->tokensHolder[grammarToken->tokensHolderSize++] = token;
}

void grammar_token_free(GrammarToken *grammarToken) {
  assert(grammarToken);
  for (int i = 0; i < grammarToken->tokensHolderSize; i++) {
    if (grammarToken->tokensHolder[i]->type > NON_TERMINAL_UNDEFINED) {
      grammar_token_free(grammarToken->tokensHolder[i]->data.grammarToken);
    }
    free(grammarToken->tokensHolder[i]);
    grammarToken->tokensHolder[i] = NULL;
  }
  grammarToken->tokensHolderSize = 0;
  grammarToken->grammarRuleId = -1;
  grammarToken->grammarRule = NULL;
}

int grammar_token_cmp(GrammarToken *a, GrammarToken *b) {
  int tk_size_cmp = a->tokensHolderSize - b->tokensHolderSize;
  if (tk_size_cmp != 0)
    return tk_size_cmp;

  for (int i = 0; i < a->tokensHolderSize; i++) {
    Token *tokenA = a->tokensHolder[i];
    Token *tokenB = b->tokensHolder[i];
    int cmp = token_cmp(tokenA, tokenB);
    if (cmp !=0) return cmp;
  }
  return 0;
}