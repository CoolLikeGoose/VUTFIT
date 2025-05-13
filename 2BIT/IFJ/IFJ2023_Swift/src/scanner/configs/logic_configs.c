#include "../scanner.h"
#include "ifj23_swift_configurator.h"

void configure_logical_OR(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '|', 101);
  automata_set_edge(&(scanner->automata), 101, '|', 102);
  automata_set_stateReturnValue(&(scanner->automata), 102, LOGICAL_OR);
}

void configure_logical_AND(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '&', 103);
  automata_set_edge(&(scanner->automata), 103, '&', 104);
  automata_set_stateReturnValue(&(scanner->automata), 104, LOGICAL_AND);
}