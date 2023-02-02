/**
 * @file stack.c
 * @brief FIFO stack.
 */
#pragma once

/**
 * @brief A stack wrapper of a void**, accessable as the `elements` field. Operations are
 *        functions with a "sk" prefix. The `len` field will be changed accordingly by
 *        modifying functions. The `_size` field is for internal use.
 */
typedef struct {
    int _size;
    int len;
    void **elements;
} Stack;

/**
 * @brief Create a new stack, with a given starting length.
 */
Stack *sknew(int length);

/**
 * @brief Free the memory of a stack.
 */
void skdel(Stack *stack);

/**
 * @brief Push a value on top of a stack.
 */
void skpush(Stack *stack, void *value);

/**
 * @brief Pop a value from the top of a stack.
 */
void skpop(Stack *stack, void **databuf);

/**
 * @brief Print a stack's elements to std
 */
void skprint(Stack *stack);
