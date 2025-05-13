/* ******************************* c204.h *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */

/* TENTO SOUBOR, PROSÍME, NEUPRAVUJTE! */

#ifndef _INFIX2POSTFIX_H_
#define _INFIX2POSTFIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Využívá příklad c202 - zásobník. */
#ifndef STUDENT_BUILD
#include "../c202/c202.h"
#else
#include "c202.h"
#endif

/** Maximální délka řetězce výrazu. */
#define MAX_LEN 64

/** Globální proměnná - indikuje, zda byla operace řešena. */
extern bool solved;

typedef struct var_value_t {
	char name;
	int value;
} VariableValue;

char *infix2postfix( const char *infixExpression );

bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value );

#endif

/* Konec hlavičkového souboru c204.h */
