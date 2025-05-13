#ifndef C202_TEST_H
#define C202_TEST_H

#include "c202.h"
#include <stdio.h>

#define TEST(NAME, DESCRIPTION)                                                \
void NAME(void) {                                                              \
	printf("\n[%s] %s\n", #NAME, DESCRIPTION);                                 \
	error_flag = false;                                                        \
    solved = true;                                                             \
    STACK_SIZE = 8;                                                            \
	Stack _stack;                                                              \
	Stack *test_stack = &_stack;

#define ENDTEST                                                                \
    Stack_Dispose(test_stack);                                                 \
	printf("\n");                                                              \
}

#define FAIL(REASON) printf("[FAILED] %s", REASON)

void test_print_stack(Stack *);

#endif
