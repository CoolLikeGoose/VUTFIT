//
// Created by burluiva on 10/10/2023.
//

#include "parser.h"
#include "math.h"

void parser_init(Parser *parser) {
  assert(parser);
  parser->llParser = NULL;
  parser->expressionParser = NULL;
}

TokenType parser_get_current_token(TokenVector *tokenVector,
                                   const int *offset) {
  return *offset >= tokenVector->length ? UNDEFINED
                                        : tokenVector->data[*offset].type;
}

Error parser_eat(GrammarToken *grammarToken, TokenType tokenType,
                 TokenVector *tokenVector, int *curOffset) {
  if (tokenType == UNDEFINED)
    return error_create(NONE, NULL);

  if (*curOffset >= tokenVector->length) {
    return error_create(SYNTACTIC_ERROR, "Missing token!");
  }

  if (tokenVector->data[*curOffset].type == tokenType) {

    if (tokenType == DELIMITER) {
      while (*curOffset < tokenVector->length &&
             tokenVector->data[*curOffset].type == DELIMITER) {
        *curOffset = *curOffset + 1;
      }
      *curOffset = *curOffset - 1;
    }

    Token *token = malloc(sizeof(Token));
    if (token == NULL)
      return error_create(FATAL, "Out of memory!");
    token_init(token);

    *token = tokenVector->data[*curOffset];
    grammar_token_add(grammarToken, token);

    *curOffset = *curOffset + 1;
    return error_create(NONE, NULL);
  }
  return error_create(
      SYNTACTIC_ERROR,
      "Missing token!"); // TODO fix error message plus other fixes
}

/// Using precedence SA parse (mainly expressions)
/// \param parser parser
/// \param tokenPointer pointer to the token which should be initialized
/// \param tokens the vector of tokens
/// \param offset offset of vector
/// \return NONE in case everything is ok
Error precedence_parser_parse(Parser *parser, Token **tokenPointer,
                              TokenVector *tokens, int *offset);

/// uses ll grammar as main parser
Error parser_parse(Parser *parser, GrammarToken *grammarToken,
                   TokenVector *tokens, int *offset, TokenType finalToken,
                   ParserMode mode) {
  assert(parser && grammarToken && tokens && offset);

  int ruleId = ll_parser_get_rule(parser->llParser, finalToken,
                                  parser_get_current_token(tokens, offset));

  if (ruleId == -1 && parser_get_current_token(tokens, offset) == DELIMITER) {
    while (*offset < tokens->length &&
           parser_get_current_token(tokens, offset) == DELIMITER) {
      *offset = *offset + 1;
    }
    ruleId = ll_parser_get_rule(parser->llParser, finalToken,
                                parser_get_current_token(tokens, offset));
  }

  if (ruleId == -1 && mode == P_PARSER) {
    ruleId = ll_parser_get_rule(parser->llParser, finalToken, UNDEFINED);
  }

  if (ruleId == -1) {
    return error_create(
        SYNTACTIC_ERROR,
        "Could not continue parsing, no rule to continue..."); // TODO
  }

  grammarToken->grammarRuleId = ruleId;
  grammarToken->grammarRule =
      &(parser->llParser->llGrammar->grammarRules[ruleId]);

  GrammarRule *grammarRule =
      &(parser->llParser->llGrammar->grammarRules[ruleId]);

  Error err;
  for (int i = 0; i < grammarRule->productionsNumber; i++) {
    TokenType requiredTokenType = grammarRule->productions[i];
    if (requiredTokenType < NON_TERMINAL_UNDEFINED) {

      if (requiredTokenType != DELIMITER) {
        while (*offset < tokens->length &&
               tokens->data[*offset].type == DELIMITER) {
          *offset = *offset + 1;
        }
      }

      err = parser_eat(grammarToken, requiredTokenType, tokens, offset);
      if (err.errorType != NONE)
        return err;
    } else {
      if (requiredTokenType != E) {
        Token *token = malloc(sizeof(Token));

        if (token == NULL)
          return error_create(FATAL, "Out of memory!");
        token_init(token);

        token->type = requiredTokenType;
        token->data.grammarToken = malloc(sizeof(GrammarToken));

        if (token->data.grammarToken == NULL)
          return error_create(FATAL, "Out of memory!");
        grammar_token_init(token->data.grammarToken);
        grammar_token_add(grammarToken, token);

        err = parser_parse(parser, token->data.grammarToken, tokens, offset,
                           requiredTokenType, mode);
      } else {
        err = precedence_parser_parse(
            parser,
            &(grammarToken->tokensHolder[grammarToken->tokensHolderSize]),
            tokens, offset);
        grammarToken->tokensHolderSize++;
      }
      if (err.errorType != NONE)
        return err;
    }
  }

  return error_create(NONE, NULL);
}

Error precedence_parser_reduce(PParser *pParser, PParserItemVector *tokenStack);

