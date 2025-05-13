//
// Created by burluiva on 11/13/2023.
//

//#include "grammar_test.h"
//#include "../../model/grammar/grammar.h"
//#include "../../model/grammar/rule/grammar_rule.h"
//#include"../../utils/logger.h"
//
//int grammar_first_nullable_simple_test() {
//  int returnCode = 0;
//
//  // S -> ID
//  // S -> E F S
//  // E -> F
//  // E -> INTEGER
//  // F ->
//  // F -> STRING
//
//  GrammarRule grammarRule[] = {
//      grammar_rule_create(S, NULL, (TokenType[]){ID}, 1),
//      grammar_rule_create(S, NULL, (TokenType[]){E, F, S}, 3),
//      grammar_rule_create(E, NULL, (TokenType[]){F}, 1),
//      grammar_rule_create(E, NULL, (TokenType[]){INTEGER}, 1),
//      grammar_rule_create(F, NULL, (TokenType[]){}, 0),
//      grammar_rule_create(F, NULL, (TokenType[]){STRING}, 1)};
//  Grammar grammar;
//  grammar_init(&grammar);
//  grammar_configure(&grammar, grammarRule, 6);
//
//  printf("Symbol S is equal to %d\n", S);
//  printf("Symbol E is equal to %d\n", E);
//  printf("Symbol F is equal to %d\n", F);
//  printf("Symbol ID is equal to %d\n", ID);
//  printf("Symbol INTEGER is equal to %d\n", INTEGER);
//  printf("Symbol STRING is equal to %d\n", STRING);
//
//  for (int i = NON_TERMINAL_UNDEFINED; i < MAX_TOKEN_TYPES_NUMBER; i++) {
//    bool notFound = true;
//    for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
//      if (notFound && grammar.first[i][j]) {
//        printf("First for %d: ", i);
//        notFound = false;
//      }
//      if (grammar.first[i][j])
//        printf("%d, ", j);
//    }
//    if (!notFound)
//      printf("\n");
//  }
//
//  for (int i = NON_TERMINAL_UNDEFINED; i < MAX_TOKEN_TYPES_NUMBER; i++) {
//    if (grammar.nullable[i])
//      printf("Symbol %d is nullable!\n", i);
//  }
//
//  return returnCode;
//}
//
//int grammar_first_nullable_simple_test_2() {
//  int returnCode = 0;
//
//  // S -> F = E
//  // S -> E
//  // E -> F
//  // F -> ID
//  // F -> COLON E
//
//  GrammarRule grammarRule[] = {
//      grammar_rule_create(S, NULL, (TokenType[]){ID}, 3),
//      grammar_rule_create(S, NULL, (TokenType[]){E, F, S}, 3),
//      grammar_rule_create(F, NULL, (TokenType[]){}, 0),
//      grammar_rule_create(F, NULL, (TokenType[]){INTEGER}, 1),
//      grammar_rule_create(E, NULL, (TokenType[]){F}, 1),
//      grammar_rule_create(E, NULL, (TokenType[]){STRING}, 1)
//  };
//
//  Grammar grammar;
//  grammar_init(&grammar);
//  grammar_configure(&grammar, grammarRule, 6);
//
//  printf("Symbol S is equal to %d\n", S);
//  printf("Symbol E is equal to %d\n", E);
//  printf("Symbol F is equal to %d\n", F);
//  printf("Symbol ID is equal to %d\n", ID);
//  printf("Symbol STRING is equal to %d\n", STRING);
//  printf("Symbol INTEGER is equal to %d\n", INTEGER);
//
//  for (int i = NON_TERMINAL_UNDEFINED; i < MAX_TOKEN_TYPES_NUMBER; i++) {
//    bool notFound = true;
//    for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
//      if (notFound && grammar.first[i][j]) {
//        printf("First for %d: ", i);
//        notFound = false;
//      }
//      if (grammar.first[i][j])
//        printf("%d, ", j);
//    }
//    if (!notFound)
//      printf("\n");
//  }
//
//  for (int i = NON_TERMINAL_UNDEFINED; i < MAX_TOKEN_TYPES_NUMBER; i++) {
//    bool notFound = true;
//    for (int j = 0; j < MAX_TOKEN_TYPES_NUMBER; j++) {
//      if (notFound && grammar.follow[i][j]) {
//        printf("Follow for %d: ", i);
//        notFound = false;
//      }
//      if (grammar.follow[i][j])
//        printf("%d, ", j);
//    }
//    if (!notFound)
//      printf("\n");
//  }
//
//  for (int i = NON_TERMINAL_UNDEFINED; i < MAX_TOKEN_TYPES_NUMBER; i++) {
//    if (grammar.nullable[i])
//      printf("Symbol %d is nullable!\n", i);
//  }
//
//  return returnCode;
//}
//
//int grammar_test() {
//  LOG_INFO("First llGrammar test started!");
//  grammar_first_nullable_simple_test();
//  LOG_INFO("Second llGrammar test started!");
//  grammar_first_nullable_simple_test_2();
//  return 0;
//}
