//
// Created by burluiva on 10/28/2023.
//

#include "grammar.h"
#include "limits.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void init_nullable(Grammar *grammar);
void init_empty(Grammar *grammar);
void init_first(Grammar *grammar);
void init_forward(Grammar *grammar);

void grammar_init(Grammar *grammar) {
  assert(grammar);
  grammar->rulesNumber = 0;
  for (int i = 0; i < MAX_GRAMMAR_RULES_SIZE; i++) {
    grammar_rule_init(&grammar->grammarRules[i]);
    grammar->empty[i] = INT_MAX;
  }

  for (int i = 0; i < MAX_TOKEN_TYPES_NUMBER; i++) {
    for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
      grammar->first[i][j] = false;
      grammar->follow[i][j] = false;
    }
    grammar->nullable[i] = false;
  }
}

void grammar_configure(Grammar *grammar, GrammarRule *grammarRules, int numberOfRules) {
  assert(grammarRules);
  assert(numberOfRules >= 0 && numberOfRules <= MAX_GRAMMAR_RULES_SIZE);
  grammar->rulesNumber = numberOfRules;
  for (int i = 0; i < numberOfRules; i++) {
    grammar->grammarRules[i] = grammarRules[i];
  }
  init_nullable(grammar);
  init_empty(grammar);
  init_first(grammar);
  init_forward(grammar);
}

bool calculate_first(Grammar *grammar, const TokenType *types, int n,
                     bool first[MAX_TOKEN_TYPES_NUMBER]) {
  assert(grammar && types);
  bool changed = false;
  for (int i = 0; i < n; i++) {
    if (i == 0 || grammar->nullable[types[i - 1]] == true) {
      for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
        changed = (first[j] == false && grammar->first[types[i]][j] == true);
        first[j] |= grammar->first[types[i]][j];
      }
    } else break;
  }
  return changed;
}

void init_nullable(Grammar *grammar) {
  assert(grammar);
  bool changed = true;
  while (changed) {
    changed = false;
    for (int i = 0; i < grammar->rulesNumber; i++) {
      int tokenType = grammar->grammarRules[i].resultTokenType;
      if (grammar->nullable[tokenType] == 1)
        continue;

      int tmp = 1;
      for (int j = 0; j < grammar->grammarRules[i].productionsNumber; j++) {
        tmp &= grammar->nullable[grammar->grammarRules[i].productions[j]];
      }
      grammar->nullable[tokenType] = tmp;

      if (grammar->nullable[tokenType] == 1)
        changed = true;
    }
  }
}

void init_empty(Grammar *grammar) {
  for (int i = 0; i < grammar->rulesNumber; i++) {
    GrammarRule *rule = &grammar->grammarRules[i];
    grammar->empty[i] = rule->productionsNumber;
    for (int j = rule->productionsNumber - 1; j >= 0; j--) {
      if (grammar->nullable[rule->productions[j]])
        grammar->empty[i] = j;
      else
        break;
    }
  }
}


void init_first(Grammar *grammar) {
  assert(grammar);
  bool changed = true;
  for (int i = UNDEFINED; i < NON_TERMINAL_UNDEFINED; i++) {
    grammar->first[i][i] = true;
  }
  while (changed) {
    changed = false;
    for (int i = 0; i < grammar->rulesNumber; i++) {
      int sourceTokenType = grammar->grammarRules[i].resultTokenType;
      for (int j = 0; j < grammar->grammarRules[i].productionsNumber; j++) {
        int destTokenType = grammar->grammarRules[i].productions[j];
        if (j != 0 &&
            !grammar->nullable[grammar->grammarRules[i].productions[j - 1]])
          break;
        for (int k = 0; k < MAX_TOKEN_TYPES_NUMBER; k++) {
          changed |= grammar->first[sourceTokenType][k] == false &&
                     grammar->first[destTokenType][k] == true;
          grammar->first[sourceTokenType][k] |=
              grammar->first[destTokenType][k];
        }
      }
    }
  }
}

void init_forward(Grammar *grammar) {
  assert(grammar);

  bool changed = true;
  for (int ii=1;ii<=100;ii++) {
    changed = false;
    for (int i = 0; i < grammar->rulesNumber; i++) {
      GrammarRule *rule = &grammar->grammarRules[i];
      for (int j = 0; j < rule->productionsNumber - 1; j++) {
        changed |= calculate_first(grammar, (rule->productions + j + 1),
                                   rule->productionsNumber - j - 1,
                                   grammar->follow[rule->productions[j]]);
      }

      for (int j = MIN(grammar->empty[i], rule->productionsNumber-1); j < rule->productionsNumber; j++) {
        for (int k = 0; k < MAX_TOKEN_TYPES_NUMBER; k++) {
          changed |= (grammar->follow[rule->productions[j]][k] == false &&
                      grammar->follow[rule->resultTokenType][k] == true);
          grammar->follow[rule->productions[j]][k] |=
              grammar->follow[rule->resultTokenType][k];
        }
      }

    }
  }
}


int findGrammarRule(Grammar *grammar, GrammarRule* grammarRule) {
  for (int i=0;i<grammar->rulesNumber;i++) {
    GrammarRule * rule = &(grammar->grammarRules[i]);
    bool found = (grammarRule->resultTokenType == rule->resultTokenType) &&
                 (grammarRule->productionsNumber == rule->productionsNumber);

    for (int j=0;found && j<rule->productionsNumber;j++) {
      found &= (grammarRule->productions[j]==rule->productions[j]);
    }
    if (found) return i;
  }
  return -1;
}