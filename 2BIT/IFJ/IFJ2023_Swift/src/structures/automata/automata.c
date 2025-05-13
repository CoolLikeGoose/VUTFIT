//
// Created by burluiva on 10/12/2023.
//

#include "automata.h"
#include "../../utils/logger.h"
#include <assert.h>
#include <regex.h>

#define AUTOMATA_STATE_ASSERT(T)                                               \
  assert((T) >= 0 && (T) < MAX_AUTOMATA_STATES_NUMBER)

#define AUTOMATA_EDGE_ASSERT(T) assert((T) >= 0 && (T) < MAX_AUTOMATA_EDGE_TYPES)

void automata_init(Automata *automata, int startState,
                   int defaultStateReturnValue) {
  assert(automata);

  automata->currentState = startState;
  automata->startState = startState;
  automata->defaultStateReturnValue = defaultStateReturnValue;

  for (int i = 0; i < MAX_AUTOMATA_STATES_NUMBER; i++) {
    for (int j = 0; j < MAX_AUTOMATA_EDGE_TYPES; j++) {
      automata->automata[i][j] = startState;
    }
    automata->stateReturnValues[i] = defaultStateReturnValue;
  }
}

void automata_set_edge(Automata *automata, int fromState, int edgeType,
                       int toState) {
  assert(automata);
  AUTOMATA_STATE_ASSERT(fromState);
  AUTOMATA_STATE_ASSERT(toState);
  AUTOMATA_EDGE_ASSERT(edgeType);

  automata->automata[fromState][edgeType] = toState;
}

int automata_get_stateReturnValue(Automata *automata, int state) {
  assert(automata);
  AUTOMATA_STATE_ASSERT(state);
  return automata->stateReturnValues[state];
}

void automata_set_stateReturnValue(Automata *automata, int state,
                                   int returnValue) {
  assert(automata);
  AUTOMATA_STATE_ASSERT(state);
  automata->stateReturnValues[state] = returnValue;
}

void automata_next_state(Automata *automata, int edgeType) {
  assert(automata);
  AUTOMATA_EDGE_ASSERT(edgeType);
  automata->currentState = automata->automata[automata->currentState][edgeType];
}

void automata_set_edge_by_regex(Automata *automata, int starEdge,
                                char *edgeTypeRegex, int toState) {
  regex_t regex;
  int compExitValue = regcomp(&regex, edgeTypeRegex, REG_EXTENDED);

  if (compExitValue) {
    LOG_ERROR("Could not compile regex");
    return;
  }
  char s[2];
  for (int i = 0; i < MAX_AUTOMATA_EDGE_TYPES; i++) {
    s[0] = (char)i;
    s[1] = 0;
    int res = regexec(&regex, s, 0, NULL, 0);
    if (res == 0) {
      automata_set_edge(automata, starEdge, i, toState);
    }
  }
}