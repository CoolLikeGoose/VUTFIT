/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/// @brief Function increase counter of letter if char already in tree or adds char with value 1 
/// @param tree 
/// @param currChar 
void add_char_toBST(bst_node_t **tree, char currChar) {
    int count = 0;
    if (bst_search(*tree, currChar, &count))                    //      If founded then value++;
        bst_insert(tree, currChar, count+1);
    else 
        bst_insert(tree, currChar, 1);                          //      Or just add char to the tree
}
/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);

    while (*input) {                                                    //      Iterate throught input
        char currChar = tolower(*input);                                //      case insensitive
        if ((currChar >= 'a' && currChar <= 'z') || currChar == ' ') {  //      Compares chars
            add_char_toBST(tree, currChar);                             //      If char is letter or space then add this char
        } else {        
            add_char_toBST(tree, '_');                                  //      Otherwise add '_'
        }

        input++;
    }
}

//function from test_utils.c
bst_items_t* bst_init_items_own() {
  bst_items_t* items = malloc(sizeof(bst_items_t));
  items->capacity = 0;
  items->nodes = NULL;
  items->size = 0;
  return items;
}
void bst_print_node_own(bst_node_t *node) {
  printf("[%c,%d]", node->key, node->value);
}
/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
// in this function we use inorder because this way gives an array with numbers sorted in 
void bst_balance(bst_node_t **tree) {
    bst_items_t *items = bst_init_items_own();                  //    Init items
    bst_inorder(*tree, items);                                  //    Get InOrder

    int itemsSize = items->size;                                //    Get itemsSize so we do not need to get items->size every time
    
    bst_node_t *output = malloc(sizeof(bst_node_t));            //    Init output tree
    bst_init(&output);
    if (itemsSize % 2 == 1) {                                   //    IF we have odd number of items, we need to add median firstly
        bst_insert(&output, items->nodes[itemsSize/2]->key, items->nodes[itemsSize/2]->value);
    }
    for (int i = itemsSize/2-1; i >= 0; i--) {                   //    Add items to new tree from center [1, 2, 3, 4, 5]
        bst_insert(&output, items->nodes[i]->key, items->nodes[i]->value);           //                      |     |  
        bst_insert(&output, items->nodes[itemsSize-i-1]->key, items->nodes[itemsSize-i-1]->value);  //  one pointer(|) moves backwards and one forward
    }

    bst_dispose(tree);                                          //    In the end we need to dispose initial tree
    *tree = output;                                             //    And change pointer to new tree
}