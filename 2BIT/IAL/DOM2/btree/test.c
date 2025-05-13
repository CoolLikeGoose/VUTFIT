#include "btree.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>

const int base_data_count = 15;
const char base_keys[] = {'H', 'D', 'L', 'B', 'F', 'J', 'N', 'A',
                          'C', 'E', 'G', 'I', 'K', 'M', 'O'};
const int base_values[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 16};

const int additional_data_count = 6;
const char additional_keys[] = {'S', 'R', 'Q', 'P', 'X', 'Y', 'Z'};
const int additional_values[] = {10, 10, 10, 10, 10, 10};

const int traversal_data_count = 5;
const char traversal_keys[] = {'D', 'B', 'A', 'C', 'E'};
const int traversal_values[] = {1, 2, 3, 4, 5};

void red() {
  printf("\033[1;31m");
}

void green() {
  printf("\033[1;32m");
}

void cyan() {
  printf("\033[1;36m");
}

void reset_color(){
  printf("\033[0m");
}

int tests_passed = 0;
int tests_failed;

void init_test() {
  printf("Binary Search Tree - testing script\n");
  printf("-----------------------------------\n");
  printf("\n");
}

TEST(test_tree_init, "Initialize the tree")
bst_init(&test_tree);
ENDTEST

TEST(test_tree_dispose_empty, "Dispose the tree")
bst_init(&test_tree);
bst_dispose(&test_tree);
ENDTEST

TEST(test_tree_search_empty, "Search in an empty tree (A)")
bst_init(&test_tree);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'A', &result);
if (bool_res == true){
  red();
  printf("\nNode A was found! [TEST FAILED ☓]\n\n");
} else{
  green();
  printf("\nNode A was not found: [TEST PASSED ✓]\n\n");
  tests_passed++;
} 
reset_color();
ENDTEST

