#include "../scanner.h"
#include "ifj23_swift_configurator.h"

void configure_plus(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '+', 59);
  automata_set_stateReturnValue(&(scanner->automata), 59, PLUS);
}
void configure_minus(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '-', 60);
  automata_set_stateReturnValue(&(scanner->automata), 60, MINUS);
}
void configure_multiply(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '*', 61);
  automata_set_stateReturnValue(&(scanner->automata), 61, MULTIPLY);
}
void configure_divide(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '/', 62);
  automata_set_stateReturnValue(&(scanner->automata), 62, DIVIDE);
}
void configure_assign(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '=', 63);
  automata_set_stateReturnValue(&(scanner->automata), 63, ASSIGN);
}
void configure_equal(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '=', 63);
  automata_set_edge(&(scanner->automata), 63, '=', 65);
  automata_set_stateReturnValue(&(scanner->automata), 65, EQUAL);
}
void configure_less(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '<', 68);
  automata_set_stateReturnValue(&(scanner->automata), 68, LESS);
}
void configure_lessequal(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '<', 68);
  automata_set_edge(&(scanner->automata), 68, '=', 69);
  automata_set_stateReturnValue(&(scanner->automata), 69, LESS_EQUAL);
}
void configure_greater(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '>', 70);
  automata_set_stateReturnValue(&(scanner->automata), 70, GREATER);
}
void configure_greaterequal(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '>', 70);
  automata_set_edge(&(scanner->automata), 70, '=', 71);
  automata_set_stateReturnValue(&(scanner->automata), 71, GREATER_EQUAL);
}
void configure_notequal(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 74, '=', 100);
  automata_set_stateReturnValue(&(scanner->automata), 100, NOT_EQUAL);
}
