//
// Created by burluiva on 11/30/2023.
//

#include "sem_analys.h"
#include "../../model/grammar/token/grammar_token.h"

void char_vector_push_cstr(CharVector *charVector, char* str) {
  int n = strlen(str);
  for (int i=0;i<n;i++) {
    char_vector_push_back(charVector, str[i]);
  }
}

void sem_analyzer_init(SemAnalyser* analyser) {
  assert(analyser);
  analyser->symTable = NULL;
  analyser->inFunctionContext = false;
}


void sem_analyse_start_local(GrammarToken* grammarToken, SemAnalyser* semAnalyser) {
  for (int i=0;i<grammarToken->tokensHolderSize;i++) {
    Token* curToken = grammarToken->tokensHolder[i];
    if (curToken->type == D && semAnalyser->symTable->length == 1) {
      continue;// all global declarations was already done
    }
    if (curToken->type>NON_TERMINAL_UNDEFINED) {
      sem_analyse_start_local(curToken->data.grammarToken, semAnalyser);
    }
  }
  if (grammarToken->grammarRule->semanticAnalysisFn != NULL) {
    grammarToken->grammarRule->semanticAnalysisFn(grammarToken, semAnalyser);
  }
}

void sem_analyse_global_declarations(GrammarToken *grammarToken,
                                     SemAnalyser *semAnalyser, bool analysing) {
  for (int i=0;i< grammarToken->tokensHolderSize;i++) {
    Token* curToken = grammarToken->tokensHolder[i];
    if (curToken->type == STS) continue; //ignore all STS token, we are interested only in declaration tokens
    if (curToken->type>NON_TERMINAL_UNDEFINED) {
      // we are interested only in declarations nothing more
      sem_analyse_global_declarations(curToken->data.grammarToken, semAnalyser,
                                      curToken->type == D ? true : analysing);
    }
  }
  if (analysing && grammarToken->grammarRule->semanticAnalysisFn != NULL) {
    grammarToken->grammarRule->semanticAnalysisFn(grammarToken, semAnalyser);
  }
}
void sem_analyse_start(GrammarToken *grammarToken) {
  SemAnalyser semAnalyser;
  sem_analyzer_init(&semAnalyser);
  SymTable symTable;
  symtable_init(&symTable);
  semAnalyser.symTable = &symTable;
  // find all declarations + define their types
  sem_analyse_global_declarations(grammarToken, &semAnalyser, false);
  sem_analyse_start_local(grammarToken, &semAnalyser);
}



