/* query.c contains functions which traverse
 * the linked list to find any matches with
 * the key and functions which print the results
 * to the output file */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "query.h"

/* Stores key from stdin */
char *get_key() {

    char buffer[MAX_STRING + 1];

    // Read from stdin
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        exit(EXIT_FAILURE);
    }
    // Remove the newline character
    size_t len = strlen(buffer);
    if (buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    // Allocate and return the key
    return str_dup(buffer);
}

/* Function to traverse sorted array and find key */
cafe_t **find_and_traverse_sorted_array(array_t *array, const char *key) {
    int count = 0, found = 0, locn, str_comps = 0, char_comps = 0,
    bit_comps = 0, left_count = 0, right_count = 0;
    int current_size = 5;

    assert(array != NULL && key != NULL);

    cafe_t **matched_cafes = (cafe_t**)malloc(current_size *
                                              sizeof(cafe_t*));
    assert(matched_cafes != NULL);

    // Binary search for the first matching element
    cafe_t *match = (cafe_t *)binary_search(array, key,
                                            &locn, &str_comps, &char_comps);

    if (match != NULL) {
        int left_locn = locn;
        int right_locn = locn;
        found = 1;
        matched_cafes[count] = match;
        count++;

        while ((match = (cafe_t *)linear_search(array, key,
                                                    &left_locn,
                                                    &right_locn,
                                                    &str_comps, &char_comps,
                                                    &left_count, &right_count))
                                                    != NULL) {
            if((count + 1) >= current_size){
                current_size *= 2;
                cafe_t **temp = realloc(matched_cafes,
                                        current_size * sizeof(cafe_t*));

                if (temp == NULL) {
                    free(matched_cafes);
                    return NULL;
                }

                matched_cafes = temp;
                assert(matched_cafes != NULL);
            }
            if (left_count > 0 && right_count == 0) {
                for (int i = (count - 1); i >= (count - left_count); i--) {
                    matched_cafes[i + 1] = matched_cafes[i];
                }
                matched_cafes[count - left_count] = match;
            }
            else {
                matched_cafes[count] = match;
            }
            count++;
        }

        bit_comps = char_comps * 8;
        printf("%s --> b%d c%d s%d\n", key, bit_comps, char_comps, str_comps);
    }
    matched_cafes[count] = NULL;

    if (found) {
        return matched_cafes;
    }
    else {
        return NULL;
    }
}

/* Function to output cafe data to file */
void output_dict_2_3(FILE *outFile, cafe_t **matched_cafes, const char *key) {

    fprintf(outFile, "%s\n", key);

    for (int i = 0; matched_cafes[i] != NULL; i++) {
        if (matched_cafes[i] && matched_cafes[i]->trading_name) {

            fprintf(outFile, "--> census_year: %d || block_id: %d || "
                             "property_id: %d || base_property_id: %d || "
                             "building_address: %s || clue_small_area: %s || "
                             "business_address: %s || trading_name: %s || "
                             "industry_code: %d || industry_description: %s || "
                             "seating_type: %s || number_of_seats: %d || "
                             "longitude: %.5f || latitude: %.5f ||\n",
                    matched_cafes[i]->census_year, matched_cafes[i]->block_id,
                    matched_cafes[i]->property_id, matched_cafes[i]->base_property_id,
                    matched_cafes[i]->building_address,
                    matched_cafes[i]->clue_small_area,
                    matched_cafes[i]->business_address,
                    matched_cafes[i]->trading_name, matched_cafes[i]->industry_code,
                    matched_cafes[i]->industry_description,
                    matched_cafes[i]->seating_type, matched_cafes[i]->number_of_seats,
                    matched_cafes[i]->longitude, matched_cafes[i]->latitude);

        }
    }
}


