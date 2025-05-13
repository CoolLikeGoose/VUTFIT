#include "../scanner.h"
#include "ifj23_swift_configurator.h"

void configure_left_curl_bracket(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '{', 55);
  automata_set_stateReturnValue(&(scanner->automata), 55, LEFT_CURL_BRACKET);
}
void configure_right_curl_bracket(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '}', 56);
  automata_set_stateReturnValue(&(scanner->automata), 56, RIGHT_CURL_BRACKET);
}
void configure_left_bracket(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, '(', 57);
  automata_set_stateReturnValue(&(scanner->automata), 57, LEFT_BRACKET);
}
void configure_right_bracket(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, ')', 58);
  automata_set_stateReturnValue(&(scanner->automata), 58, RIGHT_BRACKET);
}
void configure_colon(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, ':', 66);
  automata_set_stateReturnValue(&(scanner->automata), 66, COLON);
}
void configure_semicolon(Scanner *scanner) {
  automata_set_edge(&(scanner->automata), 0, ';', 67);
  automata_set_stateReturnValue(&(scanner->automata), 67, SEMICOLON);
}