/* radix.c stores data into a radix tree
 * and searches for keys based on their
 * binary representations (bits).
 * created by Shayan Azari Pour */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "radix.h"

struct node {
    unsigned char *prefix; // Stores the binary representation of the prefix
    int prefix_bits;       // Number of valid bits in the prefix
    struct node *left_0; // Pointer to the left child (0)
    struct node *right_1; // Pointer to the right child (1)
    void **data;  // Can hold multiple data items
    int data_count;  // Number of data items
};

struct radix {
    radix_node_t *root;
};

radix_t *make_empty_radix(void) {
    radix_t *radix;
    radix = (radix_t*)malloc(sizeof(*radix));
    assert(radix!=NULL);
    radix->root = NULL;

    return radix;
}

void recursive_free_radix(radix_node_t *root, void (*free_func)(void *data)) {
    if (root) {
        if (root->data != NULL) {
            for (int i = 0; i < root->data_count; i++) {
                free_func(root->data[i]);
                if ((i + 1) == root->data_count) {
                    free(root->data[i]);
                }
            }
            free(root->data);
        }
        recursive_free_radix(root->left_0, free_func);
        recursive_free_radix(root->right_1, free_func);
        if (root->prefix) {
            free(root->prefix);
        }
        free(root);
    }
}

void free_radix(radix_t *radix, void (*free_func)(void *data)) {
    assert(radix != NULL);
    recursive_free_radix(radix->root, free_func);
    free(radix);
}

void add_data_to_node(radix_node_t *node, void *data) {
    int new_size = (node->data_count + 1) * cafe_size();

    void **temp = realloc(node->data, new_size);
    if (temp == NULL) {
        free(node->data);
        node->data = NULL;
        return;
    }

    node->data = temp;
    node->data[node->data_count] = data;
    node->data_count++;

}

radix_node_t* create_node(const char *prefix, int prefix_bits) {
    radix_node_t *new_node = malloc(sizeof(radix_node_t));
    new_node->prefix_bits = prefix_bits;
    new_node->prefix = (unsigned char*)str_dup(prefix);
    new_node->left_0 = NULL;
    new_node->right_1 = NULL;
    new_node->data = NULL;
    new_node->data_count = 0;
    return new_node;
}

/* Return the bit for a character at index 'pos' */
int getBit(char ch, int pos) {
    return (ch >> pos) & 1;
}

