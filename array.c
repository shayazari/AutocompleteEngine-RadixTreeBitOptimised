/* array.c stores data into a sorted array
 * and searches for keys via binary
 * and linear searches.
 * created by Shayan Azari Pour */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "array.h"

struct array {
    void **data;
    int size; // Current array size
    int n;    // Current number of elements used
};

/* inserting data into array, maintaining ascending order */
void insert(array_t *arr, void *value) {
    int i;
    ensure_array_size(arr);

    // Find position to insert value given array must be sorted
    for (i = 0; i < arr->n && key_compare(value,
                                           arr->data[i]) >= 0; i++) {
    }
    if (i < arr->n && key_compare(value, arr->data[i]) == 0) {
        i++;
    }

    // Shift all array elements to make space for new key
    for (int j = arr->n; j > i; j--) {
        arr->data[j] = arr->data[j - 1];
    }

    arr->data[i] = value;
    (arr->n)++;
}

/* create array */
array_t *create_array() {
    array_t *arr = malloc(sizeof(*arr));
    assert(arr);
    int size = INIT_SIZE;
    arr->size = size;
    arr->data = malloc(size * sizeof(*(arr->data)));
    assert(arr->data);
    arr->n = 0;
    return arr;
}

/* frees array contents (cafe structs) and frees array */
void free_array(array_t *arr, void (*free_func)(void *data)) {
    for (int i = 0; i < arr->n; i++) {
        free_func(arr->data[i]);
    }
    free(arr->data);
    free(arr);
}

/* after insertion, shrinks array if needed
 * (implemented from COMP20003 workshop) */
void shrink_array(array_t *arr) {
    if (arr->size != arr->n) {
        arr->size = (arr->n > INIT_SIZE) ? arr->n : INIT_SIZE;
        arr->data = realloc(arr->data, arr->size
                                       * sizeof(*(arr->data)));
        assert(arr->data);
    }
}

/* reallocs the array if array size maxed out
 * (implemented from COMP20003 workshop) */
void ensure_array_size(array_t *arr) {
    if (arr->n == arr->size) {
        arr->size <<= 1;
        arr->data = realloc(arr->data,
                            arr->size * sizeof(*(arr->data)));
        assert(arr->data);
    }
}

/* performs binary search for a key */
void *binary_search(array_t *arr, const char *key, int *locn, int *str_comps, int *char_comps) {
    int mid, lo = 0, hi = arr->n - 1;

    while (lo <= hi) {
        mid = (lo + hi) / 2;
        (*str_comps)++;
        void *element = arr->data[mid];
        int cmp = prefix_match(key, element, char_comps);

        if (cmp == 0) {
            *locn = mid;
            return element;
        }
        else if (cmp < 0) {
            hi = mid - 1;
        }
        else {
            lo = mid + 1;
        }
    }
    return NULL;
}

/* performs linear search from a left and right locaton adjacent to
 * key location fround from binary search and */
void *linear_search(array_t *arr, const char *key, int *left_locn,
                    int *right_locn, int *str_comps, int *char_comps,
                    int *left_count, int *right_count) {
    int left = *left_locn - 1;
    int right = *right_locn + 1;

    // look left
    if (left >= 0) {
        (*str_comps)++;
        void *element = arr->data[left];
        if (prefix_match(key, element, char_comps) == 0) {
            (*left_count)++;
            (*left_locn)--;
            return element;
        }
        else {
            *left_locn = 0;
        }
    }

    // look right
    if (right < arr->n) {
        (*str_comps)++;
        void *element = arr->data[right];
        if (prefix_match(key, element, char_comps) == 0) {
            (*right_count)++;
            (*right_locn)++;
            return element;
        }
        else {
            *right_locn = arr->n;
        }
    }
    return NULL;
}