void sem_analyse_expression_binary_operators(GrammarToken *grammarToken,
                                             SemAnalyser *semAnalyser) {
  TokenType operator = grammarToken->tokensHolder[1]->type;
  assert(operator != EQUAL && operator != NOT_EQUAL && operator != SOFT_UNWRAP);
  TokenType typeA = grammarToken->tokensHolder[0]->data.grammarToken->returnType;
  bool isCastableA = grammarToken->tokensHolder[0]->data.grammarToken->isCastableType;
  TokenType typeB = grammarToken->tokensHolder[2]->data.grammarToken->returnType;
  bool isCastableB = grammarToken->tokensHolder[2]->data.grammarToken->isCastableType;

  if (typeA == UNDEFINED || typeB == UNDEFINED)
    return;

  if (typeA != typeB) { //type not equal?? try to cast them
    if (typeA == INT_TYPE && isCastableA) typeA = DOUBLE_TYPE;//cast is possible only to the double
    if (typeB == INT_TYPE && isCastableB) typeB = DOUBLE_TYPE;
    if (typeA != typeB) exit(7); //could not make an operation between them
  }

  if (typeA == BOOLEAN_NULLABLE_TYPE ||
      typeA == INT_NULLABLE_TYPE ||
      typeA == DOUBLE_NULLABLE_TYPE ||
      typeA == VOID_TYPE ||
      typeA == NIL_TYPE ||
      typeA == UNDEFINED) exit(7);//to the these types binary operators could not be applied at all

  bool undefinedOperator = false;
  TokenType resultType = typeA;
  switch (operator) {
  case PLUS:
    resultType = typeA;
    undefinedOperator =  (typeA == BOOLEAN_TYPE);
    break;
  case MINUS:
    resultType = typeA;
    undefinedOperator =  (typeA == BOOLEAN_TYPE || typeA == STRING_TYPE);
    break;
  case MULTIPLY:
  case DIVIDE:
    resultType = typeA;
    undefinedOperator = (typeA == BOOLEAN_TYPE || typeA == STRING_TYPE);
    break;
  case LOGICAL_AND:
  case LOGICAL_OR:
    resultType = typeA;
    undefinedOperator = (typeA != BOOLEAN_TYPE);
    break;
  case LESS:
  case LESS_EQUAL:
  case GREATER_EQUAL:
  case GREATER:
    resultType = BOOLEAN_TYPE;
    undefinedOperator = (typeA == BOOLEAN_TYPE);
    break;
  default:
    exit(9);//some operator not included???
    break;
  }

  if (undefinedOperator) {
    fprintf(stderr, "Basic operator failed, wrong types!\n");
    exit(7); // does not have operator for current types
  }
  grammarToken->returnType = resultType;
  grammarToken->tokensHolder[0]->data.grammarToken->returnType = typeA;
  grammarToken->tokensHolder[2]->data.grammarToken->returnType = typeB;
  grammarToken->isCastableType = isCastableB && isCastableA;
}

//TODO swift has such behaviour but we need to check
void sem_analyse_expression_hard_unwrap(GrammarToken *grammarToken,
                                        SemAnalyser *semAnalyser) {
  TokenType type = grammarToken->tokensHolder[0]->data.grammarToken->returnType;
  if (type == UNDEFINED) return;

  if (!isNullableType(type)) {
    fprintf(stderr, "Could not unwrap non nullable or nil using hard unwrap operation!");
    exit(7);
  }
  grammarToken->returnType = getReversedType(type);
  grammarToken->isCastableType = grammarToken->tokensHolder[0]->data.grammarToken->isCastableType;
}


void sem_analyse_expression_soft_unwrap(GrammarToken *grammarToken,
                                        SemAnalyser *semAnalyser) {
  TokenType operator = grammarToken->tokensHolder[1]->type;
  assert(operator == SOFT_UNWRAP);
  TokenType typeA = grammarToken->tokensHolder[0]->data.grammarToken->returnType;
  bool isCastableA = grammarToken->tokensHolder[0]->data.grammarToken->isCastableType;
  TokenType typeB = grammarToken->tokensHolder[2]->data.grammarToken->returnType;
  bool isCastableB = grammarToken->tokensHolder[2]->data.grammarToken->isCastableType;

  if (typeB == UNDEFINED || typeA == UNDEFINED) return;//TODO

  if (typeA == UNDEFINED || typeB == UNDEFINED || typeA == VOID_TYPE || typeB == VOID_TYPE)
    exit(7);

  if (isNullableType(typeB)) {
    exit(7);
  }

  if (typeA == NIL_TYPE) {
    grammarToken->returnType = typeB;
    grammarToken->isCastableType = isCastableB;
    return;
  }

  if (isNullableType(typeA)) {
    if (getReversedType(typeA) != typeB) {
      if (typeB == INT_TYPE && isCastableB) typeB = DOUBLE_TYPE;
      if (getReversedType(typeA) != typeB) exit(7);
      grammarToken->tokensHolder[2]->data.grammarToken->returnType = typeB;
    }

    grammarToken->returnType = typeB;
    grammarToken->isCastableType = isCastableB;
    return;
  }

  if (typeA != typeB) { //type not equal?? try to cast them
    if (typeA == INT_TYPE && isCastableA) typeA = DOUBLE_TYPE;//cast is possible only to the double
    if (typeB == INT_TYPE && isCastableB) typeB = DOUBLE_TYPE;
    if (typeA != typeB) exit(7); //two types
    grammarToken->tokensHolder[0]->data.grammarToken->returnType = typeA;
    grammarToken->tokensHolder[2]->data.grammarToken->returnType = typeB;
  }

  grammarToken->returnType = typeB;
  grammarToken->isCastableType = isCastableB;
}

