

#include "codegen.h"

Error generate_inter_code(GrammarToken *grToken) {
  AddressTable *addressTable = malloc(sizeof(AddressTable));
  address_table_init(addressTable);

  InterCodeInit();

  //INIT global context
  grToken->reg = get_reg_new(addressTable);
  grToken->isGlobal = true;
  int stackLevel = 0;
  enumerate_grammar_tokens(grToken, "GF", &addressTable->resRegisters);

  post_order_process(grToken, addressTable);
  InterCodeEnd();

  return error_create(NONE, "none");
}

void post_order_process(GrammarToken *grToken, AddressTable *addressTable) {
  if (grToken == NULL)
    return;

  if (grToken->grammarRule->assemblerFnPreOrd != NULL)
    grToken->grammarRule->assemblerFnPreOrd(grToken, addressTable);

  for (int i = 0; i < grToken->tokensHolderSize; i++) {
    Token *token = grToken->tokensHolder[i];

    // compute all non terminals
    if (token->type > NON_TERMINAL_UNDEFINED) {
      post_order_process(token->data.grammarToken, addressTable);
    }
  }
  if (grToken->grammarRule->assemblerFnPostOrd != NULL)
    grToken->grammarRule->assemblerFnPostOrd(grToken, addressTable);
}

void enumerate_grammar_tokens(GrammarToken *grammarToken, char* variablePrefix,
                              int *gen) {
  if (grammarToken == NULL || variablePrefix == NULL|| gen == NULL)
    return;
  //TODO global context is not fully loaded...
  //some func detecting
  if (grammarToken->grammarRule->resultTokenType == D && grammarToken->grammarRule->productions[0] == FUNC)
    return;

  int n = grammarToken->tokensHolderSize;
  for (int i = 0; i < n; i++) {
    Token *token = grammarToken->tokensHolder[i];
    if (token->type >= NON_TERMINAL_UNDEFINED) {

      token->data.grammarToken->reg = *gen;
      token->data.grammarToken->isGlobal = strcmp("LF", variablePrefix)==0 ? 0 : 1;
//      printf("DEFVAR %s@r%d\n", token->data.grammarToken->isGlobal ? "GF" : "LF",
//             token->data.grammarToken->reg);
      printf("DEFVAR %s@r%d\n",variablePrefix, token->data.grammarToken->reg);

      *gen = *gen + 1;
//      if (token->type == STS)
//        *stackLevel = *stackLevel + 1;
      enumerate_grammar_tokens(token->data.grammarToken, variablePrefix, gen);
//      if (token->type == STS)
//        *stackLevel = *stackLevel - 1;
    }
  }
}