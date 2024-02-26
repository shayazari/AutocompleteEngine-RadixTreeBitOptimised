/* driver2.c opens input and output files, calls on the relevant
functions from other files, and runs the loop to
find all cafes matching stdin key/s.
To run, type: ./dict2 2 inFile.csv outFile.txt
* created by Shayan Azari Pour */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "parsecsv.h"
#include "array.h"
#include "query.h"

array_t *insert_sorted_array(FILE *file);

int main(int argc, char **argv) {
    FILE *inFile = fopen(argv[2], "r"),
            *outFile = fopen(argv[3], "w");
    assert((inFile != NULL) && (outFile != NULL));

    array_t *sorted_array = insert_sorted_array(inFile);


    fclose(inFile);

    char *key;

    while ((key = get_key()) != NULL) {
        if ((*key == '\n') || (*key == '\0')) {
            printf ("%s --> NOTFOUND\n", key);
        }
        else {
            cafe_t **matched_cafes;
            if ((matched_cafes = find_and_traverse_sorted_array(sorted_array,
                                                                key)) != NULL) {
                output_dict_2_3(outFile, matched_cafes, key);
            }
            else {
                printf ("%s --> NOTFOUND\n", key);
            }
            free(matched_cafes);
        }
        free(key);
    }

    fclose(outFile);
    free_array(sorted_array, free_cafe);

    return 0;

}

/* Function to read from input file, create cafe struct,
 * and insert keys into sorted array */
array_t *insert_sorted_array(FILE *file) {
    char *line = read_line(file);
    free(line);
    array_t *sorted_array = create_array();

    while ((line = read_line(file)) != NULL) {
        cafe_t *cafe = (cafe_t*)malloc(sizeof(cafe_t));
        assert(cafe);
        store_line(cafe, line);
        insert(sorted_array, cafe);
        cafe = NULL;
    }
    shrink_array(sorted_array);

    return sorted_array;
}

