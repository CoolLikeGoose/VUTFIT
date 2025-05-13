#include "c204-test-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

bool solved;

bool convert( char* infExpr, char **postExpr ) {
	solved = true;

	/* Print expression to be convert */
	printf("Provided infix  : %s\n", infExpr);

	/* Perform conversion */
	*postExpr = infix2postfix(infExpr);

	/* Has function already started to be solved? */
	if (!solved)
	{
		printf("[W] Function infix2postfix hasn't been implemented yet.\n\n");
		return false;
	}

	return true;
}

void verify( char *postExpr, char *postExprOk ) {
	/* Arguments assertation */
	assert(postExpr != NULL);
	assert(postExprOk != NULL);
	assert(strlen(postExpr) <= MAX_LEN);
	assert(strlen(postExprOk) <= MAX_LEN);

	/* Test postfix string */
	if (postExpr == NULL)
	{
		/* bad allocation or bad pointer returned */
		printf("ERROR: NULL pointer returned.\n   You may have forgotten to return a correct pointer or\n   an error inside infix2postfix() occured.\n\n");
	}
	else
	{
		/* string is probably allocated */
		/* Try to find zero character stored for future test */
		char lastChar = postExpr[MAX_LEN - 1];
		/* just in case to have properly terminated string */
		postExpr[MAX_LEN - 1] = 0;

		if (strlen(postExpr) >= (MAX_LEN - 1) && lastChar != '0')
		{
			/* charater array doesn't contain zero character,
			 * this case probably never happens (zero character is often present in unitialized array) */
			printf("ERROR: String appears to be too long (exceeds %d characters).\n", MAX_LEN - 1);
			printf("   Haven't you forgotten to include zero character?\n\n");
		}
		else
		{
			/* string is OK in C language meaning */
			/* Print converted expression and conversion result */
			printf("Expected postfix: %s\n", postExpr);

			/* Test strings match */
			if (strcmp(postExpr, postExprOk) != 0)
			{
				/* conversion test failed */
				printf("Result postfix  : %s\n", postExpr);
				printf("Conversion match: FAILED\n\n");
			}
			else
			{
				/* conversion is finally OK */
				printf("Conversion match: OK\n\n");
			}
		}

		/* free memory */
		free(postExpr);
	}
}