/* Insert key + data into radix tree */
void insert_2(radix_t *tree, void *data) {
    unsigned char *key = return_cafe_key(data);

    int total_key_bits = (strlen(key) * CHAR_BITS);

    if (!tree->root) {
        tree->root = create_node(key, total_key_bits);
        add_data_to_node(tree->root, data);
        return;
    }

    radix_node_t *curr = tree->root, *parent = NULL;
    int i = 0, j = 0, key_bit_pos = 7, node_bit_pos = 7, total_same_bits = 0, prefix_bits_checked = 0, start_i = 0;

    /* continue loop and traverse through tree
     * while key bits match node bits */
    while (curr && total_same_bits < total_key_bits &&
           prefix_bits_checked < curr->prefix_bits) {
        if (getBit(key[i], key_bit_pos) != getBit(curr->prefix[j], node_bit_pos)) {
            break;
        }
        total_same_bits++;
        key_bit_pos--;
        node_bit_pos--;
        prefix_bits_checked++;

        if (key_bit_pos < 0) {
            i++;
            key_bit_pos = 7;
        }
        if (node_bit_pos < 0) {
            j++;
            node_bit_pos = 7;
        }

         /* break out of loop if key fully matched*/
        if (total_same_bits == total_key_bits) {
            break;
        }

        /* all relevant bits of node matched - move to the next one */
        if (prefix_bits_checked == curr->prefix_bits) {
            start_i = total_same_bits;
            j = 0;
            parent = curr;

            /* move to left or right node depending on next key bit */
            if (getBit(key[i], key_bit_pos) == 0) {
                node_bit_pos = 7;
                prefix_bits_checked = 0;
                curr = curr->left_0;
            } else {
                node_bit_pos = 7;
                prefix_bits_checked = 0;
                curr = curr->right_1;
            }
            if (curr) {
                prefix_bits_checked = 0;
            }
        }
    }

    // Case 1: key is a duplicate
    if (total_same_bits == total_key_bits &&
        prefix_bits_checked == curr->prefix_bits) {
        add_data_to_node(curr, data);
        return;
    }

    unsigned char *key_remainder_prefix = splitStem(key, total_same_bits,
                                                    total_key_bits - 1);
    // Case 2: insert data into leaf node
    if (curr == NULL) {
        curr = create_node(key_remainder_prefix,
                           total_key_bits - total_same_bits);
        add_data_to_node(curr, data);
        if (getBit(key[total_same_bits / CHAR_BITS], key_bit_pos) == 0) {
            parent->left_0 = curr;
        } else {
            parent->right_1 = curr;
        }
        return;
    }

    int common_bits = prefix_bits_checked;
    int remainder_bits = total_key_bits - total_same_bits;

    unsigned char *common_prefix = splitStem(key, start_i,
                                             total_same_bits - 1);
    radix_node_t *new_parent_node = create_node(common_prefix,
                                                common_bits);

    /* Case 3: key partially matched node */
    if (prefix_bits_checked < curr->prefix_bits) {
        // key is a prefix of another; add its data to the new parent
        if (total_same_bits == total_key_bits) {
            add_data_to_node(new_parent_node, data);
            if (getBit(curr->prefix[total_same_bits / 8], node_bit_pos) == 1) {
                new_parent_node->right_1 = curr;
            }
            else {
                new_parent_node->left_0 = curr;
            }
        }
         /* create new parent node with common bits, new child 
          * nodes for key remainder and old node remainder */
        else {
            if (getBit(key[total_same_bits / 8], key_bit_pos) == 0) {
                new_parent_node->right_1 = curr;
                new_parent_node->left_0 = create_node(key_remainder_prefix,
                                                      remainder_bits);
                add_data_to_node(new_parent_node->left_0, data);
            }
            else {
                new_parent_node->left_0 = curr;
                new_parent_node->right_1 = create_node(key_remainder_prefix,
                                                       remainder_bits);
                add_data_to_node(new_parent_node->right_1, data);
            }
        }
        if (curr == new_parent_node->left_0 ||
            curr == new_parent_node->right_1) {
            curr->prefix = shift_prefix_bits(curr,
                                             total_same_bits - start_i);
            curr->prefix_bits -= common_bits;
        }
        if (!parent) {
            tree->root = new_parent_node;
        } else {
            if (parent->left_0 == curr) {
                parent->left_0 = new_parent_node;
            } else {
                parent->right_1 = new_parent_node;
            }
        }
    }
}

/* Given a node containting a prefix, shift it
 * to the left 'shift' amounts */
unsigned char *shift_prefix_bits(radix_node_t* node, int shift) {
    if (!node->prefix || shift <= 0) {
        return node->prefix;
    }

    int len = (node->prefix_bits % CHAR_BITS == 0) ?
            (node->prefix_bits / CHAR_BITS) :
            (node->prefix_bits / CHAR_BITS) + 1;
    int full_chars_shift = shift / CHAR_BITS;
    int bit_shift = shift % CHAR_BITS;

    for (int i = 0; i < len - full_chars_shift; ++i) {
        if (i + full_chars_shift < len) {
            node->prefix[i] = node->prefix[i + full_chars_shift];
        } else {
            node->prefix[i] = 0;
        }
    }

    for (int i = 0; i < len - 1; ++i) {
        node->prefix[i] = (node->prefix[i] << bit_shift) |
                (node->prefix[i + 1] >> (CHAR_BITS - bit_shift));
    }
    if(bit_shift != CHAR_BITS) {
        node->prefix[len - 1] <<= bit_shift;
    }

    // Reallocate memory if possible
    int new_len = len - full_chars_shift;
    if (full_chars_shift > 0 && new_len > 0) {
        unsigned char* new_str = (unsigned char *)realloc(node->prefix, new_len);
        if (new_str) {
            node->prefix = new_str;
            return new_str;
        }
    }

    return node->prefix;
}

