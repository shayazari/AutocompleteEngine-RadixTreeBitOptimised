/*
 * File: query.h
 * Author: Aditya Sarkar
 * Description: This header defines functions and structures related
 * to finding matches with the query in the linked list as well as printing
 * results to output file.
 */

#ifndef _QUERY_H_
#define _QUERY_H_

#include "parsecsv.h"
#include "radix.h"

cafe_t **find_and_traverse_linked_list(list_t *list, const char *key);
void output_dict1(FILE *outFile, cafe_t **matched_cafes);
char *get_key();
cafe_t **find_and_traverse_sorted_array(array_t *array, const char *key);
void output_dict_2_3(FILE *outFile, cafe_t **matched_cafes, const char *key);

#endif
