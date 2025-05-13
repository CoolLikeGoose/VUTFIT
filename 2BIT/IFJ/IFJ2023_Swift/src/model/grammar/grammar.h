//
// Created by burluiva on 10/28/2023.
//

#ifndef IFJ2023_SWIFT_GRAMMAR_H
#define IFJ2023_SWIFT_GRAMMAR_H

#include "../../structures/vector/vector.h"
#include "../grammar/rule/grammar_rule.h"
#include "stdbool.h"
#define MAX_GRAMMAR_RULES_SIZE 128

typedef struct grammar_t {
  int rulesNumber;
  GrammarRule grammarRules[MAX_GRAMMAR_RULES_SIZE];
  bool first[MAX_TOKEN_TYPES_NUMBER][MAX_TOKEN_TYPES_NUMBER];
  bool follow[MAX_TOKEN_TYPES_NUMBER][MAX_TOKEN_TYPES_NUMBER];
  ///nullable[i] = true token with code i could be null
  bool nullable[MAX_TOKEN_TYPES_NUMBER];
  ///empty[i] = k means that in rule ith starting from kth token all tokens are nullable
  int empty[MAX_GRAMMAR_RULES_SIZE];
} Grammar;

void grammar_init(Grammar *grammar);
void grammar_configure(Grammar *grammar, GrammarRule *grammarRules, int numberOfRules);

///
/// \param grammar
/// \param grammarRule
/// \return -1 if no rule was found
int findGrammarRule(Grammar *grammar, GrammarRule* grammarRule);

/// Calculate the first of the array of tokens
/// \param grammar - grammar to use during calculation
/// \param types - the array of toke types
/// \param n - the length of token types array
/// \param first - to which array to write result
/// \return true - first array changed, false - first array does not have any changes
bool calculate_first(Grammar *grammar, const TokenType *types, int n,
                     bool first[MAX_TOKEN_TYPES_NUMBER]);

#endif // IFJ2023_SWIFT_GRAMMAR_H
