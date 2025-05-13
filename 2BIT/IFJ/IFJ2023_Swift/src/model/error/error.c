
#include "error.h"
#include "string.h"

// TODO error reporting thing
ErrorType error_report(Error error){
  switch (error.errorType) {
    case UNDEFINED_TOKEN:
      fprintf(stderr,"Some error occurred in scanner. Unknown token.\n Error msg: %s\n", error.msg);
      break;
    case PARSER_ERROR:
      fprintf(stderr,"Some error occurred in parser.\n Error msg: %s\n", error.msg);
      break;
    case SCANNER_ERROR:
      fprintf(stderr,"Some error occurred in scanner.\n Error msg: %s\n", error.msg);
      break;
    case FATAL:
      fprintf(stderr,"Some fatal error occurred.\n Error msg: %s\n", error.msg);
      break;
    case LEXICAL_ERROR:
      fprintf(stderr,"Some lexical error occurred.\n Error msg: %s\n", error.msg);
      break;
    case SYNTACTIC_ERROR:
      fprintf(stderr,"Some syntactic error occurred.\n Error msg: %s\n", error.msg);
      break;
    case UNDEFINED_FUNCTION_ERROR:
      fprintf(stderr,"Some function error occurred.\n Error msg: %s\n", error.msg);
      break;
    case FUNCTION_PARAM_ERROR:
      fprintf(stderr,"Semantic error. Incorrect number/type of parameters in a function call or wrong return type.\n Error msg: %s\n", error.msg);
      break;
    case UNDEFINED_VARIABLE_ERROR:
      fprintf(stderr,"Semantic error. Use of undefined or uninitialized variable.\n Error msg: %s\n", error.msg);
      break;
    case MISSING_RETURN_EXPRESSION_ERROR:
      fprintf(stderr,"Semantic error. Missing or extra expression in a return statement.\n Error msg: %s\n", error.msg);
      break;
    case TYPE_COMPATIBILITY_ERROR:
      fprintf(stderr,"Semantic error. Type compatibility error in arithmetic, string, or relational expressions.\n Error msg: %s\n", error.msg);
      break;
    case TYPE_DERIVATION_ERROR:
      fprintf(stderr,"Semantic error. Type of variable or parameter cannot be derived.\n Error msg: %s\n", error.msg);
      break;
    case OTHER_SEMANTIC_ERRORS:
      fprintf(stderr, "Semantic error. Other semantic errors.\n Error msg: %s\n", error.msg);
      break;
    case INTERNAL_COMPILER_ERROR:
      fprintf(stderr, "Internal compiler error. Unaffected by the input program (e.g., memory allocation error).\n Error msg: %s\n", error.msg);
      break;
    default:
      fprintf(stderr,"Some unknown error occurred.\n Error msg: %s\n", error.msg);
    }
  return error.errorType;
}
void error_code_create(Error *error){
  switch (error->errorType) {
    case NONE:
     error->errorCode = 0;
    case UNDEFINED_TOKEN:
    case SCANNER_ERROR:
    case LEXICAL_ERROR:
     error->errorCode = 1;
     break;
    case SYNTACTIC_ERROR:
     error->errorCode = 2;
     break;
    case REDEFINE_VARIABLE_ERROR:
    case UNDEFINED_FUNCTION_ERROR:
     error->errorCode = 3;
     break;
    case FUNCTION_PARAM_ERROR:
     error->errorCode = 4;
     break;
    case UNDEFINED_VARIABLE_ERROR:
     error->errorCode = 5;
     break;
    case MISSING_RETURN_EXPRESSION_ERROR:
     error->errorCode = 6;
     break;
    case TYPE_COMPATIBILITY_ERROR:
     error->errorCode = 7;
     break;
    case TYPE_DERIVATION_ERROR:
     error->errorCode = 8;
     break;
    case OTHER_SEMANTIC_ERRORS:
     error->errorCode = 9;
     break;
    case PARSER_ERROR:
    case FATAL:
    case INTERNAL_COMPILER_ERROR:
     error->errorCode = 99;
     break;
    default:
     error->errorCode = 666;
    }

  }

Error error_create(ErrorType errorType, char *msg) {
  Error error;
  error.errorType = errorType;
  error_code_create(&error);
  if (msg == NULL) msg = "No error message!\0";
  strcpy(error.msg, msg);
  return error;
}

/*Error error_unite(Error errorA, Error errorB){
      if(errorA.errorType == FATAL || errorB.errorType == FATAL) {
    return error_create(FATAL, "FATAL ERROR");
  }

  ErrorType mergeType = (errorA.errorType > errorB.errorType) ? errorA.errorType : errorB.errorType;

  char mergedMSG[MAX_ERROR_MESSAGE * 2];
  snprintf(mergedMSG, sizeof(mergedMSG), "%s\n%s", errorA.msg, errorB.msg);

  Error mergedError = error_create(mergeType, mergedMSG);
  return mergedError;
}*/
