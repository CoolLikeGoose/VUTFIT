/*
 * Implementace pomocných zásobníků.
 * Tento soubor neupravujte.
 */
#include "stack.h"
#include <stdio.h>

/*
 * Makro generující implementaci funkcí pracujících se zásobníky.
 * Podrobnější popis zásobníků v stack.h.
 */
#define STACKDEF(T, TNAME)                                                     \
  void stack_##TNAME##_init(stack_##TNAME##_t *stack) { stack->top = -1; }     \
                                                                               \
  void stack_##TNAME##_push(stack_##TNAME##_t *stack, T item) {                \
    if (stack->top == MAXSTACK - 1) {                                          \
      printf("[W] Stack overflow\n");                                          \
    } else {                                                                   \
      stack->items[++stack->top] = item;                                       \
    }                                                                          \
  }                                                                            \
                                                                               \
  T stack_##TNAME##_top(stack_##TNAME##_t *stack) {                            \
    if (stack->top == -1) {                                                    \
      return NULL;                                                             \
    }                                                                          \
    return stack->items[stack->top];                                           \
  }                                                                            \
                                                                               \
  T stack_##TNAME##_pop(stack_##TNAME##_t *stack) {                            \
    if (stack->top == -1) {                                                    \
      printf("[W] Stack underflow\n");                                         \
      return NULL;                                                             \
    }                                                                          \
    return stack->items[stack->top--];                                         \
  }                                                                            \
                                                                               \
  bool stack_##TNAME##_empty(stack_##TNAME##_t *stack) {                       \
    return stack->top == -1;                                                   \
  }

STACKDEF(bst_node_t*, bst)
STACKDEF(bool, bool)
