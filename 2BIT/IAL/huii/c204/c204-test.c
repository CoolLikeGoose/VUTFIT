#include "c204.h"
#include "c204-test-utils.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_COUNT ((long int) (sizeof(tests) / sizeof(void *)))


TEST(test_01, "Upper and lower case characters with plus operator")
	if (convert("a+B=", &expr))
	{
		verify(expr, "aB+=");
	}
ENDTEST

TEST(test_02, "Digits with minus operator")
	if (convert("0-1=", &expr))
	{
		verify(expr, "01-=");
	}
ENDTEST

TEST(test_03, "Mixed operands with multiplication operator")
	if (convert("a*0=", &expr))
	{
		verify(expr, "a0*=");
	}
ENDTEST

TEST(test_04, "Mixed operands with division operator")
	if (convert("B/1=", &expr))
	{
		verify(expr, "B1/=");
	}
ENDTEST

TEST(test_05, "Simple parentheses support")
	if (convert("(a+b)=", &expr))
	{
		verify(expr, "ab+=");
	}
ENDTEST

TEST(test_06, "Expression evaluation from the left to the right")
	if (convert("a+b+c=", &expr))
	{
		verify(expr, "ab+c+=");
	}
ENDTEST

TEST(test_07, "Minus operator does not have higher priority than plus")
	if (convert("a+b-c=", &expr))
	{
		verify(expr, "ab+c-=");
	}
ENDTEST

TEST(test_08, "Plus operator does not have higher priority than minus")
	if (convert("A-B+C=", &expr))
	{
		verify(expr, "AB-C+=");
	}
ENDTEST

TEST(test_09, "Plus operator does not have higher priority than minus")
	if (convert("1*2/3=", &expr))
	{
		verify(expr, "12*3/=");
	}
ENDTEST

TEST(test_10, "Parentheses change operator priority")
	if (convert("A+(B-c)=", &expr))
	{
		verify(expr, "ABc-+=");
	}
ENDTEST

TEST(test_eval_01, "Evaluate expression")
	STACK_SIZE = 128;
	expr = "(a+b)*(c-d)=";
	VariableValue values[] = {
			{'a', 5},
			{'b', 5},
			{'c', 12},
			{'d', 2},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_02, "Evaluate expression")
	STACK_SIZE = 128;
	expr = "(a-b)*C=";
	VariableValue values[] = {
			{'a', 3},
			{'b', 5},
			{'C', -5},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_03, "Evaluate expression")
	STACK_SIZE = 128;
	expr = "a*b+c/d=";
	VariableValue values[] = {
			{'a', 10},
			{'b', 3},
			{'c', 100},
			{'d', 5},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST


void (*tests[])(void) = {
		test_01,
		test_02,
		test_03,
		test_04,
		test_05,
		test_06,
		test_07,
		test_08,
		test_09,
		test_10,
		test_eval_01,
		test_eval_02,
		test_eval_03,
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
		printf("C204 - Infix to Postfix Expression Conversion, Basic Tests\n");
		printf("==========================================================\n");

		for (test_id = 0; test_id < TEST_COUNT; test_id++)
		{
			tests[test_id]();
		}
	}

	return 0;
}

/* Konec c204-test.c */
