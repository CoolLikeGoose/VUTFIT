//
// Created by burluiva on 11/18/2023.
//

#ifndef IFJ2023_SWIFT_PARSE_FN_CONFIGS_H
#define IFJ2023_SWIFT_PARSE_FN_CONFIGS_H
#include "../../../model/error/error.h"
#include "../../../model/grammar/token/grammar_token.h"
#include "../../../structures/vector/vector.h"
#include "../../ll_parser/ll_parser.h"

/// STS -> S STS (statements)
Error parse_sts(LLParser *llParser, GrammarToken *grammarToken,
                TokenVector *tokenVector, int *curOffset);

/// STS ->
Error parse_sts_empty(LLParser *llParser, GrammarToken *grammarToken,
                      TokenVector *tokenVector, int *curOffset);

/// S -> while COND BLOCK
Error parse_s_while(LLParser *llParser, GrammarToken *grammarToken,
                    TokenVector *tokenVector, int *curOffset);
/// S -> if COND BLOCK IF_ELSE
Error parse_s_if(LLParser *llParser, GrammarToken *grammarToken,
                 TokenVector *tokenVector, int *curOffset);
/// S -> E (expression)
Error parse_s_e(LLParser *llParser, GrammarToken *grammarToken,
                TokenVector *tokenVector, int *curOffset);
/// S -> D (declaration)
Error parse_s_d(LLParser *llParser, GrammarToken *grammarToken,
                TokenVector *tokenVector, int *curOffset);
/// S -> return E
Error parse_s_return(LLParser *llParser, GrammarToken *grammarToken,
                     TokenVector *tokenVector, int *curOffset);
/// S->continue
Error parse_s_continue(LLParser *llParser, GrammarToken *grammarToken,
                       TokenVector *tokenVector, int *curOffset);
/// S->break
Error parse_s_break(LLParser *llParser, GrammarToken *grammarToken,
                    TokenVector *tokenVector, int *curOffset);

/// COND -> E
Error parse_cond_e(GrammarToken *grammarToken, TokenVector *tokenVector,
                   int *curOffset);
/// COND -> let id
Error parse_cond_let(LLParser *llParser, GrammarToken *grammarToken,
                     TokenVector *tokenVector, int *curOffset);

/// ELSE ->
Error parse_else_empty(LLParser *llParser, GrammarToken *grammarToken,
                       TokenVector *tokenVector, int *curOffset);

/// ELSE -> else BLOCK
Error parse_if_else(LLParser *llParser, GrammarToken *grammarToken,
                    TokenVector *tokenVector, int *curOffset);

/// BLOCK -> {STS}
Error parse_block(LLParser *llParser, GrammarToken *grammarToken,
                  TokenVector *tokenVector, int *curOffset);

/// D -> var id TANN_NULL INIT
Error parse_dec_var(LLParser *llParser, GrammarToken *grammarToken,
                    TokenVector *tokenVector, int *curOffset);

/// D -> let id TANN_NULL INIT
Error parse_dec_const(LLParser *llParser, GrammarToken *grammarToken,
                      TokenVector *tokenVector, int *curOffset);

/// D -> func id(PARAMS)->TYPE CODE_BLOCK
Error parse_dec_func(LLParser *llParser, GrammarToken *grammarToken,
                     TokenVector *tokenVector, int *curOffset);

/// PARAM -> FUNC_ID id TANN
Error parse_param(LLParser *llParser, GrammarToken *grammarToken,
                  TokenVector *tokenVector, int *curOffset);

/// FUNC_ID -> id
Error parse_func_id_id(LLParser *llParser, GrammarToken *grammarToken,
                       TokenVector *tokenVector, int *curOffset);

/// FUNC_ID -> _
Error parse_func_id_underscore(LLParser *llParser, GrammarToken *grammarToken,
                               TokenVector *tokenVector, int *curOffset);

/// PARAMS -> PARAM PARAMS_TMP
Error parse_params(LLParser *llParser, GrammarToken *grammarToken,
                   TokenVector *tokenVector, int *curOffset);

