//
// Created by burluiva on 10/28/2023.
//

#include "grammar_rule.h"

void grammar_rule_init(GrammarRule *grammarRule) {
  assert(grammarRule);
  grammarRule->resultTokenType = UNDEFINED;
  grammarRule->assemblerFnPreOrd = NULL;
  grammarRule->assemblerFnPostOrd = NULL;
  grammarRule->semanticAnalysisFn = NULL;
  grammarRule->productionsNumber = 0;

  for (int i = 0; i < MAX_GRAMMAR_RULE_PRODUCTIONS_SIZE; i++) {
    grammarRule->productions[i] = UNDEFINED;
  }
}

GrammarRule grammar_rule_create(TokenType resultTokenType,
                                SemanticAnalysisFn sematicAnalysisFn,
                    AssemblerFn assemblerFnPostOrder,
                    AssemblerFn assemblerFnPreOrder,
                    const TokenType *tokenProductions, int productionsNumber) {
  assert(productionsNumber >= 0 &&
         productionsNumber <= MAX_GRAMMAR_RULE_PRODUCTIONS_SIZE);

  GrammarRule grammarRule;
  grammar_rule_init(&grammarRule);

  grammarRule.resultTokenType = resultTokenType;
  grammarRule.assemblerFnPostOrd = assemblerFnPostOrder;
  grammarRule.assemblerFnPreOrd = assemblerFnPreOrder;
  grammarRule.semanticAnalysisFn = sematicAnalysisFn;
  grammarRule.productionsNumber = productionsNumber;

  for (int i = 0; i < productionsNumber; i++) {
    grammarRule.productions[i] = tokenProductions[i];
  }

  return grammarRule;
}