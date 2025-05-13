//
// Created by burluiva on 10/10/2023.
//

#ifndef IFJ2023_SWIFT_TOKEN_H
#define IFJ2023_SWIFT_TOKEN_H

#include "../../structures/string/string.h"
#include "../../structures/vector/vector.h"
#include "../error/error.h"

#define MAX_TOKEN_TYPES_NUMBER 128

/*
 * only terminal token could be
 */
typedef enum token_enum {
  /// TERMINALS (should be constructed by scanner)
  UNDEFINED = 0,

  STRING = 1,
  INTEGER = 2,
  DOUBLE = 3,
  NIL = 4,
  MULTI_STRING = 5,
  COMMENT = 6,
  MULTI_COMMENT = 7,
  BOOLEAN = 8,

  ID = 9,

  BLANK = 10,
  DELIMITER = 11,

  STRING_TYPE = 12,
  INT_TYPE = 13,
  DOUBLE_TYPE = 14,
  BOOLEAN_TYPE = 15,
  STRING_NULLABLE_TYPE = 16,
  INT_NULLABLE_TYPE = 17,
  DOUBLE_NULLABLE_TYPE = 18,
  BOOLEAN_NULLABLE_TYPE = 19,

  IF = 20,
  ELSE = 21,

  LET = 22,
  VAR = 23,
  CONTINUE = 24,
  BREAK = 25,

  WHILE = 26,

  // QUOTE,
  LEFT_CURL_BRACKET = 27,
  RIGHT_CURL_BRACKET = 28,
  LEFT_BRACKET = 29,
  RIGHT_BRACKET = 30,
  COLON = 31,
  SEMICOLON = 32,

  PLUS = 33,
  MINUS = 34,
  MULTIPLY = 35,
  DIVIDE = 36,
  EQUAL = 37,
  ASSIGN = 38,
  LESS_EQUAL = 39,
  GREATER_EQUAL = 40,
  GREATER = 41,
  LESS = 42,
  NOT_EQUAL = 43,

  SOFT_UNWRAP = 44,
  HARD_UNWRAP = 45,

  FUNC = 46,
  ARROW = 47,
  COMMA = 48,
  RETURN = 49,
  UNDERSCORE = 50,
  LOGICAL_AND = 51,
  LOGICAL_OR = 52,

  ID_AND_ASSIGN=53, // exactly id=
  ID_AND_COLON=54,// exactly id:
  VOID_TYPE=55,
  NIL_TYPE=56,
  /// used only for PSA algo
  DOLLAR=57,

  /// NON_TERMINALS (should be constructed by parser)
  /// tokeType < NON_TERMINAL -> terminal, tokeType > NON_TERMINAL -
  /// non-terminal
  NON_TERMINAL_UNDEFINED, // the start of non-terminal symbols

  /// one statements
  S,
  /// expression
  E,
  /// factor of expression
  F,
  /// collection of statements
  STS,
  /// collection of statements (USED FOR LL)
  STS_TMP,
  /// CONDITION
  COND,
  /// BLOCK
  BLOCK,
  /// ELSE part of if statement
  IF_ELSE,
  /// declaration
  D,
  ID_INIT,
  /// parameter for the function (declaration)
  PARAM,
  /// specific case for declaration functions (_ || id)
  FUNC_ID,
  /// collection of params
  PARAMS,
  /// collection of params (USED FOR LL)
  PARAMS_TMP,
  /// annotation of the type (Example = id : Integer), could not be null
  TANN,
  TYPE,
  /// call of function inside expression
  F_CALL,
  /// collection of arguments (calling function)
  ARGS,
  /// collection of arguments (calling function) (USED FOR LL)
  ARGS_TMP,
  /// function argument (calling function)
  ARG,
  FUNC_RETURN,
  /// needed for undefined declaration of id
  ID_INIT_ASSIGN,

  E_NULLABLE
} TokenType;

struct grammar_token_t;

typedef struct token {
  TokenType type;
  union token_data_t {
    long long integer_value;
    double double_value;
    String string;
    struct grammar_token_t *grammarToken;
  } data;
} Token;

vector_def(Token, Token, token_);

void token_init(Token *token);

/// Creates terminal token type by catching the string
/// \param type - type of the token
/// \param str - caught string
/// \return pre-processed token
/// \note
/// \attention return token is not parsed one
Error token_create(TokenType type, char *str, Token *outToken);

/// Compares two token (is not expected to be used for order purposes)
/// \param a - first token
/// \param b - second token
/// \return 0 - tokens are equal, otherwise not equal
int token_cmp(Token* a, Token* b);

/// Creates non terminal token from other tokensHolder
/// \param type - type of token
/// \param grammarToken - caught tokensHolder
/// \return pre-processed token (not parsed one)
Token token_grammar_token_create(TokenType type,
                                 struct grammar_token_t *grammarToken);

void token_free(Token *token);



void delete_quotes(char **str);
Error preprocess_literal_multiString(char *literal);
int get_multiLine_indent(char *literal);
Error preprocess_literal_string(char *literal);
Error process_unicode(char **sequence, char *output);

TokenType getReversedType(TokenType type);
bool isNullableType(TokenType tokenType);

#endif // IFJ2023_SWIFT_TOKEN_H
