//
// Created by burluiva on 11/30/2023.
//

#ifndef IFJ2023_SWIFT_SEM_ANALYS_H
#define IFJ2023_SWIFT_SEM_ANALYS_H

#include "../../model/symtable/symtable.h"
#include "sem_analys.h"

struct grammar_token_t;
typedef struct sem_analyser_t {
  SymTable* symTable;
  bool inFunctionContext;
} SemAnalyser;

// END OF SEM-ANALYSER

/// Init before the start of sem analyser
/// \param analyser
void sem_analyser_init(SemAnalyser* analyser);


/// This function should be used as start function
/// \param grammarToken
void sem_analyse_start(struct grammar_token_t* grammarToken);

// END OF SEM-ANALYSER


//START OF GLOBAL FUNCTIONS

/// Function should get all GLOBAL declarations in several steps
/// 1) scrap all declarations
/// 2) try to guess types (if types could not be guessed throw error)
/// 3) analyse functions on stability (if we could automatically catch all variables)
/// \param grammarToken
/// \param semAnalyser
void sem_analyse_global_declarations(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser, bool analysing);

//END OF GLOBAL FUNCTIONS

// START OF BINARY OPERATORS SECTION

///
/// \param grammarToken
/// \param semAnalyser
/// \throws
/// 7 - not same operators , could not cast them (no rescue)
/// \warning
/// function is not compatible with == or != operators
void sem_analyse_expression_binary_operators(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);

///
/// \param grammarToken
/// \param semAnalyser
/// \throws
/// 7 - not same operators , could not cast them (no rescue)
/// \warning
/// function is created only for soft unwrap
void sem_analyse_expression_soft_unwrap(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);

///
/// \param grammarToken
/// \param semAnalyser
/// \throws
/// 7 - wrong types for binary operators
void sem_analyse_expression_equal_operators(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);

// UNARY OPERATORS SUBSECTION

///
/// \param grammarToken
/// \param semAnalyser
/// \throws
/// 7 - wrong type converting (could not handle nullable types)
void sem_analyse_expression_hard_unwrap(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);

///
/// \param grammarToken
/// \param semAnalyser
/// \throws
/// 7 - wrong type converting (should take only boolean)
void sem_analyse_expression_negation(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);

// UNARY OPERATORS SUBSECTION

// END OF BINARY OPERATORS SECTION

/// init of factor like F -> ID F_CALL
/// \param grammarToken
/// \param semAnalyser
/// 5 - undefined function or uninitialized id
/// 4 - bad function call (wrong number of arguments or wrong types of arguments)
/// 8 - bad parameter (undefined type == void)
void sem_analyse_factor_store_id(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);

/// Generate suffix for function call must take exactly grammar token of args
/// \param grammarToken
void sem_analyse_gen_function_suffix_from_args(struct grammar_token_t *grammarToken, CharVector* charVector);


void sem_analyse_factor_store_value(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);
// START OF DECLARATION SECTION

///
/// \param grammarToken
/// \param semAnalyser
/// 3 - redeclaration of variable
/// 8 - bad expression, could not define type(undefined type == void || nil)
void sem_analyse_declaration_id_init_typed(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);
void sem_analyse_declaration_id_init_guess(struct grammar_token_t *grammarToken,
                                           SemAnalyser *semAnalyser);
void sem_analyse_declaration_id_let_var(struct grammar_token_t *grammarToken,
                                        SemAnalyser *semAnalyser);
///
/// \param grammarToken - should be declaration of variables
/// \param semAnalyser
/// 3 - redeclaration of variable
/// 8 - bad expression, could not define type(undefined type == void || nil)
void sem_analyse_declaration_func(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);
void sem_analyse_params_creation(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);
void sem_analyse_gen_function_suffix_from_params(struct grammar_token_t *grammarToken, CharVector* charVector);

/// generate suffix for function declaration
/// \param grammarToken grammar token must be only PARAMS
//void sem_analyse_gen_function_suffix_from_params(struct grammar_token_t *grammarToken, CharVector* charVector);

// END OF DECLARATION SECTION

// START OF CONDITION SECTION

/// push new frame for symtable
/// \param grammarToken
/// \param semAnalyser
/// \param symtableEntry
void sem_analyse_let_condition(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);

void sem_analyse_factor_move(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);
void sem_analyse_factor_brackets_move(struct grammar_token_t* grammarToken, SemAnalyser* semAnalyser);

// END OF CONDITION SECTION


#endif // IFJ2023_SWIFT_SEM_ANALYS_H
