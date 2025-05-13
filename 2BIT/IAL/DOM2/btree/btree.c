#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Pomocná funkce která vypíše uzel stromu.
 */
void bst_print_node(bst_node_t *node) {
  printf("[%c,%d]", node->key, node->value);
}

/*
 * Pomocná funkce pro uložení uzlu stromu do pomocné stuktury.
 */
void bst_add_node_to_items(bst_node_t* node, bst_items_t *items) {
  if (items->capacity < items->size + 1)
  {
    items->capacity = items->capacity * 2 + 8;
    items->nodes = realloc(items->nodes, items->capacity * (sizeof(bst_node_t*)));
  }
  items->nodes[items->size] = node;
  items->size++;
}