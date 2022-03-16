#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top; // Next avabile index in the STACK
    uint32_t capacity; // Max size of the STACK
    Node **items; // ARRAY of NODES
};

// Allocated the Memory for our stack according to the @param capacity
Stack *stack_create(uint32_t capacity) {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    if (stack) {
        stack->top = 0;
        stack->capacity = capacity;
        stack->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!stack->items) {
            free(stack);
            stack = NULL;
        }
    }
    return stack;
}

// De-allocates and de-references the stack
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

// Gets the size of the stack
uint32_t stack_size(Stack *s) {
    return s->top;
}

// Checks if the stack is full
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    } else {
        return false;
    }
}

// Checks if the stack is empty
bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    } else {
        return false;
    }
}

// Pushes an element to the top of the stack
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    } else {
        s->items[s->top] = n;
        s->top += 1;
        return true;
    }
}

// Looks at the value at the top of the stack
bool stack_peek(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    } else {
        **n = *(s->items[s->top - 1]);
        return true;
    }
}

// Removes the value at the top of the stack
// Sets parameterized pointer to point to the removed value
bool stack_pop(Stack *s, Node **n) {
    if (s->top == 0) {
        return false;
    } else {
        *n = s->items[s->top - 1];
        s->top -= 1;
        return true;
    }
}

// Debug tool to print the STACK
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        node_print(s->items[i]);
        if (i + 1 != s->top) {
            printf(" -> ");
        }
    }
    printf("\n");
}
