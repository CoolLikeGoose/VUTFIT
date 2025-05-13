#include "c202.h"
#include "c202-test-utils.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_COUNT ((long int) (sizeof(tests) / sizeof(void *)))


TEST(test_init, "Initialization")
	Stack_Init(test_stack);

	test_print_stack(test_stack);
ENDTEST

TEST(test_push_empty, "Stack push after initialization")
	Stack_Init(test_stack);
	test_print_stack(test_stack);

	Stack_Push(test_stack, 'a');
	test_print_stack(test_stack);
ENDTEST

TEST(test_push_multiple, "Multiple stack pushes")
	Stack_Init(test_stack);
	test_print_stack(test_stack);

	Stack_Push(test_stack, 'a');
	Stack_Push(test_stack, 'b');
	Stack_Push(test_stack, 'c');
	test_print_stack(test_stack);
ENDTEST

TEST(test_pop, "Stack pop")
	Stack_Init(test_stack);
	Stack_Push(test_stack, 'a');
	test_print_stack(test_stack);

	Stack_Pop(test_stack);
	test_print_stack(test_stack);
ENDTEST

TEST(test_pop_multiple, "Multiple stack pops")
	Stack_Init(test_stack);
	Stack_Push(test_stack, 'a');
	Stack_Push(test_stack, 'b');
	Stack_Push(test_stack, 'c');
	Stack_Push(test_stack, 'd');
	Stack_Push(test_stack, 'e');
	test_print_stack(test_stack);

	Stack_Pop(test_stack);
	Stack_Pop(test_stack);
	Stack_Pop(test_stack);
	test_print_stack(test_stack);
ENDTEST

TEST(test_top, "Stack top")
	Stack_Init(test_stack);
	Stack_Push(test_stack, 'a');
	test_print_stack(test_stack);

	char top;
	Stack_Top(test_stack, &top);
	printf("Char on the top: %c\n", top);
	test_print_stack(test_stack);
ENDTEST

TEST(test_top_full, "Stack top of full stack")
	Stack_Init(test_stack);
	for (int i = 0; i < STACK_SIZE; i++)
	{
		Stack_Push(test_stack, '0' + i);
	}
	test_print_stack(test_stack);

	char top;
	Stack_Top(test_stack, &top);
	printf("Char on the top: %c\n", top);
	test_print_stack(test_stack);
ENDTEST

TEST(test_empty, "Stack is empty after initialization")
	Stack_Init(test_stack);

	test_print_stack(test_stack);
	printf("Is stack empty? %s\n", Stack_IsEmpty(test_stack) ? "Yes" : "No");
ENDTEST

TEST(test_not_empty, "Stack is not empty after pushing")
	Stack_Init(test_stack);
	Stack_Push(test_stack, 'a');

	test_print_stack(test_stack);
	printf("Is stack empty? %s\n", Stack_IsEmpty(test_stack) ? "Yes" : "No");
ENDTEST

TEST(test_not_empty_full, "Stack is not empty after filling")
	Stack_Init(test_stack);
	for (int i = 0; i < STACK_SIZE; i++)
	{
		Stack_Push(test_stack, '0' + i);
	}

	test_print_stack(test_stack);
	printf("Is stack empty? %s\n", Stack_IsEmpty(test_stack) ? "Yes" : "No");
ENDTEST

TEST(test_not_full_init, "Stack is not full after initialization")
	Stack_Init(test_stack);

	test_print_stack(test_stack);
	printf("Is stack full? %s\n", Stack_IsFull(test_stack) ? "Yes" : "No");
ENDTEST

TEST(test_not_full, "Stack is not full after pushing")
	Stack_Init(test_stack);
	Stack_Push(test_stack, 'a');

	test_print_stack(test_stack);
	printf("Is stack full? %s\n", Stack_IsFull(test_stack) ? "Yes" : "No");
ENDTEST

TEST(test_full, "Stack is full after filling")
	Stack_Init(test_stack);
	for (int i = 0; i < STACK_SIZE; i++)
	{
		Stack_Push(test_stack, '0' + i);
	}

	test_print_stack(test_stack);
	printf("Is stack full? %s\n", Stack_IsFull(test_stack) ? "Yes" : "No");
ENDTEST

TEST(test_dispose, "Dispose stack")
	Stack_Init(test_stack);
	for (int i = 0; i < STACK_SIZE; i++)
	{
		Stack_Push(test_stack, 'a' + i % 26);
	}
	test_print_stack(test_stack);

	Stack_Dispose(test_stack);
	test_print_stack(test_stack);
ENDTEST


void (*tests[])(void) = {
		test_init,
		// multiple
		test_push_empty,
		test_push_multiple,
		// pop
		test_pop,
		test_pop_multiple,
		// top
		test_top,
		test_top_full,
		// empty
		test_empty,
		test_not_empty,
		test_not_empty_full,
		// full
		test_not_full_init,
		test_not_full,
		test_full,
		// dispose
		test_dispose,
};

int main( int argc, char *argv[] ) {
	if (argc > 2)
	{
		fprintf(stderr, "Usage: %s [test_id]\n", argv[0]);
		return 1;
	}

	char *test_id_reminder = NULL;
	long test_id;
	if (argc == 2)
	{
		test_id = strtol(argv[1], &test_id_reminder, 10);
		if (test_id_reminder[0] != 0)
		{
			fprintf(stderr, "Usage: %s {test_id}\n", test_id_reminder);
			fprintf(stderr, "Unexpected test_id: %s\n", test_id_reminder);
			return 1;
		}

		if (test_id < 0)
		{
			test_id = TEST_COUNT + test_id;
		}

		if (test_id + 1 > TEST_COUNT)
		{
			fprintf(stderr, "Unknown test: %ld (test count: %ld)\n", test_id, TEST_COUNT);
			return 1;
		}

		tests[test_id]();
	}
	else
	{
		printf("C202 - Stack Implemented Using an Array, Basic Tests\n");
		printf("====================================================\n");

		for (test_id = 0; test_id < TEST_COUNT; test_id++)
		{
			tests[test_id]();
		}
	}

	return 0;
}