void sem_analyse_expression_negation(GrammarToken *grammarToken,
                                     SemAnalyser *semAnalyser) {
  TokenType type = grammarToken->tokensHolder[0]->data.grammarToken->returnType;
  if (type == UNDEFINED) return;

  if (type != BOOLEAN_TYPE)
    exit(7);
  grammarToken->returnType = BOOLEAN_TYPE;
  grammarToken->isCastableType = grammarToken->tokensHolder[0]->
                                 data.grammarToken->isCastableType;
}

void sem_analyse_expression_equal_operators(GrammarToken *grammarToken,
                                            SemAnalyser *semAnalyser) {
  TokenType operator = grammarToken->tokensHolder[1]->type;
  assert(operator == EQUAL || operator == NOT_EQUAL);
  TokenType typeA = grammarToken->tokensHolder[0]->data.grammarToken->returnType;
  bool isCastableA = grammarToken->tokensHolder[0]->data.grammarToken->isCastableType;
  TokenType typeB = grammarToken->tokensHolder[2]->data.grammarToken->returnType;
  bool isCastableB = grammarToken->tokensHolder[2]->data.grammarToken->returnType;

  if (typeA == NIL_TYPE || typeB == NIL_TYPE) {
    grammarToken->returnType = BOOLEAN;
    grammarToken->isCastableType = isCastableB && isCastableA;
    return;
  }

  if (typeB == UNDEFINED || typeB == UNDEFINED) return;

  if (typeA != typeB) { //type not equal?? try to cast them
    if (typeA == INT_TYPE && isCastableA) typeA = DOUBLE_TYPE;//cast is possible only to the double
    if (typeB == INT_TYPE && isCastableB) typeB = DOUBLE_TYPE;
    if (typeA != typeB) exit(7); //could not make an operation between them
    grammarToken->tokensHolder[0]->data.grammarToken->returnType = typeA;
    grammarToken->tokensHolder[2]->data.grammarToken->returnType = typeB;
  }

  grammarToken->returnType = BOOLEAN;
  grammarToken->isCastableType = isCastableB && isCastableA;
}


void sem_analyse_factor_store_value(GrammarToken *grammarToken,
                                    SemAnalyser *semAnalyser) {
  Token* token = grammarToken->tokensHolder[0];
  switch (token->type) {
  case STRING:
    grammarToken->isCastableType = false;
    grammarToken->returnType = STRING_TYPE;
    break;
  case INTEGER:
    grammarToken->isCastableType = true;
    grammarToken->returnType = INT_TYPE;
    break;
  case DOUBLE:
    grammarToken->isCastableType = false;
    grammarToken->returnType = DOUBLE_TYPE;
    break;
  case BOOLEAN:
    grammarToken->isCastableType = false;
    grammarToken->returnType = BOOLEAN_TYPE;
    break;
  case NIL:
    grammarToken->isCastableType = false;
    grammarToken->returnType = NIL_TYPE;
    break;
  default:
    exit(9);//strange situation... should not happen
    break;
  }
}

