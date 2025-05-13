#include "configs/ifj23_swift_configurator.h"
#include "scanner.h"

void scanner_configure_swift_2023(Scanner *scanner) {
  configure_integer(scanner);
  configure_multi_string(scanner);
  configure_double(scanner);
  configure_string(scanner);

  configure_id(scanner);
  configure_blank(scanner);
  configure_delimiter(scanner);

  configure_string_type(scanner);
  configure_int_type(scanner);
  configure_double_type(scanner);
  configure_nullable(scanner);
  configure_boolean_type(scanner);

  configure_if(scanner);
  configure_else(scanner);

  configure_let(scanner);
  configure_var(scanner);
  configure_continue(scanner);
  configure_break(scanner);

  configure_while(scanner);

  configure_plus(scanner);
  configure_minus(scanner);
  configure_multiply(scanner);
  configure_divide(scanner);
  configure_assign(scanner);

  configure_equal(scanner);
  configure_less(scanner);
  configure_lessequal(scanner);
  configure_greater(scanner);
  configure_greaterequal(scanner);
  configure_notequal(scanner);

  //configure_quote(scanner);
  configure_left_curl_bracket(scanner);
  configure_right_bracket(scanner);
  configure_right_curl_bracket(scanner);
  configure_left_bracket(scanner);

  configure_arrow(scanner);
  configure_func(scanner);
  configure_comma(scanner);
  configure_return(scanner);
  configure_underscore(scanner);
  configure_comment(scanner);
  configure_multi_comment(scanner);

  configure_soft_unwrap(scanner);
  configure_hard_unwrap(scanner);

  configure_colon(scanner);
  configure_semicolon(scanner);
  configure_logical_AND(scanner);
  configure_logical_OR(scanner);
  configure_boolean(scanner);
  configure_nil(scanner);

}

/*
 * each terminal should have its oven configuration function, and all of them
 * should be collected into configure automata
 */