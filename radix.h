/*
 * File: radix.h
 * Author: Shayan Azari Pour
 * Description: This header defines functions and structures
 * related to radix tree operations.
 */
#ifndef A2_RADIX_H

#define A2_RADIX_H

#include "parsecsv.h"

#define CHAR_BITS 8

typedef struct node radix_node_t;
typedef struct radix radix_t;

radix_t *make_empty_radix(void);
void recursive_free_radix(radix_node_t *root, void (*free_func)(void *data));
void free_radix(radix_t *radix, void (*free_func)(void *data));
unsigned char *return_cafe_key(void *data);
int cafe_size(void);
void insert_2(radix_t *tree, void *data);
unsigned char* splitStem(unsigned char *key, int start, int end);
unsigned char *shift_prefix_bits(radix_node_t *prefix_ptr, int bits_to_shift);
int getBit(char ch, int pos);
void add_data_to_node(radix_node_t *node, void *data);
void traverse_and_collect(radix_node_t *curr, void*** results, int* count, int *char_comps);
void** collect_data(radix_node_t *node, int *char_comps);
void** search(radix_t *tree, unsigned char *key, int *bit_comps, int *char_comps);

#endif //A2_RADIX_H