/* Splits a key by its bits at a given sequence */
unsigned char* splitStem(unsigned char *key, int start, int end) {

    // Calculate the number of bits and bytes
    int length = end - start + 1;
    // Bytes needed to store the result
    int num_bytes = (length + 7) / CHAR_BITS;

    unsigned char *result = (unsigned char *)malloc(
            (num_bytes + 1) * sizeof(unsigned char));
    assert(result != NULL);

    // byte in key wherestart position is located
    int src_byte_idx = start / CHAR_BITS;
    // the bit offset within the start byte
    int src_bit_offset = start % CHAR_BITS;

    // shifting bits
    for (int i = 0; i < num_bytes; i++, src_byte_idx++) {
        result[i] = (key[src_byte_idx] << src_bit_offset) |
                    (key[src_byte_idx + 1] >> (CHAR_BITS - src_bit_offset));

    }

    // masking with 0s
    int excess_bits = (num_bytes * CHAR_BITS) - length;
    if (excess_bits > 0) {
        result[num_bytes - 1] &= ~((1 << excess_bits) - 1);
    }

    result[num_bytes] = 0x00;
    return result;
}

/* searches through tree and calling relevant
 * function if all bits of key is matched */
void** search(radix_t *tree, unsigned char *key, int *bit_comps,
                                                int *char_comps) {
    if (!tree->root) return NULL;
    int total_key_bits = (strlen(key) * CHAR_BITS);
    radix_node_t *curr = tree->root;

    int i = 0, j = 0, key_bit_pos = 7, node_bit_pos = 7,
    prefix_bits_checked = 0, total_same_bits = 0;

    while (curr && total_same_bits < total_key_bits &&
    prefix_bits_checked < curr->prefix_bits) {

        // every 8 bits checked is 1 character (includes the root with 0 % 8)
        if (prefix_bits_checked % CHAR_BITS == 0) {
            (*char_comps)++;
        }

        (*bit_comps)++;

        if (getBit(key[i], key_bit_pos) != getBit(curr->prefix[j],
                                                  node_bit_pos)) {
            return NULL;
        }

        total_same_bits++;
        key_bit_pos--;
        node_bit_pos--;
        prefix_bits_checked++;

        if (key_bit_pos < 0) {
            i++;
            key_bit_pos = 7;
        }
        if (node_bit_pos < 0) {
            j++;
            node_bit_pos = 7;
        }
        if (total_same_bits == total_key_bits && node_bit_pos == 7) {
            break;
        }

        if (prefix_bits_checked == curr->prefix_bits &&
            total_same_bits < total_key_bits) {
            if (curr->left_0 && curr->left_0->prefix_bits == 0) {
                void **results = collect_data(curr->left_0, char_comps);
                return results;
            }
            j = 0;
            if (getBit(key[i], key_bit_pos) == 0) {
                node_bit_pos = 7;
                prefix_bits_checked = 0;
                curr = curr->left_0;
            } else {
                node_bit_pos = 7;
                prefix_bits_checked = 0;
                curr = curr->right_1;
            }
            if (curr) {
                prefix_bits_checked = 0;
            } else {
                return NULL;
            }
        }
    }

// If all bits of key matched but current node has more bits
    if ((total_same_bits == total_key_bits) && ((prefix_bits_checked < 
                curr->prefix_bits) || (prefix_bits_checked % 8) == 0)) {
        void** results = collect_data(curr, char_comps);
        return results;
    }
    else if ((total_same_bits == total_key_bits) && (prefix_bits_checked == 
                                            curr->prefix_bits)) {
        void** results = collect_data(curr, char_comps);
        return results;
    }
    return NULL;
}

/* calls up a traversal function and allocates
 * appropriate amount for the results array*/
void** collect_data(radix_node_t *node, int *char_comps) {
    void** results = NULL;
    int count = 0;

    traverse_and_collect(node, &results, &count, char_comps);

    results = realloc(results, (count + 1) * sizeof(void*));
    results[count] = NULL;

    return results;
}

/* Traverses tree and adds every 'data' occurrence to results array */
void traverse_and_collect(radix_node_t *curr, void*** results, int* count, int *char_comps) {
    if (!curr) return;
    if (curr->data) {
        for (int i = 0; i < curr->data_count; i++) {
            *results = realloc(*results, (*count + 1) * sizeof(void*));
            if (!*results) {
                exit(1);
            }
            (*results)[*count] = curr->data[i];
            (*count)++;
        }
    }
    traverse_and_collect(curr->left_0, results, count, char_comps);
    traverse_and_collect(curr->right_1, results, count, char_comps);
}
