#include "pq.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct PriorityQueue {
    uint64_t capacity; // MAX SIZE of QUEUE
    uint32_t size; // NUMBER of NODES int the QUEUE
    Node **nodes; // ARRAY of NODE POINTERS
};

// CONSTRUCTOR for PQ
PriorityQueue *pq_create(uint32_t capacity) {
    capacity += 1;
    PriorityQueue *q = (PriorityQueue *) calloc(capacity, sizeof(PriorityQueue));
    if (q) {
        q->capacity = capacity;
        q->size = 0;
        q->nodes = (Node **) calloc(capacity, sizeof(Node *));
        if (!q->nodes) {
            free(q);
            q = NULL;
        }
    }
    return q;
}

// DESCRUCTOR for PQ
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->nodes) {
        for (uint64_t i = 1; i <= (*q)->size; i += 1) {
            node_delete(&((*q)->nodes[i]));
        }
        free((*q)->nodes);
        free(*q);
        *q = NULL;
    }
    return;
}

bool pq_empty(PriorityQueue *q) {
    return q->size == 0;
}

bool pq_full(PriorityQueue *q) {
    return q->size == q->capacity - 1;
}

uint32_t pq_size(PriorityQueue *q) {
    return q->size;
}

// COMPARATOR for ENQUEUE and DEQUEUE
int compare(Node *x, Node *y) {
    // ZERO means Node D.N.E. ( CALLOC )
    if (x == 0) {
        return -1;
    } else if (x->frequency > y->frequency) {
        return 1;
    } else if (x->frequency < y->frequency) {
        return -1;
    } else {
        return 0;
    }
}

bool enqueue(PriorityQueue *q, Node *n) {
    if (q->size + 1 == q->capacity) {
        return false;
    }
    uint64_t child = ++q->size;
    q->nodes[child] = n;

    // ITERATIVELY HEAPIFY the PQ from BOTTOM-UP
    for (; compare(q->nodes[child / 2], q->nodes[child]) > 0; child /= 2) {
        q->nodes[child] = q->nodes[child / 2];
    }
    q->nodes[child] = n;

    return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    if (q->size - 1 == 0) {
        *n = q->nodes[q->size];
        q->size -= 1;
        return true;
    } else {
        // SWAP FIRST and LAST
        Node *temp;
        temp = q->nodes[1];
        q->nodes[1] = q->nodes[q->size];
        q->nodes[q->size] = temp;

        // Take the LAST item and REMOVE it
        *n = q->nodes[q->size];
        q->size -= 1;

        uint64_t child, parent = 1;
        temp = q->nodes[parent];

        // ITERATIVELY HEAPIFY the PQ from the TOP
        for (; parent * 2 <= q->size; parent = child) {
            child = parent * 2;
            if (child != q->size && compare(q->nodes[child + 1], q->nodes[child]) < 0) {
                child += 1;
            }
            if (compare(q->nodes[child], temp) < 0) {
                q->nodes[parent] = q->nodes[child];
            } else {
                break;
            }
        }
        q->nodes[parent] = temp;
        return true;
    }
}

void pq_print(PriorityQueue *q) {
    for (uint64_t i = 1; i <= q->size; i += 1) {
        printf("i %lu: ", i);
        node_print(q->nodes[i]);
    }
    return;
}