void sem_analyse_factor_store_id(GrammarToken *grammarToken,
                                 SemAnalyser *semAnalyser) {
  GrammarToken* fCall = grammarToken->tokensHolder[1]->data.grammarToken;
  SymtableEntry* found = NULL;

  if (fCall->tokensHolderSize == 0) {
    char *key = grammarToken->tokensHolder[0]->data.string.data;
    found = symtable_get_by_key(semAnalyser->symTable, key);
    if (found == NULL || !(found->value.isInitialised)) {
      fprintf(stderr, "Usage of undefined or uninitialised variable %s", key);
      exit(5);
    }
    grammarToken->returnType = found->value.returnType;
    grammarToken->isCastableType = false;
    return;
  }

  if (fCall->tokensHolderSize == 3) {

    CharVector charVector;
    char_vector_init(&charVector);
    char_vector_push_cstr(&charVector, grammarToken->tokensHolder[0]->data.string.data);
    char_vector_push_back(&charVector, '$');//sign it as function
    char_vector_push_back(&charVector, '\0');

    found = symtable_get_by_hash(semAnalyser->symTable, charVector.data);
    if (found == NULL)
      exit(3);//undefined function was found

    char_vector_pop(&charVector);
    sem_analyse_gen_function_suffix_from_args(fCall->tokensHolder[1]->data.grammarToken, &charVector);

    found = symtable_get_by_key(semAnalyser->symTable, charVector.data);
    char_vector_pop(&charVector);
    if (found == NULL)
      exit(4);//bad number of arguments

    grammarToken->returnType = found->value.returnType;
    grammarToken->isCastableType = false;
    return;
  }
  assert(NULL && "WTF why am I here....");
  return;
}

void process_arg(GrammarToken* grammarToken, CharVector* charVector);
void sem_analyse_gen_function_suffix_from_args(GrammarToken *grammarToken, CharVector* charVector) {
//      grammar_rule_create(ARGS, NULL, NULL, NULL, (TokenType[]){ARG, ARGS_TMP}, 2),
//      grammar_rule_create(ARGS, NULL, NULL, NULL, (TokenType[]){}, 0),
//      grammar_rule_create(ARGS_TMP, NULL, NULL, NULL, (TokenType[]){}, 0),
//      grammar_rule_create(ARGS_TMP, NULL, NULL, NULL,
//                          (TokenType[]){COMMA, ARG, ARGS_TMP}, 3),
//
//      grammar_rule_create(ARG, NULL, PushArgLabeled, NULL, (TokenType[]){ID_AND_COLON, E}, 2),
//      grammar_rule_create(ARG, NULL, PushArg, NULL, (TokenType[]){E}, 1)
  if (grammarToken->tokensHolderSize == 0) return;
  process_arg(grammarToken->tokensHolder[0]->data.grammarToken, charVector);
  GrammarToken* argsTmp = grammarToken->tokensHolder[1]->data.grammarToken;
  while(argsTmp->tokensHolderSize>0) {
    process_arg(argsTmp->tokensHolder[1]->data.grammarToken, charVector);
    argsTmp = argsTmp->tokensHolder[2]->data.grammarToken;
  }
  return;
}


void process_arg(GrammarToken* grammarToken, CharVector* charVector) {
  if (grammarToken->tokensHolderSize == 1) {
    char_vector_push_back(charVector, '_');
    TokenType returnType = grammarToken->tokensHolder[0]->data.grammarToken->returnType;
    if (returnType == UNDEFINED) {
      fprintf(stderr, "Undefined type of expression, could not parse to function!\n");
      exit(8);
    }
    if (returnType == VOID_TYPE) {
      fprintf(stderr, "Argument of function call could not be void type!\n");
      exit(8);
    }
    char_vector_push_back(charVector, (char) returnType);
  }
  if (grammarToken->tokensHolderSize == 2) {
    char_vector_push_cstr(charVector, grammarToken->tokensHolder[0]->data.string.data);
    TokenType returnType = grammarToken->tokensHolder[1]->data.grammarToken->returnType;
    if (returnType == UNDEFINED) {
      fprintf(stderr, "Undefined type of expression, could not parse to function!\n");
      exit(8);
    }
    char_vector_push_back(charVector, (char) returnType);
  }
}


