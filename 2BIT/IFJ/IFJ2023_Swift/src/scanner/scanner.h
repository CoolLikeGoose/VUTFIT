//
// Created by burluiva on 10/10/2023.
//

#ifndef IFJ2023_SWIFT_SCANNER_H
#define IFJ2023_SWIFT_SCANNER_H

#include "../model/error/error.h"
#include "../model/token/token.h"
#include "../structures/automata/automata.h"
#include "../structures/vector/vector.h"

#include <limits.h>

#define SCANNER_DEFAULT_STATE 0

/**
 * scanner is used for converting text to list/vector of tokens (prepare for
 * parsing) start state of automata always should be 0
 */
typedef struct scanner {
  Automata automata;
} Scanner;

void scanner_init(Scanner *scanner);
Error scanner_code_to_tokens(Scanner *scanner, char *code,
                             TokenVector *tokenVector);

void scanner_configure_swift_2023(Scanner *scanner);
void scanner_move_forward(Scanner *scanner, char symbol);

TokenType scanner_get_currentTokenType(Scanner *scanner);

#endif // IFJ2023_SWIFT_SCANNER_H
