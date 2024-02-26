/* linkedlist.c sets out the required linked list functions in order
 * to store, create and traverse the linked list
 * created by Aditya Sarkar */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "linkedlist.h"

struct node {
    void *data;
    node_t *next;
};

struct list {
    node_t *head;
    node_t *foot;
};

/* Creates an empty linked list. */
list_t *make_empty_list(void) {
    list_t *list;

    list = (list_t*)malloc(sizeof(*list));
    assert(list!=NULL);
    list->head = list->foot = NULL;

    return list;
}


/* Frees the memory occupied by the linked list and
 * its nodes. Is polymorphic and uses function pointers
 * in order to avoid having to include parsecsv */
void free_list(list_t *list, void (*free_func)(void *data)) {
    node_t *curr, *prev;

    assert(list != NULL);
    curr = list->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free_func(prev->data);
        free(prev);
    }

    free(list);
}

/* Inserts a new node with the given value
 * at the end of the linked list. */
list_t *insert_cafe(list_t *list, void *value) {

    assert(list != NULL);
    node_t *new = (node_t*)malloc(sizeof(node_t));
    assert(new != NULL);

    new->next = NULL;
    new->data = value;

    if (list->head == NULL) {
        list->head = new;
        list->foot = new;
    }
    else {
        list->foot->next = new;
        list->foot = new;
    }
    return list;
}
