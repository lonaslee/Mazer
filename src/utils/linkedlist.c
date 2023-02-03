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
/// @returns nevernull | noreturn
static inline LLNode *nodeat(LinkedList *list, llsize_t idx) {
#if LLBOUND_CHECK
    if (list->length == 0) LLERROR("nodeat list has no elements")
#endif
    LLFOREACHINDEXED(i, cur, list) {
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
#if LLBOUND_CHECK
    if (index >= list->length) LLERROR("get index bigger than last index")
#endif
    return nodeat(list, index)->data;
}

void llset(LinkedList *list, llsize_t index, void *data) {
#if LLBOUND_CHECK
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
#if LLBOUND_CHECK
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
#if LLBOUND_CHECK
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
#if LLBOUND_CHECK
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
    LLFOREACH(cur, list) {
        cur->data = NULL;
    }
}

llsize_t llcount(LinkedList *list, void *value) {
    llsize_t count = 0;
    LLFOREACH(cur, list) {
        if (cur->data == value) count++;
    }
    return count;
}

llsize_t llindex(LinkedList *list, void *value) {
    llsize_t i;
    LLFOREACH(cur, list) {
        if (cur->data == value) return i;
    }
    return llnpos;
}

llsize_t lllastindex(LinkedList *list, void *value) {
    LLFORREACHINDEXED(i, cur, list) {
        if (cur->data == value) return i;
    }
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

void llcopy(LinkedList *src, LinkedList *dst) {
    LLFOREACHINDEXED(i, cur, src) {
        if (dst->length == i) {
            llappend(dst, cur->data);
        } else {
            llset(dst, i, cur->data);
        }
    }
    while (dst->length > src->length) {
        llpop(dst);
    }
}

void lljoin(LinkedList *list, LinkedList *other) {
    if (list->last != NULL) list->last->next = other->first;
    if (other->first != NULL) other->first->prev = list->last;
    list->length += other->length;
    free(other);
}

LinkedList *llnsublist(LinkedList *list, llsize_t start, llsize_t length) {
#if LLBOUND_CHECK
    if (start + length > list->length) LLERROR("nsublist range out of bounds")
#endif
    LinkedList *sub = llnew();
    if (length == 0 || list->length == 0) return sub;
    LLNode *cur = nodeat(list, start);
    for (llsize_t i = 0; i < length; cur = cur->next, i++)
        llappend(sub, cur->data);
    return sub;
}

LinkedList *llsublist(LinkedList *list, llsize_t start, llsize_t end) {
#if LLBOUND_CHECK
    if (end > list->length) LLERROR("sublist range out of bounds")
    if (end < start) LLERROR("sublist end larger than start")
#endif
    return llnsublist(list, start, end - start);
}

void llprint(LinkedList *list) {
    printf("Linked List of length %d at %p\n", list->length, list);
    LLFOREACHINDEXED(i, cur, list) {
        printf("%d\t%p\n", i, cur->data);
    }
}

void llprintvalues(LinkedList *list, void (*printer)(const void *)) {
    printf("Linked List of length %d at %p\n", list->length, list);
    LLFOREACHINDEXED(i, cur, list) {
        printf("%d\t%p\t", i, cur->data);
        printer(cur->data);
        printf("\n");
    }
}

/* internal mergesort helpers */

static LLNode *nodemerge(LLNode *left, LLNode *right, llcmpfunc cmp) {
    if (left == NULL) return right;
    if (right == NULL) return left;
    if (cmp(left->data, right->data) < 0) {
        left->next = nodemerge(left->next, right, cmp);
        left->next->prev = left;
        left->prev = NULL;
        return left;
    } else {
        right->next = nodemerge(left, right->next, cmp);
        right->next->prev = right;
        right->prev = NULL;
        return right;
    }
}

static LLNode *split(LLNode *first) {
    LLNode *fast = first;
    LLNode *second = first;
    while (fast->next != NULL && fast->next->next != NULL) {
        fast = fast->next->next;
        second = second->next;
    }
    LLNode *tmp = second->next;
    second->next = NULL;
    return tmp;
}

static LLNode *nodemergesort(LLNode *first, llcmpfunc cmp) {
    if (first == NULL || first->next == NULL) return first;
    LLNode *second = split(first);
    first = nodemergesort(first, cmp);
    second = nodemergesort(second, cmp);
    return nodemerge(first, second, cmp);
}

void llmergesort(LinkedList *list, llcmpfunc cmp) {
    if (list->length <= 1) return;
    LLNode *left = list->first;
    LLNode *right = split(list->first);

    left = nodemergesort(left, cmp);
    right = nodemergesort(right, cmp);
    list->first = nodemerge(left, right, cmp);
    while (list->last->next != NULL) list->last = list->last->next;
}

/* quicksort helpers */

static LLNode *partition(LLNode *start, LLNode *end, llcmpfunc cmp) {
    void *pivot = end->data;
    LLNode *i = start->prev;
    LLNode *j = start;

    while (j != end) {
        if (cmp(j->data, pivot) <= 0) {
            if (i == NULL)
                i = start;
            else
                i = i->next;

            void *tmp = i->data;
            i->data = j->data;
            j->data = tmp;
        }
        j = j->next;
    }
    if (i == NULL)
        i = start;
    else
        i = i->next;
    void *tmp = i->data;
    i->data = end->data;
    end->data = tmp;
    return i;
}

static void subquicksort(LLNode *l, LLNode *h, llcmpfunc cmp) {
    if (h != NULL && l != h && l != h->next) {
        LLNode *j = partition(l, h, cmp);
        subquicksort(l, j->prev, cmp);
        subquicksort(j->next, h, cmp);
    }
}

void llquicksort(LinkedList *list, llcmpfunc cmp) {
    if (list->length <= 1) return;
    subquicksort(list->first, list->last, cmp);
}

LLNode *llfind(LinkedList *list, const void *key) {
    LLFOREACH(cur, list) {
        if (cur->data == key) return cur;
    }
    return NULL;
}

LLNode *llfindlast(LinkedList *list, const void *key) {
    LLFORREACH(cur, list) {
        if (cur->data == key) return cur;
    }
    return NULL;
}

#undef LLERROR