void sem_analyse_declaration_id_init_typed(GrammarToken *grammarToken,
                                        SemAnalyser *semAnalyser) {
//      grammar_rule_create(ID_INIT, NULL, VarTypedIdInit, NULL,
//                          (TokenType[]){ID_AND_COLON, TYPE, ID_INIT_ASSIGN}, 3),
//
//      grammar_rule_create(ID_INIT_ASSIGN, NULL, IdInitAssignPropagate, NULL,
//                          (TokenType[]){ASSIGN, E}, 2),
//      grammar_rule_create(ID_INIT_ASSIGN, NULL, NULL, NULL,
//                          (TokenType[]){}, 0)

  SymtableEntry *found = symtable_get_by_key(semAnalyser->symTable, grammarToken->tokensHolder[0]->data.string.data);
  if (found != NULL)
    exit(3);
  SymtableValue symtableValue;
  symtable_value_init(&symtableValue);

  GrammarToken *id_init_assign = grammarToken->tokensHolder[2]->data.grammarToken;
  symtableValue.isInitialised = (id_init_assign->tokensHolderSize != 0);
  symtableValue.returnType = grammarToken->tokensHolder[1]->type;

  if (symtableValue.isInitialised) {
    TokenType expectedType = grammarToken->tokensHolder[1]->type;
    TokenType expressionType = id_init_assign->tokensHolder[1]->data.grammarToken->returnType;
    if (expectedType != expressionType) {
      bool good = isNullableType(expectedType) &&
          (expressionType == NIL_TYPE || getReversedType(expressionType) == expectedType);
      if (!good)
        exit(7);
    }
  }

  SymtableEntry* entry;
  Error err = symtable_entry_create(&entry, grammarToken->tokensHolder[0]->data.string.data, &symtableValue);
  if (err.errorType != NONE) {
    error_report(err);
    exit(err.errorCode);
  }
  err = symtable_add(semAnalyser->symTable, entry);
  if (err.errorType != NONE) {
    error_report(err);
    exit(err.errorCode);
  }
}

void sem_analyse_declaration_id_init_guess(GrammarToken *grammarToken,
                                           SemAnalyser *semAnalyser)
{
//      grammar_rule_create(ID_INIT, NULL, VarIdInit, NULL,
//                          (TokenType[]){ID_AND_ASSIGN, E}, 2),

  SymtableEntry *found = symtable_get_by_key(semAnalyser->symTable, grammarToken->tokensHolder[0]->data.string.data);
  if (found != NULL)
    exit(3);
  SymtableValue symtableValue;
  symtable_value_init(&symtableValue);

  GrammarToken *e = grammarToken->tokensHolder[2]->data.grammarToken;
  symtableValue.isInitialised = true;
  if (e->returnType == UNDEFINED) {
    fprintf(stderr,"Result of expression is UNDEFINED!\n");
    exit(7);
  }
  if (e->returnType == NIL_TYPE) {
    fprintf(stderr, "Id could not define his type from NIL types!\n");
    exit(8);
  }
  if (e->returnType == VOID_TYPE) {
    fprintf(stderr, "Id could not have type VOID!\n");
    exit(7);
  }

  symtableValue.returnType = e->returnType;
  symtableValue.isInitialised = true;

  SymtableEntry* entry;
  Error err = symtable_entry_create(&entry, grammarToken->tokensHolder[0]->data.string.data, &symtableValue);
  if (err.errorType != NONE) {
    error_report(err);
    exit(err.errorCode);
  }
  err = symtable_add(semAnalyser->symTable, entry);
  if (err.errorType != NONE) {
    error_report(err);
    exit(err.errorCode);
  }
}

void sem_analyse_declaration_id_let_var(GrammarToken *grammarToken,
                                           SemAnalyser *semAnalyser) {
//  grammar_rule_create(D, NULL, NULL, NULL,
//                      (TokenType[]){VAR, ID_INIT}, 2),
//      grammar_rule_create(D, NULL, NULL, NULL,
//                          (TokenType[]){LET, ID_INIT}, 2),

  SymtableEntry *found =
      symtable_get_by_key(semAnalyser->symTable, grammarToken->
                                                 tokensHolder[1]->
                                                 data.grammarToken->
                                                 tokensHolder[0]->
                                                 data.string.data);
  if (found == NULL) {
    fprintf(stderr, "LOL we declared function previously but have not found it in symtable....\n");
    exit(99);
  }
  TokenType constToken = grammarToken->tokensHolder[0]->type;
  found->value.isConstant = (constToken == LET);
}

