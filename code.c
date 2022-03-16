#include "defines.h"
#include "code.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

Code code_init(void) {
    Code c = { .top = 0, .bits = { 0 } };
    return c;
}

uint32_t code_size(Code *c) {
    return c->top;
}

bool code_empty(Code *c) {
    return (c->top == 0);
}

bool code_full(Code *c) {
    return (c->top == ALPHABET);
}

// Bit Arithmetic to set the i-th bit in a byte
// @credit: bv8 from Code Comments respository
bool code_set_bit(Code *c, uint32_t i) {
    return c->bits[i / 8] |= ((uint32_t) 0x1 << i % 8);
}

// Bit Arithmetic to clear the i-th bit in a byte
// @credit: bv8 from Code Comments respository
bool code_clr_bit(Code *c, uint32_t i) {
    return c->bits[i / 8] &= ~((uint32_t) 0x1 << i % 8);
}

// Bit Arithmetic to get the i-th bit in a byte
// @credit: bv8 from Code Comments respository
bool code_get_bit(Code *c, uint32_t i) {
    return (c->bits[i / 8] >> i % 8) & 0x1;
}

// Push a bit to the end of the code
bool code_push_bit(Code *c, uint8_t bit) {
    if (!code_full(c)) {
        if (bit) {
            code_set_bit(c, c->top);
        } else {
            code_clr_bit(c, c->top);
        }
        c->top += 1;
        return true;
    } else {
        return false;
    }
}

// Pop a bit from the end of the code
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (!code_empty(c)) {
        *bit = code_get_bit(c, c->top - 1);
        c->top -= 1;
        return true;
    } else {
        return false;
    }
}

// Debug tool to print the Code ADT
void code_print(Code *c) {
    printf("Code ADT:\n Top: %u\n Code: ", c->top);
    for (uint32_t i = 0; i < c->top; i += 1) {
        printf("%d", code_get_bit(c, i));
    }
    printf("\n");
    return;
}
