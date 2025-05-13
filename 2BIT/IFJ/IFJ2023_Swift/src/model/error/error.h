
#ifndef IFJ2023_SWIFT_ERROR_H
#define IFJ2023_SWIFT_ERROR_H

#include <stdbool.h>
#include <stdlib.h>
#include "stdio.h"

#define MAX_ERROR_MESSAGE 256

typedef enum error_exit_code {
  NONE,
  UNDEFINED_TOKEN,
  FATAL,
  SCANNER_ERROR,
  PARSER_ERROR,
  LEXICAL_ERROR,
  SYNTACTIC_ERROR,
  UNDEFINED_FUNCTION_ERROR,
  REDEFINE_VARIABLE_ERROR,
  FUNCTION_PARAM_ERROR,
  UNDEFINED_VARIABLE_ERROR,
  MISSING_RETURN_EXPRESSION_ERROR,
  TYPE_COMPATIBILITY_ERROR,
  TYPE_DERIVATION_ERROR,
  OTHER_SEMANTIC_ERRORS,
  INTERNAL_COMPILER_ERROR
} ErrorType;

typedef struct error_t {
  ErrorType errorType;
  int errorCode;
  char msg[MAX_ERROR_MESSAGE];
} Error;

Error error_create(ErrorType errorType, char *msg);
Error error_unite(Error errorA, Error errorB);
ErrorType error_report(Error error);

#endif // IFJ2023_SWIFT_ERROR_H
