/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude        --3--
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.								--2--
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole			--4--
 * znaků postfixExpression.														
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde    			--1--
 * nadeklarovat a používat pomocnou proměnnou typu char. 
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char lastChar; 							//1
	if (Stack_IsEmpty(stack)) return;		//Some error? If we have ')' so Stack cant be empty...

	while (!Stack_IsEmpty(stack)) { 		//2
		Stack_Top(stack, &lastChar);
		Stack_Pop(stack);                   //return from Pop() char instead of void? (So that we dont need Stack_Top() func)

		if (lastChar == '(') {				//3		
			break;
		}

		postfixExpression[(*postfixExpressionLength)++] = lastChar;   //4
	}
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	if (Stack_IsEmpty(stack)) {
		Stack_Push(stack, c);
		return;
	}

	char topChar;
	Stack_Top(stack, &topChar);
	
	//[..., '('] 
	//[..., '+'] < c = '/' 
	if (topChar == '(' || ((topChar == '+' || topChar == '-') && (c == '/' || c == '*'))) { 
		Stack_Push(stack, c);
		return;
	}

	//else, add topChar to postfixExpression and try to add c again
	Stack_Pop(stack);
	postfixExpression[(*postfixExpressionLength)++] = topChar;
	doOperation(stack, c, postfixExpression, postfixExpressionLength);
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající				
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {			
	unsigned postfixExpressionLength = 0;
	char *postfixExpression = malloc(MAX_LEN * sizeof(char));			//    	MAX_LEN - number of char's?
	if (postfixExpression == NULL) {									//		if malloc failed
		printf("Failed to allocate memory for 'postfixExpression'");	
		return NULL;
	}																	// 		Allocated: postfixExpression

	Stack *stack = malloc(sizeof(Stack));							
	if (stack == NULL) {												//		if malloc failed
		free(postfixExpression);
		printf("Failed to allocate memory for 'Stack'");
		return NULL;
	}																	// 		Allocated: postfixExpression, stack
	Stack_Init(stack);

	//--------------expression processing-------------//
	char c = *infixExpression;
	while (c != '\0') {
		switch (c)
		{
		case '(':
			Stack_Push(stack, c);
			break;

		case ')':
			untilLeftPar(stack, postfixExpression, &postfixExpressionLength);
			break;

		case '+': 														//1
		case '-':
		case '*':
		case '/':
			doOperation(stack, c, postfixExpression, &postfixExpressionLength);
			break;

		case '=':														// 		End of expression, start processing
			while (!Stack_IsEmpty(stack)) {
				Stack_Top(stack, &postfixExpression[postfixExpressionLength++]);
				Stack_Pop(stack);
			}
			postfixExpression[postfixExpressionLength++] = '=';  		//4		End of processing
			break;

		default:
			if (!(
				(c >= '0' && c <= '9') ||
				(c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z')
			)) {
				printf("Bad char in input!");							//2		Unachievable if point 3 is correct
				return NULL;
			}

			postfixExpression[postfixExpressionLength++] = c;
			break;
		}

		c = *(++infixExpression);
	}
	//----------end of expression processing----------//

	postfixExpression[postfixExpressionLength++] = '\0'; 				//		end of string 

	Stack_Dispose(stack);												//		free all memory
	free(stack);
	// free(postfixExpression);
	return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	//divide the number by bytes || divide the number by chars 
    for (int i = sizeof(int) - 1; i >= 0; i--)
    {
        char byte = (value >> (i * 8)) & 0xFF; 							//		shift by i * byte
        Stack_Push(stack, byte);
    }
	// printf("Stack size is %d\n", stack->topIndex+1);
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	*value = 0;
    char byte;
    for (unsigned i = 0; i < sizeof(int); i++)
    {
		Stack_Top(stack, &byte);
		Stack_Pop(stack);
        *value |= (byte & 0xFF) << (i * 8); 
    }
}


/// @brief Searches value by Key in Dictionary
/// @param c Key for searching
/// @param variableValues Dictionary 
/// @param variableValueCount Number of elements in dictionary
/// @return value from dictionary 
int findValue(char c, VariableValue variableValues[], int variableValueCount) {
	for (int i = 0; i < variableValueCount; i++) {
		if (variableValues[i].name == c) {
			// printf("Value of %c is %d\n", c, variableValues[i].value);
			return variableValues[i].value;
		}
	}

	printf("Value of '%c' not find in VariableValue variableValues[]", c);
	return -99;
}


/// @brief Provides calculation based on args and operands stored in stack
/// @param operation char symbol from ['-', '+', '/', '*']
/// @param stack stack where operands are stored
/// @return result of operation
int provideOperation(char operation, Stack *stack) {
	int op2 = 0;
	int op1 = 0;														//		get two operands from stack
	expr_value_pop(stack, &op2);
	expr_value_pop(stack, &op1);
	// printf("Operation: %d %c %d\n", op1, operation, op2);

	switch (operation)													// 		provide operation
	{
	case '+':
		return op1 + op2;
		break;
	
	case '-':
		return op1 - op2;
		break;
	
	case '*':
		return op1 * op2;
		break;
	
	case '/':
		return op1 / op2;
		break;
	}

	printf("Some error occured in provideOperation()");
	return -99;
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	char *postfixExpression = infix2postfix(infixExpression);			//		Get postfix expression
	char *originalAdress = postfixExpression;

	Stack *stack = malloc(sizeof(Stack));							
	if (stack == NULL) {												//		if malloc failed
		printf("Failed to allocate memory for 'Stack'");
		return false;
	}																	// 		Allocated: stack
	Stack_Init(stack);

	//--------------expression processing-------------//
	int val = 0;
	int result = 0;
	char c = *postfixExpression;
	while (c != '\0') {
		switch (c)
		{
		case '-':
		case '+':
		case '*':
		case '/':
			val = provideOperation(c, stack);
			// printf("Value after operation %d\n", val);
			expr_value_push(stack, val);								//		Push back to stack
			break;
		
		case '=':
			expr_value_pop(stack, value);
			break;

		default:
			result = findValue(c, variableValues, variableValueCount);	//		Get value from VariableValue
			expr_value_push(stack, result);								//		And push it onto the stack
			break;
		}
		
		c = *(++postfixExpression); 
	}
	//----------end of expression processing----------//
	Stack_Dispose(stack);												//		free all memory
	free(stack);
	free(originalAdress);
	return true;
}

/* Konec c204.c */
