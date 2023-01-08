#include "linkedlist.h"

#include <stdio.h>
#include <stdlib.h>

/* internal */

/** print a linked list error and abort. this macro is undef'd at end */
#define LLERROR(msg)                               \
    do {                                           \
        puts("Linked List error: " msg ". abort"); \
        abort();                                   \
    } while (0);

/// @brief alloc new node from param, must be deallocated later
/// @return nevernull
static inline LLNode *newnode(LLNode *prev, LLNode *next, void *data) {
    LLNode *node = calloc(1, sizeof(LLNode));
    node->prev = prev;
    node->next = next;
    node->data = data;
    return node;
}

/// @brief dealloc node and return the data it held
static inline void *delnode(LLNode *node) {
    void *data = node->data;
    free(node);
    return data;
}

/// @brief return node at given index
/// @return nevernull | noreturn
static inline LLNode *nodeat(LinkedList *list, llsize_t idx){
    LLFOREACHINDEXED(i, cur, list){
        printf("idx : %d\n", i);
if (i == idx) return cur;
}
LLERROR("nodeat index higher than list length")
}

/* public */

LinkedList *llnew() {
    LinkedList *list = calloc(1, sizeof(LinkedList));
    list->first = NULL;
    list->last = NULL;
    list->length = 0;
    return list;
}

void lldel(LinkedList *list) {
    for (LLNode *cur = list->first; cur != NULL;) {
        LLNode *next = cur->next;
        delnode(cur);
        cur = next;
    }
    free(list);
}

void *llget(LinkedList *list, llsize_t index) {
#if BOUND_CHECK
    if (index >= list->length) LLERROR("get index bigger than last index")
#endif
    return nodeat(list, index)->data;
}

void llset(LinkedList *list, llsize_t index, void *data) {
#if BOUND_CHECK
    if (index >= list->length) LLERROR("set index bigger than last index")
#endif
    nodeat(list, index)->data = data;
}

void llappend(LinkedList *list, void *data) {
    LLNode *node = newnode(list->last, NULL, data);
    list->length++;
    if (list->last == NULL) {
        list->first = list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }
}

void *llpop(LinkedList *list) {
#if BOUND_CHECK
    if (list->length == 0) LLERROR("pop list has no elements")
#endif
    list->length--;
    LLNode *last = list->last;
    if (list->length == 0) {
        list->first = list->last = NULL;
    } else {
        list->last = list->last->prev;
        list->last->next = NULL;
    }
    return delnode(last);
}

void llinsert(LinkedList *list, llsize_t index, void *data) {
#if BOUND_CHECK
    if (index > list->length) LLERROR("insert index higher than list length")
#endif
    if (index == list->length) {
        llappend(list, data);
        return;
    }
    list->length++;
    LLNode *node = nodeat(list, index);
    LLNode *new = newnode(node->prev, node, data);
    if (new->prev != NULL)
        new->prev->next = new;
    else
        list->first = new;
    node->prev = new;
}

void *llremove(LinkedList *list, llsize_t index) {
#if BOUND_CHECK
    if (index >= list->length) LLERROR("remove index higher than last index")
    if (list->last == 0) LLERROR("remove list has no elements")
#endif
    if (index == list->length - 1) {
        return llpop(list);
    }
    list->length--;
    LLNode *node = nodeat(list, index);
    if (node->prev != NULL) node->prev->next = node->next;
    if (node->next != NULL) node->next->prev = node->prev;
    return delnode(node);
}

void llclear(LinkedList *list) {
    LLFOREACH(cur, list)
    cur->data = NULL;
}

llsize_t llcount(LinkedList *list, void *value) {
    llsize_t count = 0;
    LLFOREACH(cur, list)
    if (cur->data == value) count++;
    return count;
}

llsize_t llindex(LinkedList *list, void *value) {
    llsize_t i;
    LLFOREACH(cur, list)
    if (cur->data == value) return i;
    return llnpos;
}

llsize_t lllastindex(LinkedList *list, void *value) {
    LLFORREACHINDEXED(i, cur, list)
    if (cur->data == value) return i;
    return llnpos;
}

void llreverse(LinkedList *list) {
    LLNode *tmp = NULL;
    for (LLNode *cur = list->first; cur != NULL; cur = cur->prev) {
        tmp = cur->prev;
        cur->prev = cur->next;
        cur->next = tmp;
    }
    if (tmp != NULL) list->first = tmp->prev;
}

void llprint(LinkedList *list) {
    printf("Linked List of %d length at %p\n", list->length, list);
    int i;
    LLNode *cur;
    for (cur = list->first, i = 0; cur != NULL; cur = cur->next, i++)
        printf("%d\t%p\n", i, cur->data);
}

void llmergesort(LinkedList* list) {
    // TODO
}

void llquicksort(LinkedList* list) {
    // TODO
}

#undef LLERROR
