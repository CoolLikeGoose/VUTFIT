#include "codegen/codegen.h"
#include "parser/config/config_parser.h"
#include "parser/parser.h"
#include "scanner/scanner.h"

#include <stdio.h>

void exportLLTable(Parser* parser);
void exportLRTable(Parser* parser);

int main() {
  Error err;
  err.errorType = NONE;

  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

//freopen("../IO/input.txt", "r", stdin);
//freopen("../IO/output.txt", "w", stdout);

  Parser parser;
  parser_init(&parser);
  ifj_2023_parser_config(&parser);

  CharVector inputVector;
  char_vector_init(&inputVector);
  int c = 0;
  while ((c = getchar()) != EOF) {
    char_vector_push_back(&inputVector, (char)c);
  }

//  char_vector_push_back(&inputVector, '\n');//TODO without ending line it fails why????


  if (inputVector.length == 0) {
    printf(".IFJcode23\n");
    return 0;
  }
  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  err = scanner_code_to_tokens(&scanner, inputVector.data, &tokenVector);

  if (err.errorType != NONE) {
    error_report(err);
    return err.errorCode;
  }

  GrammarToken sts;
  grammar_token_init(&sts);
  int offset = 0;
  err = parser_parse(&parser, &sts, &tokenVector, &offset, STS, LL_PARSER);

  if (err.errorType != NONE) {
    error_report(err);
    return err.errorCode;
  }
  sem_analyse_start(&sts);
  generate_inter_code(&sts);
  return 0;
}


