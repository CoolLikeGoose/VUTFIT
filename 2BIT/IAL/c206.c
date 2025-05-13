/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() { //char* error
	// printf("*ERROR* The program has performed an illegal operation: %s.\n", error);
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->firstElement = NULL;
	list->lastElement = NULL;
	list->activeElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	if (list == NULL) return;

	DLLElementPtr nextElem = list->firstElement;
	while (nextElem != NULL) {													//		Clean all elements from first
		nextElem = nextElem->nextElement;
		free(list->firstElement);
		list->firstElement = nextElem;
	}

	list->firstElement = NULL;													//  	And clear all pointers
	list->lastElement = NULL;
	list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr element = malloc(sizeof(struct DLLElement));
	if (element == NULL) {
		// DLL_Error("Malloc for 'element' in 'DLL_InsertFirst' failed");
		DLL_Error();
		return;
	}
	
	element->data = data;														//		Set Data
	element->nextElement = list->firstElement;									//		Link this element to first element
	element->previousElement = NULL;											//		This is first element from now

	if (list->firstElement == NULL) list->lastElement = element;				//		if it is the only element of the sequence, it is also the last one
	else list->firstElement->previousElement = element;							// 		Or just link old-firstElement to new-firstElement

	list->firstElement = element;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr element = malloc(sizeof(struct DLLElement));					// 		Almost same way as in DLL_InsertFirst
	if (element == NULL) {
		// DLL_Error("Malloc for 'element' in 'DLL_InsertLast' failed");
		DLL_Error();
		return;
	}
	
	element->data = data;														//		Set Data
	element->nextElement = NULL;												//		Link this element to first element
	element->previousElement = list->lastElement;								//		This is first element from now

	if (list->lastElement == NULL) list->firstElement = element;				//		if it is the only element of the sequence, it is also the first one
	else list->lastElement->nextElement = element;								// 		Or just link old-lastElement to new-lastElement

	list->lastElement = element;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list == NULL || list->firstElement == NULL) 
		// DLL_Error("List are empty or doesnt exist");
		DLL_Error();
	else
		*dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list == NULL || list->lastElement == NULL) 
		// DLL_Error("List are empty or doesnt exist");
		DLL_Error();
	else
		*dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.											  --1--
 * Pokud byl první prvek aktivní, aktivita se ztrácí.						  --2--
 * Pokud byl seznam list prázdný, nic se neděje.							  --3--
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list == NULL || list->firstElement == NULL) return;					   	//3

	if (list->activeElement == list->firstElement) list->activeElement = NULL;	//2

	if (list->firstElement->nextElement != NULL) {								//		if in the list more than 1 element
		list->firstElement = list->firstElement->nextElement;					//		move second element on firstPos
		free(list->firstElement->previousElement);								//		free the old-firstElement
		list->firstElement->previousElement = NULL;								//		delete pointer to old-firstElement
	} 
	else {																		//		if it was only element in list
		free(list->firstElement);												//		free the old-firstElement
		list->firstElement = NULL;												//		delete pointers to the firstElement
		list->lastElement = NULL;
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {											// 		Almost same way as in DLL_InsertFirst
	if (list == NULL || list->lastElement == NULL) return;					   	//3

	if (list->activeElement == list->lastElement) list->activeElement = NULL;	//2

	if (list->lastElement->previousElement != NULL) {							//		if in the list more than 1 element
		list->lastElement = list->lastElement->previousElement;					//		move pre-last element on lastPos
		free(list->lastElement->nextElement);									//		free the old-lastElement
		list->lastElement->nextElement = NULL;									//		delete pointer to old-lastElement
	} 
	else {																		//		if it was only element in list
		free(list->lastElement);												//		free the old-lastElement
		list->firstElement = NULL;												//		delete pointers to the firstElement
		list->lastElement = NULL;
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.								  --1--
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek				  --2--
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if (list == NULL ||
		list->activeElement == NULL ||
		list->activeElement == list->lastElement) return;						//2

	DLLElementPtr elToDelete = list->activeElement->nextElement;				//		save temp var
	
	if (elToDelete == list->lastElement) {										//		if next element is the last then
		list->lastElement = list->activeElement;								//		set lastElement of list to current active
		list->activeElement->nextElement = NULL;								//		delete pointer to the next element 
	} else {
		elToDelete->nextElement->previousElement = list->activeElement;			//		Otherwise connect next-next to active element 
		list->activeElement->nextElement = elToDelete->nextElement;				//		and connect active to next-next element
	}
	
	free(elToDelete);															// 		And finally free the element
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if (list == NULL ||															// 		Same as before
		list->activeElement == NULL ||
		list->activeElement == list->firstElement) return;						//2

	DLLElementPtr elToDelete = list->activeElement->previousElement;			//		save temp var
	
	if (elToDelete == list->firstElement) {										//		if previous element is the first then
		list->firstElement = list->activeElement;								//		set firstElement of list to current active
		list->activeElement->previousElement = NULL;							//		delete pointer to the previous element 
	} else {
		elToDelete->previousElement->nextElement = list->activeElement;			//		Otherwise connect previous-previous to active element 
		list->activeElement->previousElement = elToDelete->previousElement;		//		and connect active to previous-previous element
	}
	
	free(elToDelete);															// 		And finally free the element
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if (list == NULL ||
		list->activeElement == NULL) return;

	DLLElementPtr elToAdd = malloc(sizeof(struct DLLElement));					// 		Create new element
	if (elToAdd == NULL) {
		// DLL_Error("Malloc for 'elToAdd' in 'DLL_InsertAfter' failed");
		DLL_Error();
		return;
	}

	elToAdd->data = data;
	elToAdd->nextElement = list->activeElement->nextElement;
	elToAdd->previousElement = list->activeElement;
	
	if (list->activeElement == list->lastElement) {								// 		If active element is the last
		list->lastElement = elToAdd;											//		change last element of list to jsut added
	} else {
		list->activeElement->nextElement->previousElement = elToAdd;			//		Otherwise connect next node to just added
	}	
	list->activeElement->nextElement = elToAdd;									//		connect new node to active
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if (list == NULL ||
		list->activeElement == NULL) return;

	DLLElementPtr elToAdd = malloc(sizeof(struct DLLElement));					// 		Create new element
	if (elToAdd == NULL) {
		// DLL_Error("Malloc for 'elToAdd' in 'DLL_InsertBefore' failed");
		DLL_Error();
		return;
	}

	elToAdd->data = data;
	elToAdd->nextElement = list->activeElement;
	elToAdd->previousElement = list->activeElement->previousElement;

	if (list->activeElement == list->firstElement) {							// 		If active element is the first
		list->firstElement = elToAdd;											//		change first element of list to jsut added
	} else {
		list->activeElement->previousElement->nextElement = elToAdd;			//		Otherwise connect previous node to just added
	}	
	list->activeElement->previousElement = elToAdd;								//		connect new node to active
}



//	Next functions are pretty easy, so I dont think that I have to comment them
//	because their behavior already is In description of functions 



/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list == NULL || list->activeElement == NULL) {
		// DLL_Error("List are empty or there are no active element");
		DLL_Error();
		return;
	}

	*dataPtr = list->activeElement->data; 
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement == NULL) return;
	list->activeElement->data = data; 
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement == NULL) return;
	list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement == NULL) return;
	list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement == NULL ? 0 : 1; 
}

/* Konec c206.c */