void sem_analyse_func_content(GrammarToken *grammarToken, SemAnalyser *semAnalyser);
void sem_analyse_declaration_func(GrammarToken *grammarToken,
                                  SemAnalyser *semAnalyser) {
//  grammar_rule_create(D, NULL, FuncInitializeEscape, FuncInitialize,
//                      (TokenType[]){FUNC, ID, LEFT_BRACKET, PARAMS,
//                                    RIGHT_BRACKET, FUNC_RETURN, BLOCK},
//                      7),

  CharVector charVector;
  char_vector_init(&charVector);
  char_vector_push_cstr(&charVector, grammarToken->tokensHolder[0]->data.string.data);
  char_vector_push_back(&charVector, '$');
  char_vector_push_back(&charVector, '\0');
  SymtableEntry *found = symtable_get_by_key(semAnalyser->symTable, charVector.data);
  if (found != NULL) {
    fprintf(stderr, "Redeclaration or overload of function[%s] is forbidden in this implementation!\n",
            grammarToken->tokensHolder[0]->data.string.data);
    exit(3);
  }

  SymtableValue symtableValue;
  symtable_value_init(&symtableValue);
  GrammarToken* func_return = grammarToken->tokensHolder[5]->data.grammarToken;
  symtableValue.returnType =
      func_return->tokensHolderSize == 0 ? VOID_TYPE : func_return->tokensHolder[1]->type;
  symtableValue.isConstant = true;
  symtableValue.isInitialised = true;

  SymtableEntry* entry;
  Error err = symtable_entry_create(&entry, charVector.data, &symtableValue);
  if (err.errorType != NONE) {
    error_report(err);
    exit(err.errorCode);
  }
  err = symtable_add(semAnalyser->symTable, entry);
  if (err.errorType != NONE) {
    error_report(err);
    exit(err.errorCode);
  }

  sem_analyse_func_content(grammarToken, semAnalyser);
}

void sem_analyse_func_content(GrammarToken *grammarToken, SemAnalyser *semAnalyser) {
  SemAnalyser funcSemAnalyser;
  sem_analyzer_init(&funcSemAnalyser);
  SymTable symTable;
  symtable_init(&symTable);
  hashmap_vector_push_back(&symTable, semAnalyser->symTable->data[0]);
  symtable_push_frame(&symTable);
  sem_analyse_params_creation(grammarToken->tokensHolder[3]->data.grammarToken,
                              &funcSemAnalyser);
  symtable_push_frame(&symTable);
  sem_analyse_start_local(grammarToken->
                          tokensHolder[6]->data.grammarToken->
                          tokensHolder[1]->data.grammarToken,
                          &funcSemAnalyser);
  symtable_pop_frame(&symTable);
  symtable_pop_frame(&symTable);
}

///THIS IS SHITTY FIX BUT UNFORTUNATELY IT WORKS
void sem_analyse_params_creation(GrammarToken* grammarToken, SemAnalyser* semAnalyser) {
  //  grammar_rule_create(PARAMS, NULL, NULL, NULL, (TokenType[]){PARAM, PARAMS_TMP},
  //                      2),
  //      grammar_rule_create(PARAMS_TMP, NULL, NULL, NULL, (TokenType[]){}, 0),
  //      grammar_rule_create(PARAMS_TMP, NULL, NULL, NULL,
  //                          (TokenType[]){COMMA, PARAM, PARAMS_TMP}, 3),
  //      grammar_rule_create(PARAMS, NULL, NULL, NULL, (TokenType[]){}, 0),
  // grammar_rule_create(PARAM, NULL, NULL, NULL, (TokenType[]){FUNC_ID, ID_AND_COLON, TYPE},
  if (grammarToken->tokensHolderSize > 0) {
    GrammarToken *params_tmp = grammarToken;
    int move = 0;
    do {
      GrammarToken *param = params_tmp->tokensHolder[move]->data.grammarToken;
      SymtableValue symtableValue;
      symtable_value_init(&symtableValue);
      symtableValue.isInitialised = true;
      symtableValue.isConstant = true;
      symtableValue.returnType = param->tokensHolder[2]->type;
      SymtableEntry *symtableEntry;
      symtable_entry_create(&symtableEntry, param->tokensHolder[1]->data.string.data, &symtableValue);
      symtable_add(semAnalyser->symTable, symtableEntry);
      params_tmp = params_tmp->tokensHolder[1+move]->data.grammarToken;
      move = 1;
    } while(params_tmp->tokensHolderSize > 0);
  }
}

