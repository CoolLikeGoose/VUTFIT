//
// Created by burluiva on 10/28/2023.
//

#include "config_parser.h"
#include "../../codegen/inter_code/inter_code.h"
#include "../../structures/hashmap/hashmap.h"
#include"../sem_analys/sem_analys.h"

Error ifj_2023_parser_config(Parser *parser) {
  assert(parser);
  // TODO memory free control
  parser->llParser = malloc(sizeof(LLParser));
  if (parser->llParser == NULL) {
    return error_create(FATAL, "Out of memory!");
  }
  ll_parser_init(parser->llParser);
  parser->expressionParser = malloc(sizeof(PParser));
  if (parser->expressionParser == NULL) {
    return error_create(FATAL, "Out of memory!");
  }
  precedence_parser_init(parser->expressionParser);
  parser->llParser->llGrammar = malloc(sizeof(Grammar));
  if (parser->llParser->llGrammar == NULL) {
    return error_create(FATAL, "Out of memory!");
  }
  grammar_init(parser->llParser->llGrammar);
  parser->expressionParser->pGrammar = malloc(sizeof(Grammar));
  if (parser->expressionParser->pGrammar == NULL) {
    return error_create(FATAL, "Out of memory!");
  }
  grammar_init(parser->expressionParser->pGrammar);

  assert(parser->expressionParser && parser->llParser &&
         "Set the ll parser and precedence parser");

  int llGrammarRulesCount = 61;
  GrammarRule llGrammarRules[] = {
      grammar_rule_create(STS, NULL, NULL, NULL, (TokenType[]){S, STS_TMP}, 2),
      grammar_rule_create(STS_TMP, NULL, NULL, NULL,
                          (TokenType[]){DELIMITER, S, STS_TMP}, 3),
      grammar_rule_create(STS_TMP, NULL, NULL, NULL, (TokenType[]){}, 0),

      grammar_rule_create(S, NULL, NULL, NULL, (TokenType[]){}, 0),
      grammar_rule_create(S, NULL, BlockWhileInterCode, WhileInitInterCode, (TokenType[]){WHILE, COND, BLOCK}, 3),
      grammar_rule_create(S, NULL, PostOrderForIf, PreOrderForIf,
                          (TokenType[]){IF, COND, BLOCK, IF_ELSE}, 4),
      grammar_rule_create(S, NULL, NULL, NULL, (TokenType[]){E}, 1),
      grammar_rule_create(S, NULL, IdAssignInterCode, NULL, (TokenType[]){ID_AND_ASSIGN, E}, 2),
      grammar_rule_create(S, NULL, NULL, NULL, (TokenType[]){D}, 1),
      grammar_rule_create(S, NULL, ReturnInterCode, NULL, (TokenType[]){RETURN, E_NULLABLE}, 2),
      grammar_rule_create(S, NULL, ContinueInterCode, NULL, (TokenType[]){CONTINUE}, 1),
      grammar_rule_create(S, NULL, BreakInterCode, NULL, (TokenType[]){BREAK}, 1),
      grammar_rule_create(COND, NULL, CondInterCode, NULL, (TokenType[]){E}, 1),
      grammar_rule_create(COND, NULL, UnwrapCond, NULL, (TokenType[]){LET, ID}, 2),   //TODO: Create definition for this
      grammar_rule_create(IF_ELSE, NULL, NULL, PreOrderForIfEslse, (TokenType[]){}, 0),
      grammar_rule_create(IF_ELSE, NULL, NULL, PreOrderForIfEslse, (TokenType[]){ELSE, BLOCK}, 2),
      grammar_rule_create(
          BLOCK, NULL, NULL, NULL,
          (TokenType[]){LEFT_CURL_BRACKET, STS, RIGHT_CURL_BRACKET}, 3),

      grammar_rule_create(D, NULL, NULL, NULL,
                          (TokenType[]){VAR, ID_INIT}, 2),
      grammar_rule_create(D, NULL, NULL, NULL,
                          (TokenType[]){LET, ID_INIT}, 2),
      grammar_rule_create(ID_INIT, NULL, VarTypedIdInit, NULL,
                          (TokenType[]){ID_AND_COLON, TYPE, ID_INIT_ASSIGN}, 3),
      grammar_rule_create(ID_INIT, NULL, VarIdInit, NULL,
                          (TokenType[]){ID_AND_ASSIGN, E}, 2),

      grammar_rule_create(ID_INIT_ASSIGN, NULL, IdInitAssignPropagate, NULL,
                          (TokenType[]){ASSIGN, E}, 2),
      grammar_rule_create(ID_INIT_ASSIGN, NULL, NULL, NULL,
                          (TokenType[]){}, 0),

      grammar_rule_create(D, NULL, FuncInitializeEscape, FuncInitialize,
                          (TokenType[]){FUNC, ID, LEFT_BRACKET, PARAMS,
                                        RIGHT_BRACKET, FUNC_RETURN, BLOCK},
                          7),

      grammar_rule_create(FUNC_RETURN, NULL, NULL, NULL,
                          (TokenType[]){ARROW, TYPE}, 2),
      grammar_rule_create(FUNC_RETURN, NULL, NULL, NULL,
                          (TokenType[]){},0),

      grammar_rule_create(PARAM, NULL, FuncInitParams, NULL, (TokenType[]){FUNC_ID, ID_AND_COLON, TYPE},
                          3),
      grammar_rule_create(FUNC_ID, NULL, NULL, NULL, (TokenType[]){ID}, 1),
      grammar_rule_create(FUNC_ID, NULL, NULL, NULL, (TokenType[]){UNDERSCORE}, 1),
      grammar_rule_create(PARAMS, NULL, FuncDivideScope, NULL, (TokenType[]){PARAM, PARAMS_TMP},
                          2),
      grammar_rule_create(PARAMS_TMP, NULL, NULL, NULL, (TokenType[]){}, 0),
      grammar_rule_create(PARAMS_TMP, NULL, NULL, NULL,
                          (TokenType[]){COMMA, PARAM, PARAMS_TMP}, 3),
      grammar_rule_create(PARAMS, NULL, NULL, NULL, (TokenType[]){}, 0),
      grammar_rule_create(TANN, NULL, NULL, NULL, (TokenType[]){COLON, TYPE}, 2),
      grammar_rule_create(F, sem_analyse_factor_brackets_move, StoreBrackets, NULL,
                          (TokenType[]){LEFT_BRACKET, E, RIGHT_BRACKET}, 3),
      grammar_rule_create(F, NULL, FuncCall, NULL, (TokenType[]){ID, F_CALL}, 2),
      grammar_rule_create(F, sem_analyse_factor_store_value, StoreInt, NULL, (TokenType[]){INTEGER}, 1),
      grammar_rule_create(F, sem_analyse_factor_store_value, StoreString, NULL, (TokenType[]){STRING}, 1),
      grammar_rule_create(F, sem_analyse_factor_store_value, StoreDouble, NULL, (TokenType[]){DOUBLE}, 1),
      grammar_rule_create(F, sem_analyse_factor_store_value, StoreBool, NULL, (TokenType[]){BOOLEAN}, 1),
      grammar_rule_create(F, sem_analyse_factor_store_value, StoreNil, NULL, (TokenType[]){NIL}, 1),

      grammar_rule_create(F, sem_analyse_expression_negation, NotInterCode, NULL, (TokenType[]){HARD_UNWRAP, F}, 2),
      grammar_rule_create(F_CALL, NULL, FuncArgAdd, NULL,
                          (TokenType[]){LEFT_BRACKET, ARGS, RIGHT_BRACKET}, 3),
      grammar_rule_create(F_CALL, NULL, NULL , NULL, (TokenType[]){}, 0),
      grammar_rule_create(ARGS, NULL, PushArg_ARGS, NULL, (TokenType[]){ARG, ARGS_TMP}, 2),
      grammar_rule_create(ARGS, NULL, NULL, NULL, (TokenType[]){}, 0),
      grammar_rule_create(ARGS_TMP, NULL, NULL, NULL, (TokenType[]){}, 0),
      grammar_rule_create(ARGS_TMP, NULL, PushArg_TMP, NULL,
                          (TokenType[]){COMMA, ARG, ARGS_TMP}, 3),

      grammar_rule_create(ARG, NULL, ADDArgLabeled, NULL, (TokenType[]){ID_AND_COLON, E}, 2),
      grammar_rule_create(ARG, NULL, ADDArg, NULL, (TokenType[]){E}, 1),
      grammar_rule_create(E_NULLABLE, NULL, NULL, NULL, (TokenType[]){E}, 1),
      grammar_rule_create(E_NULLABLE, NULL, NULL, NULL, (TokenType[]){}, 0),

      grammar_rule_create(TYPE, NULL, NULL, NULL, (TokenType[]){INT_TYPE}, 1),
      grammar_rule_create(TYPE, NULL, NULL, NULL, (TokenType[]){INT_NULLABLE_TYPE},
                          1),
      grammar_rule_create(TYPE, NULL, NULL, NULL, (TokenType[]){STRING}, 1),
      grammar_rule_create(TYPE, NULL, NULL, NULL, (TokenType[]){STRING_NULLABLE_TYPE},
                          1),
      grammar_rule_create(TYPE, NULL, NULL, NULL, (TokenType[]){DOUBLE_TYPE}, 1),
      grammar_rule_create(TYPE, NULL, NULL, NULL, (TokenType[]){DOUBLE_NULLABLE_TYPE},
                          1),
      grammar_rule_create(TYPE, NULL, NULL, NULL, (TokenType[]){BOOLEAN_TYPE}, 1),
      grammar_rule_create(TYPE, NULL, NULL, NULL, (TokenType[]){BOOLEAN_NULLABLE_TYPE}, 1),

      grammar_rule_create(E, sem_analyse_factor_move, GetF, NULL, (TokenType[]){F}, 1),
  };

  grammar_configure(parser->llParser->llGrammar, llGrammarRules,
                    llGrammarRulesCount);

  ll_parser_configure(parser->llParser);
  // TODO add boolean operations
  int pGrammarRulesCount = 15;
  GrammarRule pGrammarRules[] = {
      grammar_rule_create(E, sem_analyse_expression_binary_operators, SumInterCode, NULL, (TokenType[]){E, PLUS, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, SubInterCode, NULL, (TokenType[]){E, MINUS, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, MulInterCode, NULL, (TokenType[]){E, MULTIPLY, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, DivInterCode, NULL, (TokenType[]){E, DIVIDE, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_equal_operators, EqualInterCode, NULL, (TokenType[]){E, EQUAL, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_equal_operators, NotEqualInterCode, NULL, (TokenType[]){E, NOT_EQUAL, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, GreaterInterCode, NULL, (TokenType[]){E, GREATER, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, LessInterCode, NULL, (TokenType[]){E, LESS, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, GreaterEqualInterCode, NULL, (TokenType[]){E, GREATER_EQUAL, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, LessEqualInterCode, NULL, (TokenType[]){E, LESS_EQUAL, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_soft_unwrap, SoftUnwrapInterCode, NULL, (TokenType[]){E, SOFT_UNWRAP, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, OrInterCode, NULL, (TokenType[]){E, LOGICAL_OR, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_binary_operators, AndInterCode, NULL, (TokenType[]){E, LOGICAL_AND, E}, 3),
      grammar_rule_create(E, sem_analyse_expression_hard_unwrap, HardUnwrapInterCode, NULL, (TokenType[]){E, HARD_UNWRAP}, 2),
      grammar_rule_create(E, sem_analyse_factor_move, GetF, NULL, (TokenType[]){F}, 1),
  };

  grammar_configure(parser->expressionParser->pGrammar, pGrammarRules,
                    pGrammarRulesCount);
  int operatorPriority[MAX_TOKEN_TYPES_NUMBER] = {
      [HARD_UNWRAP] = 1,
      [MULTIPLY] = 2,   [DIVIDE] = 2,      [MINUS] = 3,
      [PLUS] = 3,       [EQUAL] = 4,       [NOT_EQUAL] = 4,
      [GREATER] = 4,    [LESS] = 4,        [GREATER_EQUAL] = 4,
      [LESS_EQUAL] = 4, [SOFT_UNWRAP] = 5, [LOGICAL_OR] = 6,
      [LOGICAL_AND] = 6};
  int operatorAssociation[MAX_TOKEN_TYPES_NUMBER] = {
      [HARD_UNWRAP] = -1,
      [MULTIPLY] = -1,   [DIVIDE] = -1,     [MINUS] = -1,
      [PLUS] = -1,       [EQUAL] = 0,       [NOT_EQUAL] = 0,
      [GREATER] = 0,     [LESS] = 0,        [GREATER_EQUAL] = 0,
      [LESS_EQUAL] = 0,  [SOFT_UNWRAP] = 1, [LOGICAL_OR] = -1,
      [LOGICAL_AND] = -1};
  precedence_parser_configure(parser->expressionParser,
                              parser->expressionParser->pGrammar,
                              operatorPriority, operatorAssociation);
  return error_create(NONE, NULL);
}
