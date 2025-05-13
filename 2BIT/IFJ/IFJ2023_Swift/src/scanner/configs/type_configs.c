#include "../scanner.h"
#include "ifj23_swift_configurator.h"

void configure_string_type(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'S', 14);
  automata_set_edge_by_regex(&(scanner->automata), 14, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 14, 't', 15);
  automata_set_edge_by_regex(&(scanner->automata), 15, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 15, 'r', 16);
  automata_set_edge_by_regex(&(scanner->automata), 16, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 16, 'i', 17);
  automata_set_edge_by_regex(&(scanner->automata), 17, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 17, 'n', 18);
  automata_set_edge_by_regex(&(scanner->automata), 18, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 18, 'g', 19);
  automata_set_edge_by_regex(&(scanner->automata), 19, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 19, STRING_TYPE);
  automata_set_stateReturnValue(&(scanner->automata), 14, ID);
  automata_set_stateReturnValue(&(scanner->automata), 15, ID);
  automata_set_stateReturnValue(&(scanner->automata), 16, ID);
  automata_set_stateReturnValue(&(scanner->automata), 17, ID);
  automata_set_stateReturnValue(&(scanner->automata), 18, ID);
}

void configure_int_type(Scanner *scanner) {
  // 20 //21 //22
  automata_set_edge(&(scanner->automata), 0, 'I', 20);
  automata_set_edge_by_regex(&(scanner->automata), 20, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 20, 'n', 21);
  automata_set_edge_by_regex(&(scanner->automata), 21, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 21, 't', 22);
  automata_set_edge_by_regex(&(scanner->automata), 22, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 22, INT_TYPE);
  automata_set_stateReturnValue(&(scanner->automata), 20, ID);
  automata_set_stateReturnValue(&(scanner->automata), 21, ID);
}

void configure_double_type(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'D', 23);
  automata_set_edge_by_regex(&(scanner->automata), 23, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 23, 'o', 24);
  automata_set_edge_by_regex(&(scanner->automata), 24, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 24, 'u', 25);
  automata_set_edge_by_regex(&(scanner->automata), 25, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 25, 'b', 26);
  automata_set_edge_by_regex(&(scanner->automata), 26, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 26, 'l', 27);
  automata_set_edge_by_regex(&(scanner->automata), 27, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 27, 'e', 28);
  automata_set_edge_by_regex(&(scanner->automata), 28, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 28, DOUBLE_TYPE);
  automata_set_stateReturnValue(&(scanner->automata), 23, ID);
  automata_set_stateReturnValue(&(scanner->automata), 24, ID);
  automata_set_stateReturnValue(&(scanner->automata), 25, ID);
  automata_set_stateReturnValue(&(scanner->automata), 26, ID);
  automata_set_stateReturnValue(&(scanner->automata), 27, ID);
}

void configure_boolean_type(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'B', 29);
  automata_set_edge_by_regex(&(scanner->automata), 29, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 29, 'o', 30);
  automata_set_edge_by_regex(&(scanner->automata), 30, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 30, 'o', 31);
  automata_set_edge_by_regex(&(scanner->automata), 31, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 31, 'l', 32);
  automata_set_edge_by_regex(&(scanner->automata), 32, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 32, BOOLEAN_TYPE);
  automata_set_stateReturnValue(&(scanner->automata), 29, ID);
  automata_set_stateReturnValue(&(scanner->automata), 30, ID);
  automata_set_stateReturnValue(&(scanner->automata), 31, ID);
}

void configure_nullable(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 19, '?', 33);
  automata_set_edge(&(scanner->automata), 22, '?', 34);
  automata_set_edge(&(scanner->automata), 28, '?', 35);
  automata_set_edge(&(scanner->automata), 32, '?', 36);
  automata_set_stateReturnValue(&(scanner->automata), 33, STRING_NULLABLE_TYPE);
  automata_set_stateReturnValue(&(scanner->automata), 34, INT_NULLABLE_TYPE);
  automata_set_stateReturnValue(&(scanner->automata), 35, DOUBLE_NULLABLE_TYPE);
  automata_set_stateReturnValue(&(scanner->automata), 36,
                                BOOLEAN_NULLABLE_TYPE);
}