/// uses precedence
Error precedence_parser_parse(Parser *parser, Token **tokenPointer,
                              TokenVector *tokens, int *offset) {
  assert(parser && tokenPointer && tokens && offset);
  int balance = 0;

  PParserItemVector stack;
  pparser_item_vector_init(&stack);

  Token dollarToken;
  token_init(&dollarToken);
  dollarToken.type = DOLLAR;

  PParserItem initItem;
  pparser_item_init(&initItem);

  initItem.token = &dollarToken;

  pparser_item_vector_push_back(&stack, initItem);

  bool lastWasF = false;
  while (true) {
    bool delimiterRemoved = false;
    while (*offset < tokens->length &&
           tokens->data[*offset].type == DELIMITER) {
      *offset = *offset + 1;
      delimiterRemoved = true;
    }

    Token *token = malloc(sizeof(Token));

    if (token == NULL) {
      free(stack.data);
      return error_create(FATAL, "Out of memory!");
    }
    token_init(token);

    token->type = parser->expressionParser->idRepresentative;
    token->data.grammarToken = malloc(sizeof(GrammarToken));

    if (token->data.grammarToken == NULL) {
      // free(token);
      free(stack.data);
      return error_create(FATAL, "Out of memory!");
    }

    grammar_token_init(token->data.grammarToken);

    int backupOffset = *offset;
    Error errLLParser = error_create(SYNTACTIC_ERROR, NULL);
    if (!lastWasF) {
      errLLParser = parser_parse(parser, token->data.grammarToken, tokens, offset,
                               token->type, P_PARSER);
    }

    if (errLLParser.errorType != NONE) {
      grammar_token_free(token->data.grammarToken);
      *offset = backupOffset;
      if (*offset < tokens->length) {
        *token = tokens->data[*offset];
        *offset = *offset + 1;
      } else {
        token->type = UNDEFINED;
      }
    }

    int foundTerminalPointer = stack.length - 1;
    while (foundTerminalPointer >= 0 &&
           !(precedence_parser_could_process(
               parser->expressionParser,
               stack.data[foundTerminalPointer].token->type)))
      foundTerminalPointer--;

    if (foundTerminalPointer < 0) {
      // TODO clean memory
      return error_create(FATAL,
                          "Could not find dollar sign during PParser...");
    }

    PParserItem item;
    pparser_item_init(&item);
    item.token = token;

    int closeType =
        parser->expressionParser->priorityTable[stack.data[foundTerminalPointer]
                                                    .token->type][token->type];
    if (closeType == 0) {
      *offset = backupOffset;
      if (delimiterRemoved || tokens->data[*offset-1].type == DELIMITER)
        *offset = *offset - 1;

      free(token);
      while (stack.length >= 2 && balance > 0) {
        stack.data[stack.length - 1].closed = 1;
        Error err = precedence_parser_reduce(parser->expressionParser, &stack);
        balance -= 1;
        if (err.errorType != NONE) {
          // TODO clean the memory used + unite errors
          free(stack.data);
          *offset = backupOffset;
          return err;
        }
      }
      if (stack.length != 2 || stack.data[1].token->type != E) {
        // TODO clean the memory + unite errors
        return error_create(SYNTACTIC_ERROR, "Parsing of expression failed");
      }
      *tokenPointer = stack.data[1].token;
      free(stack.data);
      return error_create(NONE, NULL);
    } else if (closeType == 1) {
      pparser_item_vector_push_back(&stack, item);
      assert(stack.data[foundTerminalPointer + 1].opened == 0);
      stack.data[foundTerminalPointer + 1].opened = 1;
      balance += 1;
    } else if (closeType == -1) {
      stack.data[foundTerminalPointer].closed = 1;
      balance -= 1;

      if (balance < 0) {
        // TODO clean memory?
        free(stack.data);
        return error_create(SYNTACTIC_ERROR, "Could not parse expression!");
      }

      Error err = precedence_parser_reduce(parser->expressionParser, &stack);
      if (err.errorType != NONE) {
        // TODO free memory
        return err;
      }
      *offset = backupOffset;
      continue;
    }

    lastWasF = (errLLParser.errorType == NONE);
  }

  free(stack.data);
  return error_create(NONE, NULL);
}

Error precedence_parser_reduce(PParser *pParser,
                               PParserItemVector *tokenStack) {

  int rightPointer = tokenStack->length - 1;
  int leftPointer = rightPointer;

  while (leftPointer >= 0 && !(tokenStack->data[leftPointer].opened))
    leftPointer--;

  if (leftPointer < 0 ||
      rightPointer - leftPointer + 1 > MAX_GRAMMAR_RULE_PRODUCTIONS_SIZE)
    return error_create(SYNTACTIC_ERROR, "Broken expression...");

  GrammarRule testRule;
  grammar_rule_init(&testRule);
  testRule.resultTokenType = pParser->resultTokenType;
  for (int i = leftPointer; i <= rightPointer; i++) {
    testRule.productions[testRule.productionsNumber++] =
        tokenStack->data[i].token->type;
  }

  int ruleId = findGrammarRule(pParser->pGrammar, &testRule);
  if (ruleId < 0)
    return error_create(SYNTACTIC_ERROR, "Could not find rule for parsing...");

  Token *reducedToken = malloc(sizeof(Token));
  if (reducedToken == NULL)
    return error_create(FATAL, "Out of memory!");
  reducedToken->data.grammarToken = malloc(sizeof(GrammarToken));
  if (reducedToken->data.grammarToken == NULL) {
    free(reducedToken);
    return error_create(FATAL, "Out of memory!");
  }

  reducedToken->data.grammarToken = malloc(sizeof(GrammarToken));
  if (reducedToken->data.grammarToken == NULL) {
    free(reducedToken);
    return error_create(FATAL, "Out of memory!");
  }
  grammar_token_init(reducedToken->data.grammarToken);

  reducedToken->type = pParser->pGrammar->grammarRules[ruleId].resultTokenType;
  reducedToken->data.grammarToken->grammarRuleId = ruleId;
  reducedToken->data.grammarToken->grammarRule =
      &(pParser->pGrammar->grammarRules[ruleId]);

  for (int i = leftPointer; i <= rightPointer; i++) {
    grammar_token_add(reducedToken->data.grammarToken,
                      tokenStack->data[i].token);
  }
  tokenStack->length = leftPointer;

  PParserItem reducedItem;
  pparser_item_init(&reducedItem);
  reducedItem.token = reducedToken;

  pparser_item_vector_push_back(tokenStack, reducedItem);
  return error_create(NONE, NULL);
}