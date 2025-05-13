/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) return false;                             //    Not found
  if (tree->key == key) {                                     //    Found and return
    *value = tree->value;
    return true;
  }

  if (key < tree->key) return bst_search(tree->left, key, value);       //    If key smaller than current node then go to left tree
  return bst_search(tree->right, key, value);                           //    If key bigger than current node then go to right tree
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (*tree == NULL) {                                                  //    If there is no node then we can insert new node
    bst_node_t *newNode = malloc(sizeof(bst_node_t)); //TODO CHECK    
    newNode->key = key;
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    *tree = newNode;                                                    //    create new node and insert in tree
    return;
  }

  if (key < (*tree)->key) {                                             //    If key smaller than current node then go to left tree
    bst_insert(&(*tree)->left, key, value);
  } else if (key > (*tree)->key) {                                      //    If key bigger than current node then go to right tree
    bst_insert(&(*tree)->right, key, value);                            
  } else {                                                              //    If we found same key
    (*tree)->value = value;                                             //      just change value
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree)->right == NULL) {                               //    If current node is null -> this is rightmost
    target->key = (*tree)->key;                               //      replace target 
    target->value = (*tree)->value;

    free(*tree);                                              //    Delete rightmost
    *tree = (*tree)->left;                                    //    IF rightmost has left childs then first left child now is rightmost
    return;
  }
  bst_replace_by_rightmost(target, &(*tree)->right);          //    Continue finding rightmost
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (*tree == NULL) return;                                  //    Key not found

  if (key < (*tree)->key) {                                   //    If key smaller than current node then go to left tree
    bst_delete(&(*tree)->left, key);
    return;
  } 
  if (key > (*tree)->key) {                                   //    If key bigger than current node then go to right tree
    bst_delete(&(*tree)->right, key);
    return;    
  } 

  if ((*tree)->left == NULL && (*tree)->right == NULL) {    //    Node has no childrens
      free(*tree);                                          //      just delete this node
      *tree = NULL;
      return;
  } 
  if ((*tree)->left != NULL && (*tree)->right != NULL) {    //    Node has two childrens
      bst_replace_by_rightmost(*tree, &(*tree)->left);      //      find the rightmost in left child
      return;
  }
                                                            //    Choose child to save
  bst_node_t *child = (*tree)->left != NULL ? (*tree)->left : (*tree)->right;
  free(*tree);
  *tree = child;                                            //    Replace current node with child
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if (*tree == NULL) return;                                  //    If node is NULL -> do nothing

  bst_dispose(&(*tree)->left);                                //    Dispose left childs
  bst_dispose(&(*tree)->right);                               //    Dispose right childs

  free(*tree);                                                //    Dispose current node
  *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL) return;                                   //    preorder (NODE)(left)(right)

  bst_add_node_to_items(tree, items);
  bst_preorder(tree->left, items);
  bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL) return;                                   //    preorder (left)(NODE)(right)

  bst_inorder(tree->left, items);
  bst_add_node_to_items(tree, items);
  bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL) return;                                   //    preorder (left)(right)(NODE)

  bst_postorder(tree->left, items);                           
  bst_postorder(tree->right, items);
  bst_add_node_to_items(tree, items);
}