/// PARAMS_TMP ->
Error parse_params_tmp_empty(GrammarToken *grammarToken,
                             TokenVector *tokenVector, int *curOffset);

/// PARAMS_TMP -> ,PARAM PARAMS_TMP
Error parse_params_tmp(LLParser *llParser, GrammarToken *grammarToken,
                       TokenVector *tokenVector, int *curOffset);

/// PARAMS ->
Error parse_params_empty(LLParser *llParser, GrammarToken *grammarToken,
                         TokenVector *tokenVector, int *curOffset);

/// TANN -> : TYPE
Error parse_type_annotation(LLParser *llParser, GrammarToken *grammarToken,
                            TokenVector *tokenVector, int *curOffset);

/// TANN_NULL -> TANN
Error parse_nullable_type_annotation(LLParser *llParser,
                                     GrammarToken *grammarToken,
                                     TokenVector *tokenVector, int *curOffset);

/// TANN_NULL ->
Error parse_nullable_type_annotation_empty(LLParser *llParser,
                                           GrammarToken *grammarToken,
                                           TokenVector *tokenVector,
                                           int *curOffset);

/// INIT -> = E
Error parse_init(LLParser *llParser, GrammarToken *grammarToken,
                 TokenVector *tokenVector, int *curOffset);

/// INIT ->
Error parse_init_nullable(LLParser *llParser, GrammarToken *grammarToken,
                          TokenVector *tokenVector, int *curOffset);

/// F ->  (E)
Error parse_f_exp(LLParser *llParser, GrammarToken *grammarToken,
                  TokenVector *tokenVector, int *curOffset);

/// F -> -F
Error parse_f_minus(LLParser *llParser, GrammarToken *grammarToken,
                    TokenVector *tokenVector, int *curOffset);

/// F -> id F_CALL
Error parse_f_id(LLParser *llParser, GrammarToken *grammarToken,
                 TokenVector *tokenVector, int *curOffset);

/// F -> int
Error parse_f_int(LLParser *llParser, GrammarToken *grammarToken,
                  TokenVector *tokenVector, int *curOffset);

/// F -> string (value)
Error parse_f_string(LLParser *llParser, GrammarToken *grammarToken,
                     TokenVector *tokenVector, int *curOffset);

/// F -> bool (value)
Error parse_f_bool(LLParser *llParser, GrammarToken *grammarToken,
                   TokenVector *tokenVector, int *curOffset);

/// F -> !F
Error parse_f_negate(LLParser *llParser, GrammarToken *grammarToken,
                     TokenVector *tokenVector, int *curOffset);

/// F_CALL -> (ARGS)
Error parse_f_call(LLParser *llParser, GrammarToken *grammarToken,
                   TokenVector *tokenVector, int *curOffset);
/// F_CALL ->
Error parse_f_call_empty(LLParser *llParser, GrammarToken *grammarToken,
                         TokenVector *tokenVector, int *curOffset);

/// ARGS -> ARG ARGS_TMP
Error parse_args(LLParser *llParser, GrammarToken *grammarToken,
                 TokenVector *tokenVector, int *curOffset);

/// ARGS_TMP ->
Error parse_args_tmp_empty(GrammarToken *grammarToken, TokenVector *tokenVector,
                           int *curOffset);

/// ARGS_TMP -> ,ARG PARAMS_TMP
Error parse_args_tmp(LLParser *llParser, GrammarToken *grammarToken,
                     TokenVector *tokenVector, int *curOffset);

/// ARGS ->
Error parse_args_empty(LLParser *llParser, GrammarToken *grammarToken,
                       TokenVector *tokenVector, int *curOffset);

/// ARG -> id: E
Error parse_arg_id_e(LLParser *llParser, GrammarToken *grammarToken,
                     TokenVector *tokenVector, int *curOffset);

/// ARG -> E
Error parse_arg_e(LLParser *llParser, GrammarToken *grammarToken,
                  TokenVector *tokenVector, int *curOffset);

#endif // IFJ2023_SWIFT_PARSE_FN_CONFIGS_H
