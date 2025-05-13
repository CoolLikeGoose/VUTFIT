#include "../scanner.h"
#include "ifj23_swift_configurator.h"

void configure_if(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'i', 37);
  automata_set_edge_by_regex(&(scanner->automata), 37, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 37, 'f', 38);
  automata_set_edge_by_regex(&(scanner->automata), 38, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 38, IF);
  automata_set_stateReturnValue(&(scanner->automata), 37, ID);
}
void configure_else(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'e', 39);
  automata_set_edge_by_regex(&(scanner->automata), 39, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 39, 'l', 40);
  automata_set_edge_by_regex(&(scanner->automata), 40, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 40, 's', 41);
  automata_set_edge_by_regex(&(scanner->automata), 41, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 41, 'e', 42);
  automata_set_edge_by_regex(&(scanner->automata), 42, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 42, ELSE);
  automata_set_stateReturnValue(&(scanner->automata), 39, ID);
  automata_set_stateReturnValue(&(scanner->automata), 40, ID);
  automata_set_stateReturnValue(&(scanner->automata), 41, ID);
}