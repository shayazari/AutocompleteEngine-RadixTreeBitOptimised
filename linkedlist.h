/*
 * File: linkedlist.h
 * Author: Aditya Sarkar
 * Description: This header defines functions and structures
 * related to linked list operations.
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node node_t;

typedef struct list list_t;

list_t *make_empty_list(void);
void free_list(list_t *list, void (*free_func)(void *data));
list_t *insert_cafe(list_t *list, void *value);

#endif
