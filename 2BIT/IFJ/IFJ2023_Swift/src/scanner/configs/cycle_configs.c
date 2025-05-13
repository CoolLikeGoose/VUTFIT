#include "../scanner.h"
#include "ifj23_swift_configurator.h"

void configure_while(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, 'w', 49);
  automata_set_edge_by_regex(&(scanner->automata), 49, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 49, 'h', 50);
  automata_set_edge_by_regex(&(scanner->automata), 50, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 50, 'i', 51);
  automata_set_edge_by_regex(&(scanner->automata), 51, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 51, 'l', 52);
  automata_set_edge_by_regex(&(scanner->automata), 52, "[a-zA-Z0-9_]+", 12);
  automata_set_edge(&(scanner->automata), 52, 'e', 53);
  automata_set_edge_by_regex(&(scanner->automata), 53, "[a-zA-Z0-9_]+", 12);
  automata_set_stateReturnValue(&(scanner->automata), 53, WHILE);
  automata_set_stateReturnValue(&(scanner->automata), 49, ID);
  automata_set_stateReturnValue(&(scanner->automata), 50, ID);
  automata_set_stateReturnValue(&(scanner->automata), 51, ID);
  automata_set_stateReturnValue(&(scanner->automata), 52, ID);
}