#include "c202-test-utils.h"
#include <stdio.h>

void test_print_stack(Stack *stack)
{
	if (solved)
	{
		int maxIndex = stack->topIndex;
		if (stack->topIndex > STACK_SIZE - 1)
		{
			printf("Warning: stack->topIndex (value: %i) is out of range STACK_SIZE (%i).\n", stack->topIndex,
			       STACK_SIZE);
			maxIndex = STACK_SIZE - 1;
		}
		if (stack->array == NULL) {
			printf("STACK: NOT INITIALISED\n");
			return;
		}
		printf("STACK: BOTTOM [");
		for (int i = 0; i <= maxIndex; i++)
		{
			putchar(' ');
			putchar(stack->array[i]);
		}
		printf(" ] TOP\n");
	}
	else
	{
		printf("TASK NOT SOLVED.\n");
	}
}
