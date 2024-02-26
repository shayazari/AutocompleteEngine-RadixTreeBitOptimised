/* driver3.c opens input and output files, calls on the relevant
functions from other files, and runs the loop to
find all cafes matching stdin key/s.
To run, type: ./dict3 3 inFile.csv outFile.txt
* created by Shayan Azari Pour */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "parsecsv.h"
#include "radix.h"
#include "query.h"

radix_t *insert_radix(FILE *file);

int main(int argc, char **argv) {
    FILE *inFile = fopen(argv[2], "r"),
            *outFile = fopen(argv[3], "w");
    assert((inFile != NULL) && (outFile != NULL));

    radix_t *radix = insert_radix(inFile);

    fclose(inFile);
    unsigned char *key;
    int bit_comps = 0, char_comps = 0, str_comps = 1;
    while ((key = get_key()) != NULL) {
        if ((*key == '\n') || (*key == '\0')) {
            printf ("%s --> NOTFOUND\n", key);
        }
        else {
            cafe_t** results = (cafe_t**)search(radix,
                                                key, &bit_comps, &char_comps);

            if (results == NULL) {
                printf( "%s --> NOTFOUND\n", key);
            }
            else if (results) {
                    printf("%s --> b%d c%d s%d\n", key, bit_comps,
                           char_comps, str_comps);
                output_dict_2_3(outFile, results, key);
            }
            if (results) {
                free(results);
            }
            bit_comps = 0, char_comps = 0;
        }
        free(key);
    }

    fclose(outFile);
    free_radix(radix, free_cafe);

    return 0;

}

/* Function to read from input file, create cafe struct,
 * and insert keys into radix tree */
radix_t *insert_radix(FILE *file) {
    char *line = read_line(file);
    free(line);
    radix_t *radix = make_empty_radix();

    while ((line = read_line(file)) != NULL) {
        cafe_t *cafe = (cafe_t*)malloc(sizeof(cafe_t));
        assert(cafe);
        store_line(cafe, line);
        insert_2(radix, cafe);
        cafe = NULL;
    }
    return radix;
}

