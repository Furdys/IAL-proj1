/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

	// Set default values to list attributes
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	struct tDLElem *element = L->First;			// Set current element to the first of the list
	struct tDLElem *nextElement;
	
	while(element != NULL)									// For each element in the list
	{
		nextElement = element->rptr; 					// Get pointer of the next element
		free(element); 												// Free the memory of the current element
		element = nextElement; 								// Move to next element
	}

	// Set default values to list attributes once again
	L->First = NULL;
	L->Last = NULL;
	L->Act = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**
**/

	struct tDLElem *wasFirst = L->First;															// The element that used to be first
	struct tDLElem *newElement = malloc(sizeof(struct tDLElem));			// Allocate memory for new element
	
	if(newElement == NULL)			// If allocation wasn't successful
	{
		DLError();								// Throw error
		return;
	}
		
	newElement->data = val;							// Set value of the new element
	newElement->lptr = NULL;						// There is no left element if this one is the first in the list
	newElement->rptr = wasFirst;				// Link the element that used to be first to the right from the new one
	
	if(wasFirst == NULL)								// If it is was empty list
		L->Last = newElement;							// New element is both first and last
	else
		wasFirst->lptr = newElement;			// Link the new element to the left from the previous first element 
		
	L->First = newElement;							// Change list attribute to point to the new first element
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	

	struct tDLElem *wasLast = L->Last;																// The element that used to be last
	struct tDLElem *newElement = malloc(sizeof(struct tDLElem));			// Allocate memory for new element
	
	if(newElement == NULL)			// If allocation wasn't successful
	{
		DLError();								// Throw error
		return;
	}
		
	newElement->data = val;							// Set value of the new element
	newElement->lptr = wasLast;					// Link the element that used to be last to the left from the new one
	newElement->rptr = NULL;						// There is no right element if this one is the last in the list
	
	if(wasLast == NULL)								// If it is was empty list
		L->First = newElement;					// New element is both first and last
	else
		wasLast->rptr = newElement;			// Link the new element to the right from the previous last element 
		
	L->Last = newElement;							// Change list attribute to point to the new last element
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

	L->Act = L->First;		// Mark the first element as active
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;			// Mark the last element as active
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First == NULL)			// If the list is empty
	{
		DLError();							// Throw an error
		return;
	}
	
	*val = L->First->data; // Put value of the first element into 'val'
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->Last == NULL)				// If the list is empty
	{
		DLError();							// Throw an error
		return;
	}
	
	*val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

	if(L->First == NULL)														// If the list is empty
		return;

	if(L->Act == L->First)													// If activate element is about to be destroyed
		L->Act = NULL;																// Disable the active attribut

	struct tDLElem *newFirst = L->First->rptr;			// Element that is going to be first
	free(L->First);																	// Free the memory of deleted element
	L->First = newFirst;														// Change the list attribute poining to first element
	
	if(newFirst == NULL)														// If there is not any element left after the deletion
	{
		L->First = NULL;															// Disable list attribute pointing to first element	
		L->Last = NULL;																// Disable list attribute pointing to last element	
	}
	else
		newFirst->lptr = NULL;												// There is not any element on the left side from the new first
	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 

	if(L->Last == NULL)														// If the list is empty
		return;
	
	if(L->Act == L->Last)													// If active element is about to be destroyed
		L->Act = NULL;															// Disable the active attribut

	struct tDLElem *newLast = L->Last->lptr;			// Element that is going to be last
	free(L->Last);																// Free the memory of deleted element
	L->Last = newLast;														// Change the list attribute poining to last element
	
	
	if(newLast == NULL)														// If there is not any element left after the deletion
	{
		L->First = NULL;														// Disable list attribute pointing to first element	
		L->Last = NULL;															// Disable list attribute pointing to last element	
	}
	else
		newLast->rptr = NULL;												// There is not any element on the right side from the new last
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

	if(L->Act == NULL || L->Last == L->Act)					// If there is no active element or the active is also last one
		return;
		
	struct tDLElem *delElement = L->Act->rptr;			// Element to be deleted
	
	if(delElement->rptr == NULL)										// If the deleted element was last in the list
	{
		L->Last =	L->Act;															// Change the list attribute poining to last element
		L->Last->rptr =	NULL;													// TO-DO WATCHOUT!!! CHECK LATER!!! EVERYTIME YOU CHANGE LAST OR FIRST BE SURE TO DESTROY L/R POINTER IN ELEMENT
	}
		
	L->Act->rptr = delElement->rptr; 								// Connect the active element to the next after the deleted element
	free(delElement);
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	
	if(L->Act == NULL || L->First == L->Act)				// If there is no active element or the active is also first one
		return;
		
	struct tDLElem *delElement = L->Act->lptr;			// Element to be deleted
	
	if(delElement->lptr == NULL)										// If the deleted element was last in the list
	{
		L->First =	L->Act;														// Change the list attribute poining to first element
		L->First->lptr = NULL;												// TO-DO WATCHOUT!!! CHECK LATER!!! EVERYTIME YOU CHANGE LAST OR FIRST BE SURE TO DESTROY L/R POINTER IN ELEMENT
	}
		
	L->Act->lptr = delElement->lptr;								// Connect the active element to the next before the deleted element
	free(delElement);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	if(L->Act != NULL)																									// If there is an active element
	{
		struct tDLElem *newElement = malloc(sizeof(struct tDLElem));			// Allocate memory for new element
	
		if(newElement == NULL)																						// If the allocation wasn't successful
		{
			DLError(); 																											// Throw an error
			return;
		}
		
		// Set attributes for new element
		newElement->data = val; 
		newElement->lptr = L->Act;
		newElement->rptr = L->Act->rptr;

		// Change attributes for neighbour elements
		L->Act->rptr = newElement;									// Link element on the right (active) to the new element
		if(newElement->rptr != NULL) 								// If there is an element on the right from the new one
			newElement->rptr->lptr = newElement; 			// Link it to the new element
		else
			L->Last = newElement; 										// The new element is also the last one
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	if(L->Act != NULL)																									// If there is an active element
	{
		struct tDLElem *newElement = malloc(sizeof(struct tDLElem));			// Allocate memory for new element
	
		if(newElement == NULL)																						// If the allocation wasn't successful
		{
			DLError(); 																											// Throw an error
			return;
		}
		
		// Set attributes for new element
		newElement->data = val; 
		newElement->lptr = L->Act->lptr;
		newElement->rptr = L->Act;
		
		// Change attributes for neighbour elements
		L->Act->lptr = newElement;									// Link element on the right (active) to the new element
		if(newElement->lptr != NULL) 								// If there is an element on the left side from the new one
			newElement->lptr->rptr = newElement; 			// Link it to the new element
		else
			L->First = newElement; 										// The new element is also the first one
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	
	if(L->Act == NULL)
	{
		DLError();
		return;
	}
	
	*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
	if(L->Act != NULL)
	{
		L->Act->data = val; 
	}
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

	if(L->Act != NULL)
	{
		L->Act = L->Act->rptr; 
	}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

	if(L->Act != NULL)
	{
		L->Act = L->Act->lptr; 
	}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act == NULL) ? 0 : 1;
}

/* Konec c206.c*/
