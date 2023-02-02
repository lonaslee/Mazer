#pragma once
/**
 * @file linkedlist.c
 * @brief Doubly linked list implementation, providing access and modifying
 *        functions ("ll" prefixed).
 */

#include <limits.h>

/** Whether functions should error out or segfault. */
#define LLBOUND_CHECK 1

/// @brief Numeric type of linked list size and indices.
typedef unsigned int llsize_t;

/// @brief Number returned when linked list functions fail.
const llsize_t llnpos = UINT_MAX;

/// @brief Give a raw name (no need to declare) and a LinkedList pointer, and
///        follow with a block, using name to access each element.
#define LLFOREACH(name, list) \
    for (LLNode *name = list->first; name != NULL; name = name->next)

/// @brief Give two raw names (no need to declare) and a LinkedList pointer, and follow
///        with a block, using name one to access index and name two to acces each element.
#define LLFOREACHINDEXED(numname, name, list)                 \
    for (llsize_t numname = 0, once = 1; once == 1; once = 0) \
        for (LLNode *name = list->first; name != NULL; name = name->next, numname++)

/// @brief reversed LLFOREACH
#define LLFORREACH(name, list) \
    for (LLNode *name = list->last; name != NULL; name = name->prev)

/// @brief reversed LLFOREACHINDEXED
#define LLFORREACHINDEXED(numname, name, list)                               \
    for (llsize_t numname = list->length - 1, once = 1; once == 1; once = 0) \
        for (LLNode *name = list->last; name != NULL; name = name->prev, numname--)

/// @brief Comparison function that is given the data of two nodes.
///        It must return negative for lesser, 0 for equal, and positive for greater.
typedef int (*llcmpfunc)(const void *, const void *);

/**
 * @brief A doubly linked list. Access and modifications are
 *        provided by functions with the `ll` prefix.
 */
typedef struct {
    struct LLNode *first;
    struct LLNode *last;
    llsize_t length;
} LinkedList;

/// Indicate that a LinkedList* can be null.
typedef LinkedList *NullableLinkedListPtr;

typedef struct LLNode {
    struct LLNode *prev;
    struct LLNode *next;
    void *data;
} LLNode;

/**
 * @brief Allocate a linked list.
 *        It must be freed with the `lldel` function.
 */
LinkedList *llnew();

/**
 * @brief Deallocate a linked list.
 */
void lldel(LinkedList *list);

/**
 * @brief Get the data of the node at a specified index in a LinkedList.
 */
void *llget(LinkedList *list, llsize_t index);

/**
 * @brief Set the data of the node at a specified index to a
 *        new value in a LinkedList.
 */
void llset(LinkedList *list, llsize_t index, void *data);

/**
 * @brief Create and append a new node with data to a LinkedList.
 */
void llappend(LinkedList *list, void *data);

/**
 * @brief Remove the last element of a linked list, and return its data.
 */
void *llpop(LinkedList *list);

/**
 * @brief Create and insert a new node with data
 *        to the specified index of a LinkedList.
 */
void llinsert(LinkedList *list, llsize_t index, void *data);

/**
 * @brief Remove and return the data of a node
 *        at the specified index of a LinkedList.
 */
void *llremove(LinkedList *list, llsize_t index);

/**
 * @brief Set all data fields of a LinkedList to NULL;
 */
void llclear(LinkedList *list);

/**
 * @brief Return number of occurrences a given value occurs in
 *        all nodes of a LinkedList.
 */
llsize_t llcount(LinkedList *list, void *value);

/**
 * @brief Return the first index a given value occurs in a LinkedList, or llnpos
 */
llsize_t llindex(LinkedList *list, void *value);

/**
 * @brief Return the last index a given value occurs in a LinkedList, or llnpos
 */
llsize_t lllastindex(LinkedList *list, void *value);

/**
 * @brief Reverse a LinkedList in place.
 */
void llreverse(LinkedList *list);

/**
 * @brief Copy the data of each node of a linked list to another, adding more
 *        nodes to the other as neccesary.
 */
void llcopy(LinkedList *src, LinkedList *dst);

/**
 * @brief Link the end of the first list to the start of the other, and
 *        deallocate the other.
 */
void lljoin(LinkedList *list, LinkedList *other);

/**
 * @brief Creat a sublist of length new nodes from start (inclusive).
 */
LinkedList *llnsublist(LinkedList *list, llsize_t start, llsize_t length);

/**
 * @brief Create a sublist of new nodes from start (inclusive) to end (exclusive).
 */
LinkedList *llsublist(LinkedList *list, llsize_t start, llsize_t end);

/**
 * @brief Print a representation of a linked list to stdout.
 */
void llprint(LinkedList *list);

/**
 * @brief Print a linked list, using a given function to print
 *        values at each node.
 *        The printer function must not advance a line.
 */
void llprintvalues(LinkedList *list, void (*printer)(const void *));

/**
 * @brief Mergesort a LinkedList in place.
 */
void llmergesort(LinkedList *list, llcmpfunc cmp);

/**
 * @brief Quicksort a LinkedList in place. Note that this swaps the values,
 *        not the nodes themselves.
 */
void llquicksort(LinkedList *list, llcmpfunc cmp);

/**
 * @brief Search a sorted LinkedList for a given key,
 *        and return its node, or NULL.
 */
LLNode *llfind(LinkedList *list, const void *key);
