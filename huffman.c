#include "huffman.h"
#include "stack.h"
#include "pq.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Builds the HUFFMAN TREE
// Constructs a PriorityQueue, dequeues two children, enqueues the new parent
// Once there is only one NODE left, return a pointer to that NODE
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq;
    Node *node;
    pq = pq_create(ALPHABET);

    for (int i = 0; i < ALPHABET; i += 1) {
        if (hist[i] > 0) {
            node = node_create(i, hist[i]);
            enqueue(pq, node);
        }
    }

    Node *left;
    Node *right;
    Node *parent;
    Node *root;
    while (pq_size(pq) > 1) {
        dequeue(pq, &left);
        dequeue(pq, &right);
        parent = node_join(left, right);
        enqueue(pq, parent);
    }
    dequeue(pq, &root);
    pq_delete(&pq);
    return root;
}

// Recursive function for building codes
// Pre-Order traversal of the HUFFMAN TREE
// Visiting the left child pushes a 0 to the code
// Visiting the right child pushes a 1 to the code
// Once a leaf node has been found, set the parameterized code to the new code
void recursive_build_codes(Node *root, Code table[static ALPHABET], Code c) {
    uint8_t bit;
    if (root != NULL) {
        if ((root->left == NULL) && (root->right == NULL)) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 1);
            recursive_build_codes(root->right, table, c);
            code_pop_bit(&c, &bit);

            code_push_bit(&c, 0);
            recursive_build_codes(root->left, table, c);
            code_pop_bit(&c, &bit);
        }
    }
}

// Calls the recursive_build_codes
void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    recursive_build_codes(root, table, c);
    return;
}

// Dumps the directions of the HUFFMAN TREE to the outfile
// L implies the next character is a leaf
// I implies it is a parent node, pop twice and re-queue
void dump_tree(int outfile, Node *root) {
    static uint8_t dump_buf[BLOCK];
    static uint32_t nbytes = 0;
    if (root != NULL) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);

        if ((root->left == NULL) && (root->right == NULL)) {
            // LEAF NODE
            dump_buf[nbytes] = 'L';
            write(outfile, dump_buf + nbytes, 1);
            nbytes += 1;

            dump_buf[nbytes] = root->symbol;
            write(outfile, dump_buf + nbytes, 1);
            nbytes += 1;
        } else {
            dump_buf[nbytes] = 'I';
            write(outfile, dump_buf + nbytes, 1);
            nbytes += 1;
        }
    }
    return;
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    // scan over dumped tree
    // if we hit an L then the next character is a leaf
    //      leave its frequency as zero on the stack
    // resume scanning from after the leaf until we've hit an I
    // If we hit an I, pop the stack twice and join the nodes
    // push the new joined node onto the stack
    // The last node in the stack is the root of the huffman tree
    Node *left;
    Node *right;
    Node *parent;
    Stack *stack;
    stack = stack_create(nbytes);
    for (uint16_t i = 0; i < nbytes; i += 1) {
        if (tree[i] == 'L') {
            stack_push(stack, node_create(tree[i + 1], 0));
            i += 1; // This skips the character inserted
        } else if (tree[i] == 'I') {
            stack_pop(stack, &right);
            stack_pop(stack, &left);
            stack_push(stack, node_join(left, right));
        }
    }
    stack_pop(stack, &parent);
    stack_delete(&stack);
    return parent;
}

// Deletes the HUFFMAN TREE
void delete_tree(Node **root) {
    if (root != NULL && (*root) != NULL) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        node_delete(root);
    }
    return;
}
