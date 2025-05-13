//
// Created by burluiva on 10/13/2023.
//

#ifndef IFJ2023_SWIFT_IFJ23_SWIFT_CONFIGURATOR_H
#define IFJ2023_SWIFT_IFJ23_SWIFT_CONFIGURATOR_H

#include "../scanner.h"

void configure_string(Scanner *scanner);
void configure_integer(Scanner *scanner);
void configure_double(Scanner *scanner);
void configure_nil(Scanner *scanner);
void configure_multi_string(Scanner *scanner);
void configure_boolean(Scanner *scanner);

void configure_id(Scanner *scanner);
void configure_blank(Scanner *scanner);
void configure_delimiter(Scanner *scanner);

void configure_string_type(Scanner *scanner);
void configure_int_type(Scanner *scanner);
void configure_double_type(Scanner *scanner);
void configure_nullable(Scanner *scanner);
void configure_boolean_type(Scanner *scanner);

void configure_if(Scanner *scanner);
void configure_else(Scanner *scanner);

void configure_let(Scanner *scanner);
void configure_var(Scanner *scanner);
void configure_continue(Scanner *scanner);
void configure_break(Scanner *scanner);

void configure_while(Scanner *scanner);

void configure_plus(Scanner *scanner);
void configure_minus(Scanner *scanner);
void configure_multiply(Scanner *scanner);
void configure_divide(Scanner *scanner);

void configure_assign(Scanner *scanner);
void configure_equal(Scanner *scanner);
void configure_less(Scanner *scanner);
void configure_lessequal(Scanner *scanner);
void configure_greater(Scanner *scanner);
void configure_greaterequal(Scanner *scanner);
void configure_notequal(Scanner *scanner);

void configure_left_curl_bracket(Scanner *scanner);
void configure_right_curl_bracket(Scanner *scanner);
void configure_left_bracket(Scanner *scanner);
void configure_right_bracket(Scanner *scanner);

void configure_func(Scanner *scanner);
void configure_arrow(Scanner *scanner);
void configure_comma(Scanner *scanner);
void configure_return(Scanner *scanner);
void configure_underscore(Scanner *scanner);
void configure_comment(Scanner *scanner);
void configure_multi_comment(Scanner *scanner);

void configure_soft_unwrap(Scanner *scanner);
void configure_hard_unwrap(Scanner *scanner);

void configure_colon(Scanner *scanner);
void configure_semicolon(Scanner *scanner);

void configure_logical_OR(Scanner *scanner);
void configure_logical_AND(Scanner *scanner);

#endif // IFJ2023_SWIFT_IFJ23_SWIFT_CONFIGURATOR_H
