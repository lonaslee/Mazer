#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

Stack *sknew(int size) {
    Stack *stack = calloc(1, sizeof(Stack));
    stack->_size = size;
    stack->len = 0;
    stack->elements = calloc(size, sizeof(void *));
    return stack;
}

void skdel(Stack *stack) {
    free(stack->elements);
    free(stack);
}

void skpush(Stack *stack, void *value) {
    if (stack->len == stack->_size) {
        stack->elements = realloc(stack->elements, (stack->_size *= 2) * sizeof(void *));
        memset(&stack->elements[stack->len], 0, (stack->_size / 2) * sizeof(void *));
    }
    stack->elements[stack->len++] = value;
}

void skpop(Stack *stack, void **databuf) {
    if (!stack->len) {
        if (databuf) *databuf = NULL;
        return;
    }
    stack->len--;
    if (databuf) *databuf = stack->elements[stack->len];
    stack->elements[stack->len] = NULL;
}

void skprint(Stack *stack) {
    printf("\nStack at %p (size: %d, length: %d)\n\n", stack, stack->_size, stack->len);
    for (int i = stack->len - 1; i >= 0; i--)
        printf("%3d  %p\n", i, stack->elements[i]);
    puts("");
}
