#pragma once
/**
 * @file linkedlist.c
 * @brief Doubly linked list implementation.
 */

/** Whether functions should error out or segfault. */
#define BOUND_CHECK 1

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

/**
 * @brief A doubly linked list. Access and modifications are
 *        provided by functions with the `ll` prefix.
 */
typedef struct {
    struct LLNode *first;
    struct LLNode *last;
    llsize_t length;
} LinkedList;

typedef struct LLNode {
    struct LLNode *prev;
    struct LLNode *next;
    void *data;
} LLNode;

/**
 * @brief Allocate a linked list.
 *        It must be freed with the `lldel` function.
 * @returns nevernull
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
 * @brief Print a representation of a linked list to stdout.
 */
void llprint(LinkedList *list);

/**
 * @brief Mergesort a LinkedList in place.
 */
void llmergesort(LinkedList *list);

/**
 * @brief Quicksort a LinkedList in place.
 */
void llquicksort(LinkedList *list);
