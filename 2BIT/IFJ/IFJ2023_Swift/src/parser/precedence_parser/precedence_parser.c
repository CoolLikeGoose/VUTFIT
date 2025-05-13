//
// Created by burluiva on 11/18/2023.
//

#include "precedence_parser.h"

void pparser_item_init(PParserItem *item) {
  assert(item);
  item->token = NULL;
  item->opened = 0;
  item->closed = 0;
}

TokenType precedence_operator_find_id_representative(Grammar *grammar) {
  assert(grammar);
  TokenType idRepresentative = UNDEFINED;
  for (int i = 0; i < grammar->rulesNumber; i++) {
    GrammarRule *grammarRule = &(grammar->grammarRules[i]);

    if (grammarRule->productionsNumber == 1) {
      assert(idRepresentative == UNDEFINED &&
             "Should be only one representative");
      idRepresentative = grammarRule->productions[0];
    }
  }
  return idRepresentative;
}

void precedence_parser_init(PParser *pParser) {
  assert(pParser);
  for (int i = 0; i < MAX_TOKEN_TYPES_NUMBER; i++) {
    for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
      pParser->priorityTable[i][j] = 0;
    }
    pParser->isOperator[i] = false;
  }
  pParser->pGrammar = NULL;
}

void precedence_parser_configure(
    PParser *parser, Grammar *pGrammar,
    const int operatorPriority[MAX_TOKEN_TYPES_NUMBER],
    const int operatorAssociation[MAX_TOKEN_TYPES_NUMBER]) {

  assert(parser && pGrammar);
  assert(pGrammar->rulesNumber > 0);

  parser->resultTokenType = pGrammar->grammarRules[0].resultTokenType;

  parser->pGrammar = pGrammar;

  TokenType idRepresentative =
      precedence_operator_find_id_representative(pGrammar);

  assert(idRepresentative != UNDEFINED && "Could not find id representative");
  parser->idRepresentative = idRepresentative;

  for (int i = 0; i < pGrammar->rulesNumber; i++) {
    GrammarRule *grammarRule = &(pGrammar->grammarRules[i]);
    assert(parser->resultTokenType == grammarRule->resultTokenType);

    // resolving operator and id order
    for (int j = 0; j < grammarRule->productionsNumber; j++) {

      if (grammarRule->productions[j] > NON_TERMINAL_UNDEFINED) {
        if (j == 0)
          parser->priorityTable[idRepresentative][DOLLAR] = -1;
        if (j == grammarRule->productionsNumber - 1)
          parser->priorityTable[DOLLAR][idRepresentative] = 1;
        continue; // we need only operators
      }

      TokenType operator= grammarRule->productions[j];
      parser->isOperator[operator] = true;
      assert(operatorPriority[operator] != -1 &&
      "Priority of operator is not set...");

      parser->priorityTable[operator][idRepresentative] =
        (j < grammarRule->productionsNumber -1 && grammarRule->productions[j+1] > NON_TERMINAL_UNDEFINED) ? 1 :
          parser->priorityTable[operator][idRepresentative];
      parser->priorityTable[idRepresentative][operator] =
        (j > 0 && grammarRule->productions[j-1] > NON_TERMINAL_UNDEFINED) ? -1 :
          parser->priorityTable[idRepresentative][operator];

      assert(parser->priorityTable[operator][DOLLAR] == 0);
      assert(parser->priorityTable[DOLLAR][operator] == 0);
      parser->priorityTable[operator][DOLLAR] = -1;
      parser->priorityTable[DOLLAR][operator] = 1;

      if (j == 0) {
        parser->priorityTable[operator][DOLLAR] = 0;
      }
      if (j == pGrammar->rulesNumber - 1) {
        parser->priorityTable[DOLLAR][operator] = 0;
      }
    }
  }

  for (int i = 0; i < MAX_TOKEN_TYPES_NUMBER; i++)
    for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
      if (!(parser->isOperator[i]) || !(parser->isOperator[j]))
        continue;
      if (operatorPriority[i] == operatorPriority[j]) {
        if (operatorAssociation[i] == 1 && operatorAssociation[j] == 1) {
          parser->priorityTable[i][j] = 1;
        }
        if (operatorAssociation[i] == -1 && operatorAssociation[j] == -1) {
          parser->priorityTable[i][j] = -1;
        }
      } else {
        parser->priorityTable[i][j] =
            operatorPriority[i] > operatorPriority[j] ? 1 : -1;
      }
    }
}

bool precedence_parser_could_process(PParser *parser, TokenType type) {
  return parser->isOperator[type] || parser->idRepresentative == type ||
      type == DOLLAR;
}