TEST(test_tree_insert_root, "Insert an item (H,1)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
bst_print_tree(test_tree);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'H', &result);
if (bool_res == true && (result) == 1){
  green();
  printf("Node H was inserted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node H was NOT inserted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
ENDTEST

TEST(test_tree_search_root, "Search in a single node tree (H)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'H', &result);
bst_print_tree(test_tree);
if (bool_res == true){
  green();
  printf("Node H was found correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node H was NOT found correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
ENDTEST

TEST(test_tree_update_root, "Update a node in a single node tree (H,1)->(H,8)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
bst_print_tree(test_tree);
bst_insert(&test_tree, 'H', 8);
bst_print_tree(test_tree);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'H', &result);
if (bool_res == true && result == 8){
  green();
  printf("Value of the H node was updated correctly! [TEST PASSED ✓]\n");
  printf("New value of the H node is: 8\n");
  tests_passed++;
} else {
  red();
  printf("Value of the H node was NOT updated correctly! [TEST PASSED ✓]\n\n");
}
reset_color();
ENDTEST

TEST(test_tree_insert_many, "Insert many values")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
cyan();
printf("\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("|  If you implemented the bst_insert() correctly, output below should look like this now  |\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("\n");
printf("Binary tree structure:\n");
printf("\n");
printf("           +-[O,16]\n");
printf("           |\n");
printf("        +-[N,14]\n");
printf("        |  |\n");
printf("        |  +-[M,13]\n");
printf("        |\n");
printf("     +-[L,12]\n");
printf("     |  |\n");
printf("     |  |  +-[K,11]\n");
printf("     |  |  |\n");
printf("     |  +-[J,10]\n");
printf("     |     |\n");
printf("     |     +-[I,9]\n");
printf("     |\n");
printf("  +-[H,8]\n");
printf("     |\n");
printf("     |     +-[G,7]\n");
printf("     |     |\n");
printf("     |  +-[F,6]\n");
printf("     |  |  |\n");
printf("     |  |  +-[E,5]\n");
printf("     |  |\n");
printf("     +-[D,4]\n");
printf("        |\n");
printf("        |  +-[C,3]\n");
printf("        |  |\n");
printf("        +-[B,2]\n");
printf("           |\n");
printf("           +-[A,1]\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search, "Search for an item deeper in the tree (A)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'A', &result);
if (bool_res == true){
  green();
  printf("\nNode A was found correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else{
  red();
  printf("\nNode A was NOT found correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search_missing, "Search for a missing key (X)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'X', &result);
if (bool_res == true){
  red();
  printf("\nNode X was found INcorrectly: [TEST FAILED ☓]\n\n");
} else {
  green();
  printf("\nNode X was NOT found: [TEST PASSED ✓]\n\n");
  tests_passed++;
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_leaf, "Delete a leaf node (A)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'A');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool_res = bst_search(test_tree, 'A', &result);
bool_res2 = bst_search(test_tree, 'H', &result2);
if (bool_res2 == true && bool_res == false){
  green();
  printf("Node A was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node A was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_left_subtree, "Delete a node with only left subtree (R)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'R');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool_res = bst_search(test_tree, 'R', &result);
bool_res2 = bst_search(test_tree, 'H', &result2);
if (bool_res2 == true && bool_res == false){
  green();
  printf("Node R was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node R was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_right_subtree,
     "Delete a node with only right subtree (X)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);

bst_print_tree(test_tree);
bst_delete(&test_tree, 'X');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool_res = bst_search(test_tree, 'X', &result);
bool_res2 = bst_search(test_tree, 'H', &result2);
if (bool_res2 == true && bool_res == false){
  green();
  printf("Node X was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node X was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_both_subtrees, "Delete a node with both subtrees (L)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);

bst_print_tree(test_tree);
bst_delete(&test_tree, 'L');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool_res = bst_search(test_tree, 'L', &result);
bool_res2 = bst_search(test_tree, 'H', &result2);
if (bool_res2 == true && bool_res == false){
  green();
  printf("Node L was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node L was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_missing, "Delete a node that doesn't exist (U)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'U');
cyan();
cyan();
printf("\n");
printf("--------------------------------------------------------------------------\n");
printf("|  Nothing should change ---> Output above and below should be the same  |\n");
printf("--------------------------------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_root, "Delete the root node (H)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'H');
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'H', &result);
if (bool_res == true){
  red();
  printf("Node H was NOT deleted correctly: [TEST FAILED ☓]\n\n");
} else {
  green();
  printf("Node H was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_dispose_filled, "Dispose the whole tree")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_dispose(&test_tree);
cyan();
printf("\n");
printf("---------------------------------------------------\n");
printf("|  Correct output below should be: Tree is empty  |\n");
printf("---------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_preorder, "Traverse the tree using preorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_preorder(test_tree, test_items);
bst_print_tree(test_tree);
cyan();
printf("\n");
printf("---------------------------------------------------------------\n");
printf("|  Correct output below should be: [D,1][B,2][A,3][C,4][E,5]  |\n");
printf("---------------------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_items(test_items);
ENDTEST

TEST(test_tree_inorder, "Traverse the tree using inorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_inorder(test_tree, test_items);
bst_print_tree(test_tree);
cyan();
printf("\n");
printf("---------------------------------------------------------------\n");
printf("|  Correct output below should be: [A,3][B,2][C,4][D,1][E,5]  |\n");
printf("---------------------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_items(test_items);
ENDTEST

TEST(test_tree_postorder, "Traverse the tree using postorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_postorder(test_tree, test_items);
bst_print_tree(test_tree);
cyan();
printf("\n");
printf("---------------------------------------------------------------\n");
printf("|  Correct output below should be: [A,3][C,4][B,2][E,5][D,1]  |\n");
printf("---------------------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_items(test_items);
ENDTEST

#ifdef EXA

TEST(test_letter_count, "Count letters");
bst_init(&test_tree);
letter_count(&test_tree, "abBcCcdef_ 123 *");
bst_print_tree(test_tree);
ENDTEST

TEST(test_balance, "Count letters and balance");
bst_init(&test_tree);
letter_count(&test_tree, "abBcCcdef_ 123 *");
bst_balance(&test_tree);
bst_print_tree(test_tree);
ENDTEST

#endif // EXA

int main(int argc, char *argv[]) {
  init_test();

  test_tree_init();
  test_tree_dispose_empty();
  test_tree_search_empty();
  test_tree_insert_root();
  test_tree_search_root();
  test_tree_update_root();
  test_tree_insert_many();
  test_tree_search();
  test_tree_search_missing();
  test_tree_delete_leaf();
  test_tree_delete_left_subtree();
  test_tree_delete_right_subtree();
  test_tree_delete_both_subtrees();
  test_tree_delete_missing();
  test_tree_delete_root();
  test_tree_dispose_filled();
  test_tree_preorder();
  test_tree_inorder();
  test_tree_postorder();
  
  tests_failed = 11 - tests_passed;
  printf("\n");
  printf("---------- TESTS SUMMARY ----------\n");
  printf("|                                 |\n");
  printf("|");
  green();
  printf(" TESTS PASSED: %d", tests_passed);
  reset_color();
  if(tests_passed < 10)
    printf(" ");
  printf("                |\n");
  printf("|");
  red();
  printf(" TESTS FAILED: %d", tests_failed);
  reset_color();
    if(tests_failed < 10)
    printf(" ");
  printf("                |\n");
  printf("|                                 |\n");
  printf("-----------------------------------\n");
  printf("\n");
  reset_color();

#ifdef EXA
  test_letter_count();
  test_balance();
#endif // EXA
}

/* author ~ xcuprm01 */
