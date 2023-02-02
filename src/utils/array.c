#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** print an Array error and abort. this macro is undef'd at end */
#define ARERROR(msg)                               \
    do {                                           \
        puts("Linked List error: " msg ". abort"); \
        abort();                                   \
    } while (0);

Array *arnew(arsize_t size) {
    if (size == 0) ARERROR("new size is 0")
    Array *array = calloc(1, sizeof(Array));
    if (array == NULL) ARERROR("failed new allocation")
    array->elems = calloc(size, sizeof(void *));
    if (array->elems == NULL) ARERROR("failed elems allocation")
    array->len = 0;
    array->_size = size;
}

void ardel(Array *array) {
    free(array->elems);
    free(array);
}

void arappend(Array *array, void *value) {
    if (array->len >= array->_size) {
        array->elems = realloc(array->elems, (array->_size *= 2) * sizeof(void *));
        memset(&array->elems[array->len], 0, (array->_size / 2) * sizeof(void *));
    }
    array->elems[array->len++] = value;
}

void *arpop(Array *array) {
    if (array->len == 0) return NULL;
    return array->elems[--array->len];
}

#undef ARERROR