void sem_analyse_let_condition(GrammarToken *grammarToken,
                               SemAnalyser *semAnalyser) {
  SymtableEntry *found = symtable_get_by_key(semAnalyser->symTable,
                                             grammarToken->tokensHolder[0]->data.string.data);
  if (found == NULL || !(found->value.isInitialised)) {
    fprintf(stderr,"Undefined or uninitialized id[%s] got\n", found->key);
    exit(5);
  }
  symtable_push_frame(semAnalyser->symTable);
  SymtableEntry* entry;
  Error err = symtable_entry_create(&entry, found->key, &found->value);
  if (err.errorType != NONE) {
    error_report(err);
    exit(err.errorCode);
  }
  entry->value.isInitialised = true;
  entry->value.isConstant = true;
  err = symtable_add(semAnalyser->symTable, entry);
  if (err.errorType != NONE) {
    error_report(err);
    exit(err.errorCode);
  }

}

void sem_analyse_gen_function_suffix_from_params(GrammarToken *grammarToken,
                                                 CharVector *charVector) {
//  grammar_rule_create(PARAMS, NULL, NULL, NULL, (TokenType[]){PARAM, PARAMS_TMP},
//                      2),
//      grammar_rule_create(PARAMS_TMP, NULL, NULL, NULL, (TokenType[]){}, 0),
//      grammar_rule_create(PARAMS_TMP, NULL, NULL, NULL,
//                          (TokenType[]){COMMA, PARAM, PARAMS_TMP}, 3),
//      grammar_rule_create(PARAMS, NULL, NULL, NULL, (TokenType[]){}, 0),
// grammar_rule_create(PARAM, NULL, NULL, NULL, (TokenType[]){FUNC_ID, ID_AND_COLON, TYPE},
//                          3),

  if (grammarToken->tokensHolderSize > 0) {
    GrammarToken *params_tmp = grammarToken;
    int move = 0;
    do {
      GrammarToken *param = params_tmp->tokensHolder[move]->data.grammarToken;
      GrammarToken *func_id = param->tokensHolder[0]->data.grammarToken;

      char_vector_push_cstr(charVector, func_id->tokensHolder[0]->data.string.data);
      char_vector_push_back(charVector, (char) param->tokensHolder[2]->type);

      params_tmp = params_tmp->tokensHolder[1+move]->data.grammarToken;
      move = 1;
    } while(params_tmp->tokensHolderSize > 0);
  }
}


void sem_analyse_factor_move(GrammarToken* grammarToken, SemAnalyser* semAnalyser){
  grammarToken->returnType = grammarToken->tokensHolder[0]->data.grammarToken->returnType;
  grammarToken->isCastableType = grammarToken->tokensHolder[0]->data.grammarToken->isCastableType;
}

void sem_analyse_factor_brackets_move(GrammarToken* grammarToken, SemAnalyser* semAnalyser){
  grammarToken->returnType = grammarToken->tokensHolder[1]->data.grammarToken->returnType;
  grammarToken->isCastableType = grammarToken->tokensHolder[1]->data.grammarToken->isCastableType;
}