#include "../scanner.h"
#include "ifj23_swift_configurator.h"

void configure_let(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'l', 43);
  automata_set_edge_by_regex(&(scanner->automata), 43, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 43, 'e', 44);
  automata_set_edge_by_regex(&(scanner->automata), 44, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 44, 't', 45);
  automata_set_edge_by_regex(&(scanner->automata), 45, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 45, LET);
  automata_set_stateReturnValue(&(scanner->automata), 43, ID);
  automata_set_stateReturnValue(&(scanner->automata), 44, ID);
}
void configure_var(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'v', 46);
  automata_set_edge_by_regex(&(scanner->automata), 46, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 46, 'a', 47);
  automata_set_edge_by_regex(&(scanner->automata), 47, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 47, 'r', 48);
  automata_set_edge_by_regex(&(scanner->automata), 48, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 48, VAR);
  automata_set_stateReturnValue(&(scanner->automata), 46, ID);
  automata_set_stateReturnValue(&(scanner->automata), 47, ID);
}
void configure_continue(Scanner *scanner){
  automata_set_edge(&(scanner->automata), 0, 'c', 113);
  automata_set_edge_by_regex(&(scanner->automata), 113, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 113, 'o', 114);
  automata_set_edge_by_regex(&(scanner->automata), 114, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 114, 'n', 115);
  automata_set_edge_by_regex(&(scanner->automata), 115, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 115, 't', 116);
  automata_set_edge_by_regex(&(scanner->automata), 116, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 116, 'i', 117);
  automata_set_edge_by_regex(&(scanner->automata), 117, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 117, 'n', 118);
  automata_set_edge_by_regex(&(scanner->automata), 118, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 118, 'u', 119);
  automata_set_edge_by_regex(&(scanner->automata), 119, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 119, 'e', 120);
  automata_set_edge_by_regex(&(scanner->automata), 120, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 120, CONTINUE);
  automata_set_stateReturnValue(&(scanner->automata), 113, ID);
  automata_set_stateReturnValue(&(scanner->automata), 114, ID);
  automata_set_stateReturnValue(&(scanner->automata), 115, ID);
  automata_set_stateReturnValue(&(scanner->automata), 116, ID);
  automata_set_stateReturnValue(&(scanner->automata), 117, ID);
  automata_set_stateReturnValue(&(scanner->automata), 118, ID);
  automata_set_stateReturnValue(&(scanner->automata), 119, ID);
}
void configure_break(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'b', 121);
  automata_set_edge_by_regex(&(scanner->automata), 121, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 121, 'r', 122);
  automata_set_edge_by_regex(&(scanner->automata), 122, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 122, 'e', 123);
  automata_set_edge_by_regex(&(scanner->automata), 123, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 123, 'a', 124);
  automata_set_edge_by_regex(&(scanner->automata), 124, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 124, 'k', 125);
  automata_set_edge_by_regex(&(scanner->automata), 125, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 125, BREAK);
  automata_set_stateReturnValue(&(scanner->automata), 121, ID);
  automata_set_stateReturnValue(&(scanner->automata), 122, ID);
  automata_set_stateReturnValue(&(scanner->automata), 123, ID);
  automata_set_stateReturnValue(&(scanner->automata), 124, ID);
}