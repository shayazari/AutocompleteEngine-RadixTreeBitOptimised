/*
 * File: parsecsv.h
 * Author: Shayan Azari Pour
 * Description: This header defines functions and structures
 * related to file and cafe operations.
 */
#ifndef _PARSECSV_H_
#define _PARSECSV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "linkedlist.h"
#include "array.h"
#include "radix.h"

#define MAX_STRING 128
#define MAX_LINE 512
#define MAX_FIELDS 13
#define NOT_FOUND 1
#define FOUND 0

typedef struct {
    int census_year;
    int block_id;
    int property_id;
    int base_property_id;
    char *building_address;
    char *clue_small_area;
    char *business_address;
    char *trading_name;
    int industry_code;
    char *industry_description;
    char *seating_type;
    int number_of_seats;
    double longitude;
    double latitude;

} cafe_t;


char *read_line(FILE *file);
void store_line(cafe_t *cafe, char *line);
void free_cafe(void *data);
int prefix_check(const char *s1, const char *s2, size_t n, int *char_comps);
char *str_dup(const char *s);

#endif
