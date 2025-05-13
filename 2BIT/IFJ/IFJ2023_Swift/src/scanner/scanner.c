//
// Created by burluiva on 10/10/2023.
//

#include "scanner.h"

void scanner_init(Scanner *scanner) {
  assert(scanner);
  automata_init(&(scanner->automata), SCANNER_DEFAULT_STATE, UNDEFINED);
}

Error scanner_code_to_tokens(Scanner *scanner, char *code,
                             TokenVector *tokenVector) {
  assert(scanner && code && tokenVector);
  size_t code_len = strlen(code);

  char *tokenStr = NULL;
  int tokenStrLeftPointer = 0;
  int tokenStrRightPointer = 0;

  TokenType lastTokenTypeRecorded = UNDEFINED;
  int endTokenStrPointer = 0;

  for (int i = 0; i < code_len; i++) {
    char feed = code[i];
    scanner_move_forward(scanner, feed);

    if (scanner->automata.currentState == scanner->automata.startState) {
      if (lastTokenTypeRecorded == UNDEFINED) {
        return error_create(SCANNER_ERROR, "undefined token bro...");
      }

      tokenStr = calloc((tokenStrRightPointer - tokenStrLeftPointer + 128),
                        sizeof(char));
      strncpy(tokenStr, code + tokenStrLeftPointer,
              endTokenStrPointer - tokenStrLeftPointer + 1);

      Token token;
      Error error = token_create(lastTokenTypeRecorded, tokenStr, &token);
      if (error.errorType != NONE) {
        return error;
      }

      if (token.type != UNDEFINED && token.type != MULTI_COMMENT &&
          token.type != COMMENT && token.type != BLANK &&
          token.type != SEMICOLON) {
        token_vector_push_back(tokenVector, token);

        /// TOKEN MERGER (really shit code in design perspective)
        if (token.type == COLON || token.type == ASSIGN) {
          int index = ((int)(tokenVector->length)) - 2;
          while (index > 0 &&
                 token_vector_at(tokenVector, index).type == DELIMITER)
            index--;
          if (index > 0 && token_vector_at(tokenVector, index).type == ID) {
            // start merging
            Token mergeToken;
            token_init(&mergeToken);
            token_create(token.type == COLON ? ID_AND_COLON : ID_AND_ASSIGN,
                         NULL,
                         &mergeToken);
            mergeToken.data.string = token_vector_at(tokenVector,index).data.string;

            while(tokenVector->length > index)
              token_vector_pop(tokenVector);

            token_vector_push_back(tokenVector, mergeToken);
          }
        }
        /// END OF TOKEN MERGER
      }



      free(tokenStr);
      tokenStr = NULL;

      tokenStrLeftPointer = i;
      tokenStrRightPointer = tokenStrLeftPointer;
      endTokenStrPointer = tokenStrLeftPointer;

      lastTokenTypeRecorded = UNDEFINED;

      scanner_move_forward(scanner, feed);
    }

    tokenStrRightPointer++;

    if (scanner_get_currentTokenType(scanner) != UNDEFINED) {
      lastTokenTypeRecorded = scanner_get_currentTokenType(scanner);
      endTokenStrPointer = tokenStrRightPointer - 1;
    }
  }

//  if (tokenStrLeftPointer == endTokenStrPointer)
//    return error_create(NONE, NULL);

  if (lastTokenTypeRecorded == UNDEFINED) {
    return error_create(UNDEFINED_TOKEN, "undefined token...");
  }

  tokenStr =
      calloc((tokenStrRightPointer - tokenStrLeftPointer + 128), sizeof(char));
  strncpy(tokenStr, code + tokenStrLeftPointer,
          endTokenStrPointer - tokenStrLeftPointer + 1);

  Token token;
  Error error = token_create(lastTokenTypeRecorded, tokenStr, &token);
  if (error.errorType != NONE) {
    return error;
  }

  if (token.type != UNDEFINED && token.type != MULTI_COMMENT &&
      token.type != COMMENT && token.type != BLANK && token.type != SEMICOLON) {
    token_vector_push_back(tokenVector, token);
    /// TOKEN MERGER (really shit code in design perspective)
    if (token.type == COLON || token.type == ASSIGN) {
      int index = ((int)(tokenVector->length)) - 2;
      while (index > 0 &&
             token_vector_at(tokenVector, index).type == DELIMITER)
        index--;
      if (index > 0 && token_vector_at(tokenVector, index).type == ID) {
        // start merging
        Token mergeToken;
        token_init(&mergeToken);
        token_create(token.type == COLON ? ID_AND_COLON : ID_AND_ASSIGN,
                     NULL,
                     &mergeToken);
        mergeToken.data.string = token_vector_at(tokenVector,index).data.string;

        while(tokenVector->length > index)
          token_vector_pop(tokenVector);

        token_vector_push_back(tokenVector, mergeToken);
      }
    }
    /// END OF TOKEN MERGER
  }

  free(tokenStr);
  tokenStr = NULL;

  return error_create(NONE, NULL);
}

void scanner_move_forward(Scanner *scanner, char symbol) {
  assert(scanner);
  automata_next_state(&(scanner->automata), symbol);
}

TokenType scanner_get_currentTokenType(Scanner *scanner) {
  assert(scanner);
  return automata_get_stateReturnValue(&(scanner->automata),
                                       scanner->automata.currentState);
}