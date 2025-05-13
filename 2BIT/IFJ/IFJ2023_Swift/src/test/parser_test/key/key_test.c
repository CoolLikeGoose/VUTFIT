//#include "../../../parser/config/key/config_parser_key.h"
//#include "../../../parser/parser.h"
//#include "../../../utils/logger.h"
//
//int parser_key_test() {
//  // Grammar to be tested
//
//  // S -> (E)
//  // S -> STRING
//  // E -> S
//  // E -> E, S
//  int returnCode = 0;
//  GrammarRule grammarRule[] = {
//      grammar_rule_create(S, S_SEQUENTIAL,
//                          (TokenType[]){LEFT_BRACKET, E, RIGHT_BRACKET}, 3),
//      grammar_rule_create(S, S_SEQUENTIAL, (TokenType[]){STRING}, 1),
//      grammar_rule_create(E, E_TERMINAL, (TokenType[]){S}, 1),
//      grammar_rule_create(E, E_TERMINAL, (TokenType[]){E, COMMA, S}, 3),
//  };
//  Grammar grammar = grammar_configure(grammarRule, 4);
//
//  ParserKey parserKey;
//  parser_key_init(&parserKey);
//  bool used[MAX_GRAMMAR_RULES_SIZE][MAX_GRAMMAR_RULE_PRODUCTIONS_SIZE + 1] = {
//      false};
//
//  parser_key_add_rules_from_grammar(&parserKey, used, S, &grammar);
//
//  LOG_INFO_WHITE("Start asserting the state of parser key...");
//
//  if (parserKey.n != 2) {
//    LOG_ERROR("Expected to have 2 keys, but got %d", parserKey.n);
//    returnCode = 1;
//  }
//  LOG_INFO("First test iteration finished!");
//  for (int i = 0; i < parserKey.n; i++) {
//    LOG_INFO_WHITE("Rules which was found in key point: %d grammarRuleId: %d",
//                   parserKey.subKeys[i].point,
//                   parserKey.subKeys[i].grammarRuleId);
//  }
//
//  memset(used, false, sizeof(used));
//
//  parser_key_init(&parserKey);
//  parser_key_add_rules_from_grammar(&parserKey, used, E, &grammar);
//
//  LOG_INFO_WHITE("Start asserting the state of parser key...");
//
//  if (parserKey.n != 4) {
//    LOG_ERROR("Expected to have 4 keys, but got %d", parserKey.n);
//    returnCode = 1;
//  }
//  LOG_INFO("Second test iteration finished!");
//  for (int i = 0; i < parserKey.n; i++) {
//    LOG_INFO_WHITE("Rules which was found in key point: %d grammarRuleId: %d",
//                   parserKey.subKeys[i].point,
//                   parserKey.subKeys[i].grammarRuleId);
//  }
//
//  return returnCode;
//}