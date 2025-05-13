//
// Created by thekr on 11/22/2023.
//

#include "codegen_test.h"

int codegen_test() {
  Parser parser;
  parser_init(&parser);
  ifj_2023_parser_config(&parser);

  PParser *pParser = parser.expressionParser;
//  int operators[6] = {PLUS, MINUS, MULTIPLY, DIVIDE, F, DOLLAR};
//  char chOperators[6] = {'+', '-', '*', '/', 'f', '$'};
//  printf("  ");
//  for (int i = 0; i < 6; i++)
//    printf("%c ", chOperators[i]);
//  printf("\n");
//  for (int i = 0; i < 6; i++) {
//    printf("%c ", chOperators[i]);
//    for (int j = 0; j < 6; j++) {
//      char ch = 'x';
//      if (pParser->priorityTable[operators[i]][operators[j]] == 1)
//        ch = '<';
//      if (pParser->priorityTable[operators[i]][operators[j]] == -1)
//        ch = '>';
//      printf("%c ", ch);
//    }
//    printf("\n");
//  }

  TokenVector tokenVector;
  token_vector_init(&tokenVector);

  /**
   * while id >= id {
   *
   *    let id = (id+id)*(id/id-INTEGER*DOUBLE)
   *
   *    if let id { return INTEGER
   *    }
   *    if id == INTEGER { return id + id }
   *
   * }
   *
   *
   */
    token_vector_push_back(&tokenVector, fast_create_token(INTEGER));
    tokenVector.data[0].data.integer_value = 1;
    token_vector_push_back(&tokenVector, fast_create_token(PLUS));
    token_vector_push_back(&tokenVector, fast_create_token(INTEGER));
    tokenVector.data[2].data.integer_value = 2;

//  vector_push_back(&tokenVector, fast_create_token(LET));
//  vector_push_back(&tokenVector, fast_create_token(ID));
//  tokenVector.data[1].data.string = string_create("a");
//  vector_push_back(&tokenVector, fast_create_token(ASSIGN));
//  vector_push_back(&tokenVector, fast_create_token(INTEGER));
//  tokenVector.data[3].data.integer_value = 1;
//  vector_push_back(&tokenVector, fast_create_token(PLUS));
//  vector_push_back(&tokenVector, fast_create_token(INTEGER));
  tokenVector.data[5].data.integer_value = 2;

  GrammarToken grammarToken;
  grammar_token_init(&grammarToken);
  int offset = 0;
  Error err = parser_parse(&parser, &grammarToken, &tokenVector, &offset, STS,
                           LL_PARSER);
  assert(err.errorType == NONE);

  //code gen
  //err = generate_inter_code(&grammarToken);
  assert(err.errorType == NONE);
//  printToken(&grammarToken);
  return 0;
}