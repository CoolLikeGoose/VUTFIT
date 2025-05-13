#include "../../scanner/configs/ifj23_swift_configurator.h"
#include "../scanner_test/scanner_test.h"
#include <stdio.h>
// Норм кенты и так знают что у них нихуя не работает

int test_if_token() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "if";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];

  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == IF);
  printf("Test if_token passed.\n");
}

int test_number_token() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "9890?";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];

  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == INTEGER);
  printf("Test int_token passed.\n");
}

int test_id_token() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "naa_irll_03_aad09_hj";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];

  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == ID);
  printf("Test id_token passed.\n");
}

int test_unite() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);

  char *code = "if (x > 0) {print(\"Hello,World!\")}";
  Error res = scanner_code_to_tokens(&scanner, code, &tokenVector);
  printf("Finished with type: %d msg: %s", res.errorType, res.msg);

  TokenType types[1000];
  for (int i = 0; i < 1000; i++) {
    types[i] = UNDEFINED;
  }
  for (size_t i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }

  TokenType expectedTypes[] = {IF,
                               BLANK,
                               LEFT_BRACKET,
                               ID,
                               BLANK,
                               GREATER,
                               BLANK,
                               INTEGER,
                               RIGHT_BRACKET,
                               BLANK,
                               LEFT_CURL_BRACKET,
                               ID,
                               LEFT_BRACKET,
                               STRING,
                               RIGHT_BRACKET,
                               RIGHT_CURL_BRACKET};
  size_t expectedLength = sizeof(expectedTypes) / sizeof(expectedTypes[0]);

  assert(tokenVector.length == expectedLength);

  for (size_t i = 0; i < tokenVector.length; i++) {
    if (tokenVector.data[i].type != expectedTypes[i]) {
      printf("Несоответствие на индексе %zu: Ожидалось %d, Получено %d\n", i,
             expectedTypes[i], tokenVector.data[i].type);
    }
    assert(tokenVector.data[i].type == expectedTypes[i]);
  }

  printf("Test unite passed.\n");
}

int test_let_token() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "let12";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];

  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == ID);
  printf("Test let_token passed.\n");
}

int test_var_token() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "var";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];

  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == VAR);
  printf("Test var_token passed.\n");
}

int test_multi_string() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);

  char *code =
      "\"\"\"\n"
      "The White Rabbit put on his spectacles.  \"Where shall I begin, \\\n"
      "please your Majesty?\" he asked.\n"
      "\n"
      "\"Begin at the beginning,\" the King said gravely, \"and go on \\\n"
      "till you come to the end; then stop.\"\n"
      "\"\"\"\0";
  //  char *code = "\"\"\"\n"
  //               "The \\\n"
  //               "please\"\n"
  //               "\"\"\"";
  Error res = scanner_code_to_tokens(&scanner, code, &tokenVector);
  printf("Finished with type: %d msg: %s\n", res.errorType, res.msg);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == MULTI_STRING);
  printf("Test multi_string_token passed.\n");
}

int test_string() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);

  char *code = "\"Hallo ,world!\"";
  Error res = scanner_code_to_tokens(&scanner, code, &tokenVector);
  printf("Finished with type: %d msg: %s \n", res.errorType, res.msg);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == STRING);
  printf("Test string_token passed.\n");
}

int test_math_operations() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);

  char *code = "12-10";
  Error res = scanner_code_to_tokens(&scanner, code, &tokenVector);
  printf("Finished with type: %d msg: %s \n", res.errorType, res.msg);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 3);
  assert(types[0] == INTEGER);
  assert(types[1] == MINUS);
  assert(types[2] == INTEGER);
  printf("Test math_operation passed.\n");
}

int test_double() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "1.e45";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == DOUBLE);
  printf("Test double_token passed.\n");
}
int test_double_nul() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "Double";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == DOUBLE_TYPE);
  printf("Test double_token passed.\n");
}

int test_double_type() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "1.e45?";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == DOUBLE);
  printf("Test double_token passed.\n");
}

int test_while() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "while";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == WHILE);
  printf("Test while_token passed.\n");
}

int test_delimiter() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "\n";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000];
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == DELIMITER);
  printf("Test delimiter_token passed.\n");
}

int logic_test() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "&& ||";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 3);
  assert(types[0] == LOGICAL_AND);
  assert(types[1] == BLANK);
  assert(types[2] == LOGICAL_OR);
  printf("test_logical passed.\n");
}

int comment_test() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "//fhhfhfhfhfh\n";
  scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 2);
  assert(types[0] == COMMENT);
  printf("test_comment passed.\n");
}

int multi_comment_test() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "/*jdkj.fdk*fdjk*fdjk*/";
  Error r = scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == MULTI_COMMENT);
  printf("test_multy_comment passed.\n");
}

int bool_test() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "(true || false) && true";
  Error r = scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 11);
  assert(types[1] == BOOLEAN);
  assert(tokenVector.data[1].data.integer_value == true);

  printf("test_bool passed.\n");
}

int break_test() {
  Scanner scanner;
  scanner_init(&scanner);
  scanner_configure_swift_2023(&scanner);

  TokenVector tokenVector;
  token_vector_init(&tokenVector);
  char *code = "break";
  Error r = scanner_code_to_tokens(&scanner, code, &tokenVector);
  TokenType types[1000] = {UNDEFINED};
  for (int i = 0; i < tokenVector.length; i++) {
    types[i] = tokenVector.data[i].type;
  }
  assert(tokenVector.length == 1);
  assert(types[0] == BREAK);

  printf("break passed.\n");
}

// int main() {
//   break_test();
//   //test_automata();
//   //test_unite();
//   test_double_nul();
//   test_delimiter();
//   test_id_token();
//   test_number_token();
//   test_if_token();
//   test_let_token();
//   test_while();
//   test_double();
//   test_var_token();
//   logic_test();
//   bool_test();
//   multi_comment_test();
//   comment_test();
//   test_multi_string();
//   test_math_operations();
//   test_string();
//   test_double_type();
//   test_double();
//   test_double_nul();
//   return 0;
// }