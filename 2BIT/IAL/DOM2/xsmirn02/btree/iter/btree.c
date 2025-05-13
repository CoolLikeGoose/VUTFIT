/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;                                               //    The pointer is now NULL, so we can determine 
}                                                             //      if the tree has already been initialized

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while(tree) {                                               //    Iterate while tree is not NULL
    if (tree->key == key) {                                   //    Check if we found node
      *value = tree->value;
      return true;
    }

    if (key > tree->key) tree = tree->right;                  //    If key bigger than current node then go to right tree
    else tree = tree->left;                                   //    If key smaller than current node then go to left tree
  }

  return false;                                               //    Node not found
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  while(*tree) {                                              //    Iterate while tree is not NULL
    if ((*tree)->key == key) {                                //    Check if we found node
      (*tree)->value = value;                                 //      and just rewrite value
      return;
    }

    // root = tree;                                              //    New potential parent
    if (key > (*tree)->key) tree = &(*tree)->right;           //    If key bigger than current node then go to right tree
    else tree = &(*tree)->left;                               //    If key smaller than current node then go to left tree
  }

  bst_node_t *newNode = malloc(sizeof(bst_node_t)); //TODO CHE//    IF we here that means we not found node with same key
  newNode->key = key;                                         //      Starting with create new node
  newNode->value = value;   
  newNode->left = NULL;
  newNode->right = NULL;                                      

  *tree = newNode;                                            //    create new node and insert in tree
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  while ((*tree)->right) tree = &(*tree)->right;              //    If current node is null -> this is rightmost
                     
  target->key = (*tree)->key;                                 //      replace target 
  target->value = (*tree)->value;

  free(*tree);                                                //    Delete rightmost
  *tree = (*tree)->left;                                      //    IF rightmost has left childs then first left child now is rightmost
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  while(*tree) {                                              //    Iterate while tree is not NULL or Searched value
    if ((*tree)->key == key) {                                //    Check if we found node
      break;
    }
    
    if (key > (*tree)->key) tree = &(*tree)->right;           //    If key bigger than current node then go to right tree
    else tree = &(*tree)->left;                               //    If key smaller than current node then go to left tree
  }
  if (*tree == NULL) return;                                  //    If node not found just exit from function

  if ((*tree)->left == NULL && (*tree)->right == NULL) {      //    Node has no childrens
    free(*tree);                                              //      just delete this node
    *tree = NULL;
    return;
  } 
  if ((*tree)->left != NULL && (*tree)->right != NULL) {      //    Node has two childrens
    bst_replace_by_rightmost(*tree, &(*tree)->left);          //      find the rightmost in left child
    return;
  }
                                                              //    Choose child to save
  bst_node_t *child = (*tree)->left != NULL ? (*tree)->left : (*tree)->right;
  free(*tree);
  *tree = child;                                              //    Replace current node with child
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack_bst;                                      //    Create and init stack
  stack_bst_init(&stack_bst);

  stack_bst_push(&stack_bst, *tree);                          //    Push the first(main node)
  while (!stack_bst_empty(&stack_bst)) {                      //    Iterate until stack is empty
    bst_node_t *node = stack_bst_pop(&stack_bst);             //    Process last added node

    if (node == NULL) continue;                               //    Node can be null(the parent node without child)

    stack_bst_push(&stack_bst, node->right);                  //    Push childs of this parent to later
    stack_bst_push(&stack_bst, node->left);                   //      to process them later

    free(node);                                               //    Free current node
  }

  *tree = NULL;                                               //    Return tree to start state
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  while(tree) {
    stack_bst_push(to_visit, tree->right);                    //    add right node to visit later
    bst_add_node_to_items(tree, items);                       //    print current node
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack_bst;                                      //    Create and init stack
  stack_bst_init(&stack_bst);

  bst_leftmost_preorder(tree, &stack_bst, items);             //    Start from root node
  while(!stack_bst_empty(&stack_bst))
    bst_leftmost_preorder(stack_bst_pop(&stack_bst), &stack_bst, items);     //    Process all nodes from stack
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree) { 
    stack_bst_push(to_visit, tree);                         //    Save all left nodes to later usage
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack_bst;                                      //    Create and init stack
  stack_bst_init(&stack_bst);

  bst_leftmost_inorder(tree, &stack_bst);                     //    Start from root node
  while(!stack_bst_empty(&stack_bst)){
    tree = stack_bst_pop(&stack_bst);                         //    Get nodes from stack and add to items
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, &stack_bst);            //    Iterate for right tree of node
  }
    
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  while(tree) {
    stack_bst_push(to_visit, tree);                           //    Push current node to stack
    stack_bool_push(first_visit, true);                       //    Is this our first visit?
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack_bst;                                      //    Create and init stack
  stack_bool_t stack_bool;
  stack_bst_init(&stack_bst);
  stack_bool_init(&stack_bool);

  bst_leftmost_postorder(tree, &stack_bst, &stack_bool);      //    Start from root node
  while(!stack_bst_empty(&stack_bst)) {
    tree = stack_bst_pop(&stack_bst);                         //    Get last visited node from stack

    if (stack_bool_pop(&stack_bool)) {                        //    If last visit in this node was first?
      stack_bst_push(&stack_bst, tree);                       //    then push this node again
      stack_bool_push(&stack_bool, false);                    //    Set current visit as not-first(second)
      bst_leftmost_postorder(tree->right, &stack_bst, &stack_bool);//   Run postorder by right tree of our current node
    } else {
      bst_add_node_to_items(tree, items);                     //    If we were in this node already 3 times, then we can add this to items
    }
  }
}
