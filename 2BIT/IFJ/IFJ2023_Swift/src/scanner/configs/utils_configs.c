#include "../scanner.h"
#include "ifj23_swift_configurator.h"

void configure_func(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'f', 80);
  automata_set_edge_by_regex(&(scanner->automata), 80, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 80, 'u', 81);
  automata_set_edge_by_regex(&(scanner->automata), 81, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 81, 'n', 82);
  automata_set_edge_by_regex(&(scanner->automata), 82, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 82, 'c', 83);
  automata_set_edge_by_regex(&(scanner->automata), 83, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 83, FUNC);
  automata_set_stateReturnValue(&(scanner->automata), 80, ID);
  automata_set_stateReturnValue(&(scanner->automata), 81, ID);
  automata_set_stateReturnValue(&(scanner->automata), 82, ID);
}
void configure_arrow(Scanner *scanner) {
  automata_set_edge_by_regex(&(scanner->automata), 60, ">", 84);
  automata_set_stateReturnValue(&(scanner->automata), 84, ARROW);
}
void configure_comma(Scanner *scanner) {
  automata_set_edge_by_regex(&(scanner->automata), 0, ",", 85);
  automata_set_stateReturnValue(&(scanner->automata), 85, COMMA);
}
void configure_return(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'r', 86);
  automata_set_edge_by_regex(&(scanner->automata), 86, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 86, 'e', 87);
  automata_set_edge_by_regex(&(scanner->automata), 87, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 87, 't', 88);
  automata_set_edge_by_regex(&(scanner->automata), 88, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 88, 'u', 89);
  automata_set_edge_by_regex(&(scanner->automata), 89, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 89, 'r', 90);
  automata_set_edge_by_regex(&(scanner->automata), 90, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 90, 'n', 91);
  automata_set_edge_by_regex(&(scanner->automata), 91, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 91, RETURN);
  automata_set_stateReturnValue(&(scanner->automata), 86, ID);
  automata_set_stateReturnValue(&(scanner->automata), 87, ID);
  automata_set_stateReturnValue(&(scanner->automata), 88, ID);
  automata_set_stateReturnValue(&(scanner->automata), 89, ID);
  automata_set_stateReturnValue(&(scanner->automata), 90, ID);
}
void configure_underscore(Scanner *scanner) {
  automata_set_edge_by_regex(&(scanner->automata), 0, "_", 92);
  automata_set_edge_by_regex(&(scanner->automata), 92, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 92, UNDERSCORE);
}
void configure_comment(Scanner *scanner) {
  //automata_set_edge(&(scanner->automata), 0, '/', 93);
  automata_set_edge(&(scanner->automata), 62, '/', 94);
  automata_set_edge_by_regex(&(scanner->automata), 94, "[^\n]", 94);
  //automata_set_edge(&(scanner->automata), 94, '\n', 96);
  //automata_set_edge(&(scanner->automata), 94, '\0', 96);
  automata_set_stateReturnValue(&(scanner->automata), 94, COMMENT);
}
void configure_multi_comment(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 62, '*', 97);
  automata_set_edge_by_regex(&(scanner->automata), 97, ".", 97);
  automata_set_edge(&(scanner->automata), 97, '*', 98);
  automata_set_edge_by_regex(&(scanner->automata), 98, ".", 97);
  automata_set_edge(&(scanner->automata), 98, '/', 99);
  automata_set_stateReturnValue(&(scanner->automata), 99, MULTI_COMMENT);
}
