/**
 * @file array.c
 * @brief Wrapper for array providing access and modifying functions ("ar" prefixed).
 */
#pragma once

typedef unsigned int arsize_t;  //< Numeric size of Array length and indices.

#define ARBOUND_CHECK 1  //< Whether to do bound checking and exiting with an error message, or segfaulting.

/**
 * @brief Array of unfixed size. Access and modifications are provided by
 *        functions with the "ar" prefix.
 */
typedef struct {
    void **elems;    //< Underlying storage field. Indicing can segfault.
    arsize_t len;    //< Number of elements currently stored, updated by modifying functions.
    arsize_t _size;  //< Internal field for reallocation uses.
} Array;

/**
 * @brief Allocate a new Array with an initial size.
 */
Array *arnew(arsize_t size);

/**
 * @brief Deallocate an Array.
 */
void ardel(Array *array);

/**
 * @brief Get the value of an array at a specific index, performing bound
 *        checking if ARBOUND_CHECK is 1.
 */
void *arget(Array *array, arsize_t index);

/**
 * @brief Set the value of an array at a specific index, performing bound
 *        checkign if ARBOUND_CHECK is 1.
 */
void arset(Array *array, arsize_t index, void *value);

/**
 * @brief Add an element to the back of an Array.
 */
void arappend(Array *array, void *element);

/**
 * @brief Remove and return the last element of an Array, or NULL if
 *        the array does not have any elements.
 */
void *arpop(Array *array);

/**
 * @brief Insert an element at a specified index.
 */
void arinsert(Array *array, arsize_t index, void *value);

/**
 * @brief Remove and return the element at a specified index.
 */
void *arremove(Array *array, arsize_t index);

/**
 * @brief Display a representation of an array to stdout.
 */
void arprint(Array *array);