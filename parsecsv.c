/* parsecsv.c reads CSV data from a file, stores fields in respective
 * structs, and stores structs in a linked list (dictionary)
 * created by Shayan Azari Pour */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parsecsv.h"

/* Reads one line from input file */
char *read_line(FILE *file) {
    char *line = (char*)malloc(MAX_LINE + 1);
    assert(line != NULL);

    if (fgets(line, MAX_LINE + 1, file) != NULL)
    {
        return line;
    }
    else {
        free(line);
        return NULL;
    }

}

/* Parses a line read and stores inside structure */
void store_line(cafe_t *cafe, char *line) {
    int line_index = 0, i = 0, j = 0;
    char temp_string[MAX_STRING + 1];

    while (line[i] != '\n' && line[i] != '\0') {

        // Enclosed in double quotes; different end condition
        if (line[i] == '"') {
            i++;
            while (line[i] != '"') {
                temp_string[j] = line[i];
                i++;
                j++;

            }
            i += 2;
        }
        else {
            while (line[i] != ',' && line[i] != '\n' && line[i] != '\0') {
                temp_string[j] = line[i];
                i++;
                j++;

            }
            i++;
        }
        temp_string[j] = '\0';
        j = 0;

        // Storing in appropriate field based on line index
        switch (line_index) {
            case 0: sscanf(temp_string, "%d", &cafe->census_year); break;
            case 1: sscanf(temp_string, "%d", &cafe->block_id); break;
            case 2: sscanf(temp_string, "%d", &cafe->property_id); break;
            case 3: sscanf(temp_string, "%d", &cafe->base_property_id); break;
            case 4: cafe->building_address = str_dup(temp_string);
                assert(cafe->building_address != NULL); break;
            case 5: cafe->clue_small_area = str_dup(temp_string);
                assert(cafe->clue_small_area != NULL); break;
            case 6: cafe->business_address = str_dup(temp_string);
                assert(cafe->business_address != NULL); break;
            case 7: cafe->trading_name = str_dup(temp_string);
                assert(cafe->trading_name != NULL); break;
            case 8: sscanf(temp_string, "%d", &cafe->industry_code); break;
            case 9: cafe->industry_description = str_dup(temp_string);
                assert(cafe->industry_description != NULL); break;
            case 10: cafe->seating_type = str_dup(temp_string);
                assert(cafe->seating_type != NULL); break;
            case 11: sscanf(temp_string, "%d", &cafe->number_of_seats); break;
            case 12: sscanf(temp_string, "%lf", &cafe->longitude); break;
            case 13: sscanf(temp_string, "%lf", &cafe->latitude); break;
        }
        line_index++;

        if (line_index > MAX_FIELDS) {
            break;
        }
    }
}

/* frees cafe */
void free_cafe(void *data) {
    cafe_t *cafe = (cafe_t *)data;

    free(cafe->building_address); cafe->building_address = NULL;
    free(cafe->clue_small_area); cafe->clue_small_area = NULL;
    free(cafe->business_address); cafe->business_address = NULL;
    free(cafe->trading_name); cafe->trading_name = NULL;
    free(cafe->industry_description); cafe->industry_description = NULL;
    free(cafe->seating_type); cafe->seating_type = NULL;
    free(cafe);
}

/* returns the comparison reslts between two cafe keys */
int key_compare(const void *a, const void *b) {
    const cafe_t *cafe_a = (const cafe_t *)a;
    const cafe_t *cafe_b = (const cafe_t *)b;
    int cmp = strcmp(cafe_a->trading_name, cafe_b->trading_name);
    return cmp;
}

/* determines if key is a valid prefix of another */
int prefix_match(const void *a, const void *b, int *char_comps) {
    const char *str = (const char *)a;
    const cafe_t *cafe = (const cafe_t *)b;

    size_t prefix_length = strlen(str);
    size_t cafe_length = strlen(cafe->trading_name);
    int prefix_check_return;
    if (prefix_length > cafe_length) {
        prefix_length = cafe_length;
        if ((prefix_check_return = prefix_check(
                    str, cafe->trading_name, prefix_length,
                                            char_comps)) == 0) {
            return 1;
        }
        else {
            return prefix_check_return;
        }
    }
    return prefix_check(str, cafe->trading_name,
                        prefix_length, char_comps);
}

/* Comparing 2 strings where 1 could be a prefix of the other */
int prefix_check(const char *s1, const char *s2, size_t n, int *char_comps) {
    while (n > 0 && *s1 && *s2) {
        (*char_comps)++;
        if (*s1 != *s2) {
            return (*s1 - *s2);
        }
        s1++;
        s2++;
        n--;
    }

    if (n == 0) {
        /* unnecessary; only to match sample solutions
         * (ensures null byte is counted) */
        (*char_comps)++;
        return FOUND;
    } else {
        return (*s1 - *s2);
    }
}

/* Returns cafe key */
unsigned char *return_cafe_key(void *data) {
    cafe_t *cafe = (cafe_t *)data;
    unsigned char *key = (unsigned char *)cafe->trading_name;
    return key;
}

/* Returns cafe size */
int cafe_size() {
    return sizeof(cafe_t);
}

/* Duplicates string */
char *str_dup(const char *s) {
    if (!s) {
        return NULL;  // Handle NULL input
    }
    char *p;
    char *duplicate = malloc((strlen(s) + 1) * sizeof(*duplicate));
    assert(duplicate);

    for (p = duplicate; *s; *(p++) = *(s++));
    *p = '\0';

    return duplicate;
}
