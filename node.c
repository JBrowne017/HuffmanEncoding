#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// CONSTRUCTOR for NODE
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->symbol = symbol;
    n->frequency = frequency;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// DESTRUCTOR for NODE
void node_delete(Node **n) {
    free((*n)->left);
    free((*n)->right);
    (*n)->left = NULL;
    (*n)->right = NULL;
    free(*n);
    *n = NULL;
    return;
}

uint64_t node_frequency(Node *n) {
    return n->frequency;
}

uint8_t node_symbol(Node *n) {
    return n->symbol;
}

// JOINS two NODES together with symbol $ and combined frequencies of children
Node *node_join(Node *left, Node *right) {
    Node *n;
    n = node_create('$', (node_frequency(left) + node_frequency(right)));
    n->right = right;
    n->left = left;
    return n;
}

// Debug tool to print the NODE ADT
void node_print(Node *n) {
    printf("Node %c: %lu\n", node_symbol(n), node_frequency(n));
    return;
}
