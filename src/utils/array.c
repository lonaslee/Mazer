#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** print an Array error and abort. this macro is undef'd at end */
#define ARERROR(msg)                               \
    do {                                           \
        puts("Array error: " msg ". abort"); \
        abort();                                   \
    } while (0);

#define GROW_ARRAY(array)                                                           \
    do {                                                                            \
        array->elems = realloc(array->elems, (array->_size *= 2) * sizeof(void *)); \
        memset(&array->elems[array->len], 0, (array->_size / 2) * sizeof(void *));  \
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

void *arget(Array *array, arsize_t index) {
#if ARBOUND_CHECK
    if (index >= array->len) ARERROR("get index out of bounds")
#endif
    return array->elems[index];
}

void arset(Array *array, arsize_t index, void *value) {
#if ARBOUND_CHECK
    if (index >= array->len) ARERROR("set index out of bounds")
#endif
    array->elems[index] = value;
}

void arappend(Array *array, void *value) {
    if (array->len >= array->_size)
        GROW_ARRAY(array);
    array->elems[array->len++] = value;
}

void *arpop(Array *array) {
    if (array->len == 0) return NULL;
    return array->elems[--array->len];
}

void arinsert(Array *array, arsize_t index, void *value) {
#if ARBOUND_CHECK
    if (index > array->len) ARERROR("insert out of bounds")
#endif

    if (array->len >= array->_size)
        GROW_ARRAY(array);
    if (array->len > 0)
        for (int i = array->len - 1; i >= index && i >= 0; i--)
            array->elems[i + 1] = array->elems[i];
    array->elems[index] = value;
    array->len++;
}

void *arremove(Array *array, arsize_t index) {
}

void arprint(Array *array) {
    printf("Array of length %d, size %d at %p\n", array->len, array->_size, array);
    for (int i = 0; i < array->len; i++)
        printf("%d\t%p\n", i, array->elems[i]);
}

#undef ARERROR
