//
// Created by burluiva on 10/12/2023.
//

#ifndef IFJ2023_SWIFT_AUTOMATA_H
#define IFJ2023_SWIFT_AUTOMATA_H

#include "../../model/error/error.h"

#define MAX_AUTOMATA_STATES_NUMBER 1024
#define MAX_AUTOMATA_EDGE_TYPES 256

typedef struct automata_t {
  int currentState;
  int automata[MAX_AUTOMATA_STATES_NUMBER][MAX_AUTOMATA_EDGE_TYPES];
  int stateReturnValues[MAX_AUTOMATA_STATES_NUMBER];
  int startState, defaultStateReturnValue;
} Automata;

void automata_init(Automata *automata, int startState,
                   int defaultStateReturnValue);

void automata_set_edge(Automata *automata, int fromState, int edgeType,
                       int toState);

int automata_get_stateReturnValue(Automata *automata, int state);
void automata_set_stateReturnValue(Automata *automata, int state,
                                   int returnValue);

void automata_configure(Automata *automata);

void automata_next_state(Automata *automata, int edgeType);
void automata_set_edge_by_regex(Automata *automata, int starEdge,
                                char *edgeTypeRegex, int toState);

#endif // IFJ2023_SWIFT_AUTOMATA_H
