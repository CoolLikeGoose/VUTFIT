//
// Created by burluiva on 10/10/2023.
//

#include "util.h"
#include <assert.h>
#include <string.h>

void swap(void *a, void *b, size_t size) {
  assert(a && b && size > 0);
  void *tmp = malloc(size);
  memcpy(tmp, a, size);
  memcpy(a, b, size);
  memcpy(b, tmp, size);
  free(tmp);
}
