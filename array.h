/*
 * File: array.h
 * Author: Shayan Azari Pour
 * Description: This header defines functions and structures
 * related to array operations.
 */

#ifndef A1_ARRAY_H
#define A1_ARRAY_H



#define INIT_SIZE 2     // Initial size of arrays

typedef struct array array_t;

typedef int (*compare_func_t)(const void *, const void *);

void insert(array_t *arr, void *value);
void free_array(array_t *arr, void (*free_func)(void *data));
void shrink_array(array_t *arr);
array_t *create_array();
void ensure_array_size(array_t *arr);
void *binary_search(array_t *arr, const char *key, int *locn, int *str_comps,
                                                            int *char_comps);
void *linear_search(array_t *arr, const char *key, int *left_loc,
                    int *right_locn, int *str_comps, int *char_comps,
                    int *left_count, int *right_count);
int prefix_match(const void *a, const void *b, int *char_comps);
int key_compare(const void *a, const void *b);

#endif //A1_ARRAY_H

