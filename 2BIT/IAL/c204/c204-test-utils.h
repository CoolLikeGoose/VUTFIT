#ifndef C204_TEST_H
#define C204_TEST_H

#include "c204.h"
#include <stdio.h>

#define TEST(NAME, DESCRIPTION)                                                \
void NAME(void) {                                                              \
	printf("\n[%s] %s\n", #NAME, DESCRIPTION);                                 \
    solved = true;                                                             \
	char *expr;

#define ENDTEST                                                                \
	printf("\n");                                                              \
}

#define PRINT_EXPR_EVAL_CONFIG()                                               \
int result, valuesCount = (int) sizeof(values) / sizeof(VariableValue *);      \
printf("expression : %s\n", expr);                                             \
printf("variables  : ");                                                       \
for (int i = 0; i < valuesCount; ++i)                                          \
{                                                                              \
	if (i != 0) printf(", ");                                                  \
	printf("%c = %d", values[i].name, values[i].value);      \
}                                                                              \
printf("\n")


#define FAIL(REASON) printf("[FAILED] %s", REASON)

bool convert( char* infExpr, char **postExpr );

void verify( char *postExpr, char *postExprOk );

#endif
