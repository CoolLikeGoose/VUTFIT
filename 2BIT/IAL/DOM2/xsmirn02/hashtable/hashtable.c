/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;                                       //   Set all items in table to NULL 
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {        
  ht_item_t *item = (*table)[get_hash(key)];                  //    Get item from table by key's hash

  while (item != NULL) {                                      //    Iterate in linked list of items
    if (item->key == key) return item;                        //    If we found item with our key
    item = item->next;                                        //    Otherwise get next item from linked list
  }

  return NULL;                                                //    item now is NULL(not found)
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tazbulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    ht_item_t *item = ht_search(table, key);                  //    Search if we already have item with same key 
    if (item != NULL) {                                       //    If we found item 
      item->value = value;                                    //    Just change value of item
      return;
    }

    item = malloc(sizeof(ht_item_t));                         //    Otherwise create new item
    item->key = key;
    item->value = value;
    item->next = (*table)[get_hash(key)];                     //    Insert new item on first position of our linked list
    (*table)[get_hash(key)] = item;                           //    Set new item in table
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key);                    //    Search item in table
  
  if (item == NULL) return NULL;                              //    If key isnt in table return NULL
  return &(item->value);                                      //    Otherwise return value of item
} 

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  ht_item_t *item = (*table)[get_hash(key)];                  //    Get table item by hash    
  ht_item_t *prev = NULL;                                     //    Prepare previous item

  while (item != NULL) {                                      //    Iterate throught current synonimous
    if (item->key == key) {                                   //    If we found key, then:
      if (prev != NULL) {                                     //    If current item isnt first
        prev->next = item->next;                              //      just link previous item to next item
      } else {                                                //    If current item is first in linked list
        (*table)[get_hash(key)] = item->next;                 //      just replace link in table item to next synonym
      }
      free(item);
      return;                                                 //    Element deleted
    }
    prev = item;                                              //    If key not find, move poointers to right by 1 synonym
    item = item->next;
  }                                                           //    If element doesnt found, just exit from function
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++) {                         //    Iterate throught the table
    ht_item_t *item = (*table)[i];                            //    get current item

    while(item != NULL) {                                     //    Iterate throught linked list in this item of table
      ht_item_t *next = item->next;                           //    Save link to next element before delete current                
      free(item);                                             //    Delete current element
      item = next;                                            //    Next element of linked list is now current
    }

    (*table)[i] = NULL;                                       //    Set current item of table to NULL (same as ht_init)
  }
}
