//
// Created by burluiva on 10/10/2023.
//

#ifndef IFJ2023_SWIFT_STRING_H
#define IFJ2023_SWIFT_STRING_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../vector/vector.h"

typedef struct string {
  size_t length;
  char *data;
} String;

vector_def(String, String, string_);
typedef String* StringPointer;
vector_def(StringPointer, StringPointer, string_pointer_);

String string_create(char *cstr);
int string_cmp(String *a, String* b);
void string_free(String *str);

#endif // IFJ2023_SWIFT_STRING_H
