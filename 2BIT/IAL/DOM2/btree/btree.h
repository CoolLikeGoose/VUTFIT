/*
 * Hlavičkový soubor pro binární vyhledávací strom.
 * Tento soubor neupravujte.
 */

#ifndef IAL_BTREE_H
#define IAL_BTREE_H

#include <stdbool.h>

// Uzel stromu
typedef struct bst_node {
  char key;               // klíč
  int value;              // hodnota
  struct bst_node *left;  // levý potomek
  struct bst_node *right; // pravý potomek
} bst_node_t;

void bst_init(bst_node_t **tree);
void bst_insert(bst_node_t **tree, char key, int value);
bool bst_search(bst_node_t *tree, char key, int *value);
void bst_delete(bst_node_t **tree, char key);
void bst_dispose(bst_node_t **tree);

// Pole uzlu
typedef struct bst_items {
  bst_node_t **nodes;     // pole uzlu
  int capacity;           // kapacita alokované paměti v počtu položek
  int size;               // aktuální velikost pole v počtu položek
} bst_items_t;

void bst_add_node_to_items(bst_node_t* node, bst_items_t *items);

void bst_preorder(bst_node_t *tree, bst_items_t *items);
void bst_inorder(bst_node_t *tree, bst_items_t *items);
void bst_postorder(bst_node_t *tree, bst_items_t *items);

void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);

void bst_print_node(bst_node_t *node);

void bst_balance(bst_node_t **tree);
void letter_count(bst_node_t **letter_frequency_tree, char *input);

#endif
