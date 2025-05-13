//
// Created by burluiva on 10/10/2023.
//

#ifndef IFJ2023_SWIFT_VECTOR_T_H
#define IFJ2023_SWIFT_VECTOR_T_H

#include "../../utils/util.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define vector_MIN_CAP 16

#define vector_struct_def(T, N)                                                \
  typedef struct N##_vector_t {                                                \
    T *data;                                                                   \
    size_t capacity;                                                           \
    size_t length;                                                             \
  } N##Vector;

#define vector_funcs_def(T, N, FN)                                             \
  void FN##vector_init_size(N##Vector *vec, size_t length);                    \
  void FN##vector_init_arr(N##Vector *vec, const T *arr, size_t arr_length);   \
  void FN##vector_init(N##Vector *vec);                                        \
  T FN##vector_at(N##Vector *vec, size_t idx);                                 \
  void FN##vector_set(N##Vector *vec, size_t idx, T value);                    \
  void FN##vector_isert(N##Vector *vec, size_t idx, T value);                  \
  void FN##vector_push_back(N##Vector *vec, T value);                          \
  void FN##vector_free(N##Vector *vec);                                        \
  T FN##vector_remove(N##Vector *vec, size_t pos);                             \
  T FN##vector_pop(N##Vector *vec);                                            \
  void FN##vector_clear(N##Vector *vec);

#define vector_init_impl(T, N, FN)                                             \
  void FN##vector_init_size(N##Vector *vec, size_t length) {                   \
    assert(vec);                                                               \
    vec->capacity = vector_MIN_CAP > length ? vector_MIN_CAP : length;         \
    vec->length = length;                                                      \
    vec->data = calloc(sizeof(T), vec->capacity);                              \
    assert(vec->data);                                                         \
  }                                                                            \
  void FN##vector_init_arr(N##Vector *vec, const T *arr, size_t arr_length) {  \
    assert(vec &&arr &&arr_length >= 0);                                       \
    FN##vector_init_size(vec, arr_length);                                     \
    for (size_t i = 0; i < arr_length; i++)                                    \
      vec->data[i] = arr[i];                                                   \
  }                                                                            \
                                                                               \
  void FN##vector_init(N##Vector *vec) { FN##vector_init_size(vec, 0); }

#define vector_at_impl(T, N, FN)                                               \
  T FN##vector_at(N##Vector *vec, size_t idx) {                                \
    assert(vec &&idx < vec->length && vec->data);                              \
    return vec->data[idx];                                                     \
  }

#define vector_set_impl(T, N, FN)                                              \
  void FN##vector_set(N##Vector *vec, size_t idx, T value) {                   \
    assert(vec &&idx < vec->length && vec->data);                              \
    vec->data[idx] = value;                                                    \
  }

#define vector_push_impl(T, N, FN)                                             \
  void FN##vector_push_back(N##Vector *vec, T value) {                         \
    assert(vec && vec->data);                                                  \
    if (vec->length == vec->capacity) {                                        \
      vec->capacity *= 2;                                                      \
      T *tmp = realloc(vec->data, sizeof(T) * vec->capacity);                  \
      assert(tmp);                                                             \
      vec->data = tmp;                                                         \
    }                                                                          \
    vec->length = vec->length + 1;                                             \
    FN##vector_set(vec, vec->length - 1, value);                               \
  }                                                                            \
  void FN##vector_insert(N##Vector *vec, size_t idx, T value) {                \
    assert(vec &&idx <= vec->length && vec->data);                             \
    FN##vector_push_back(vec, value);                                          \
    for (size_t i = vec->length - 1; i > idx; i--) {                           \
      swap(vec->data + i, vec->data + i - 1, sizeof(T));                       \
    }                                                                          \
  }

#define vector_free_impl(T, N, FN)                                             \
  void FN##vector_free(N##Vector *vec) {                                       \
    assert(vec);                                                               \
    free(vec);                                                                 \
  }

#define vector_remove_impl(T, N, FN)                                           \
  T FN##vector_remove(N##Vector *vec, size_t pos) {                            \
    assert(vec && vec->data && pos < vec->length);                             \
    T tmp = FN##vector_at(vec, pos);                                           \
    vec->length = vec->length - 1;                                             \
    for (size_t i = pos; i < vec->length; i++) {                               \
      swap(vec->data + i, vec->data + i + 1, sizeof(T));                       \
    }                                                                          \
    return tmp;                                                                \
  }                                                                            \
  T FN##vector_pop(N##Vector *vec) {                                           \
    assert(vec && vec->length > 0);                                            \
                                                                               \
    return FN##vector_remove(vec, vec->length - 1);                            \
  }                                                                            \
  void FN##vector_clear(N##Vector *vec) {                                      \
    assert(vec);                                                               \
    vec->length = 0;                                                           \
  }

#define vector_def(T, N, FN)                                                   \
  vector_struct_def(T, N);                                                     \
  vector_funcs_def(T, N, FN)

#define vector_impl(T, N, FN)                                                  \
  vector_init_impl(T, N, FN) vector_free_impl(T, N, FN)                        \
      vector_at_impl(T, N, FN) vector_set_impl(T, N, FN)                       \
          vector_push_impl(T, N, FN) vector_remove_impl(T, N, FN)

#endif // IFJ2023_SWIFT_VECTOR_T_H
