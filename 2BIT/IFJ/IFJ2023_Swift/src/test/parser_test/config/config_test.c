//
// Created by burluiva on 11/13/2023.
//

#include "config_test.h"

Token fast_create_token(TokenType type) {
  Token token;
  token.type = type;
  return token;
}

int parser_config_test() {
  Parser parser;
  parser_init(&parser);
  ifj_2023_parser_config(&parser);

  PParser *pParser = parser.expressionParser;
  int operators[6] = {PLUS, MINUS, MULTIPLY, DIVIDE, F, DOLLAR};
  char chOperators[6] = {'+', '-', '*', '/', 'f', '$'};
  printf("  ");
  for (int i = 0; i < 6; i++)
    printf("%c ", chOperators[i]);
  printf("\n");
  for (int i = 0; i < 6; i++) {
    printf("%c ", chOperators[i]);
    for (int j = 0; j < 6; j++) {
      char ch = 'x';
      if (pParser->priorityTable[operators[i]][operators[j]] == 1)
        ch = '<';
      if (pParser->priorityTable[operators[i]][operators[j]] == -1)
        ch = '>';
      printf("%c ", ch);
    }
    printf("\n");
  }

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
  token_vector_push_back(&tokenVector, fast_create_token(WHILE));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(GREATER_EQUAL));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(LEFT_CURL_BRACKET));

  token_vector_push_back(&tokenVector, fast_create_token(LET));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(ASSIGN));
  token_vector_push_back(&tokenVector, fast_create_token(LEFT_BRACKET));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(PLUS));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(RIGHT_BRACKET));
  token_vector_push_back(&tokenVector, fast_create_token(MULTIPLY));
  token_vector_push_back(&tokenVector, fast_create_token(LEFT_BRACKET));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(DIVIDE));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(MINUS));
  token_vector_push_back(&tokenVector, fast_create_token(INTEGER));
  token_vector_push_back(&tokenVector, fast_create_token(MULTIPLY));
  token_vector_push_back(&tokenVector, fast_create_token(DOUBLE));
  token_vector_push_back(&tokenVector, fast_create_token(RIGHT_BRACKET));

  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));
  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));

  token_vector_push_back(&tokenVector, fast_create_token(IF));
  token_vector_push_back(&tokenVector, fast_create_token(LET));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(LEFT_CURL_BRACKET));
  token_vector_push_back(&tokenVector, fast_create_token(RETURN));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));
  token_vector_push_back(&tokenVector, fast_create_token(RIGHT_CURL_BRACKET));
  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));

  token_vector_push_back(&tokenVector, fast_create_token(IF));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(EQUAL));
  token_vector_push_back(&tokenVector, fast_create_token(INTEGER));
  token_vector_push_back(&tokenVector, fast_create_token(LEFT_CURL_BRACKET));
  token_vector_push_back(&tokenVector, fast_create_token(RETURN));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(PLUS));
  token_vector_push_back(&tokenVector, fast_create_token(ID));
  token_vector_push_back(&tokenVector, fast_create_token(RIGHT_CURL_BRACKET));

  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));
  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));
  token_vector_push_back(&tokenVector, fast_create_token(RIGHT_CURL_BRACKET));
  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));
  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));
  token_vector_push_back(&tokenVector, fast_create_token(DELIMITER));

  GrammarToken grammarToken;
  grammar_token_init(&grammarToken);
  int offset = 0;
  Error err = parser_parse(&parser, &grammarToken, &tokenVector, &offset, STS,
                           LL_PARSER);
  assert(err.errorType == NONE);
  return 0;
}

//int main() { parser_config_test(); }