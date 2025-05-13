#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *subtree_prefix = "  |";
const char *space_prefix = "   ";

char *make_prefix(char *prefix, const char *suffix) {
  char *result = (char *)malloc(strlen(prefix) + strlen(suffix) + 1);
  strcpy(result, prefix);
  result = strcat(result, suffix);
  return result;
}

void bst_print_subtree(bst_node_t *tree, char *prefix, direction_t from) {
  if (tree != NULL) {
    char *current_subtree_prefix = make_prefix(prefix, subtree_prefix);
    char *current_space_prefix = make_prefix(prefix, space_prefix);

    if (from == left) {
      printf("%s\n", current_subtree_prefix);
    }

    bst_print_subtree(
        tree->right,
        from == left ? current_subtree_prefix : current_space_prefix, right);

    printf("%s  +-", prefix);
    bst_print_node(tree);
    printf("\n");

    bst_print_subtree(
        tree->left,
        from == right ? current_subtree_prefix : current_space_prefix, left);

    if (from == right) {
      printf("%s\n", current_subtree_prefix);
    }

    free(current_space_prefix);
    free(current_subtree_prefix);
  }
}

void bst_print_tree(bst_node_t *tree) {
  printf("Binary tree structure:\n");
  printf("\n");
  if (tree != NULL) {
    bst_print_subtree(tree, "", none);
  } else {
    printf("Tree is empty\n");
  }
  printf("\n");
}

bst_items_t* bst_init_items() {
  bst_items_t* items = malloc(sizeof(bst_items_t));
  items->capacity = 0;
  items->nodes = NULL;
  items->size = 0;
  return items;
}

void bst_print_items(bst_items_t *items) {
  printf("Traversed items:\n");
    for (int i = 0; i < items->size; i++)
    {
        bst_print_node(items->nodes[i]);
    }
  printf("\n");
}

void bst_reset_items (bst_items_t *items) {
  if(items != NULL) {
    if (items->capacity > 0)
    {
      free(items->nodes);
    }
    items->capacity = 0;
    items->size = 0;
  }
}

void bst_insert_many(bst_node_t **tree, const char keys[], const int values[],
                     int count) {
  for (int i = 0; i < count; i++) {
    bst_insert(tree, keys[i], values[i]);
  }
}
