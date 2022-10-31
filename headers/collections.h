/**
 * @file collections.c
 * @brief Collections of Stack, Tree, and Linked List.
 */
#pragma once

/* * * * *\
 * Stack *
\* * * * */

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

/* * * * *\
 * Tree  *
\* * * * */

/**
 * @brief A Node in a Tree. Fields are managed by functions with a "tr" prefix.
 */
typedef struct TNode {
    void *data;
    struct TNode *nextsib;
    struct TNode *child;
    struct TNode *parent;
} TNode;

/**
 * @brief Create a new tree with a first node.
 * @return TNode* - tip of the tree, new node created.
 */
TNode *trnew(void *data);

/**
 * @brief Recursively delete all siblings and children of a node.
 */
void trdel(TNode *node);

/**
 * @brief Append a new node to this layer of the tree under the parent of the given node.
 */
TNode *trappendsib(TNode *node, void *data);

/**
 * @brief Remove the last node in this layer of the tree under the parent of the given node.
 */
void *trshavesib(TNode *node);

/**
 * @brief Append a child to the given node's linked list of children, or create the linked
 *        list if the node has no children. This is equivalent to calling `trappendsib` on
 *        the node's children.
 * @return TNode* - the new child created
 */
TNode *trappendchild(TNode *node, void *data);

/**
 * @brief Remove the last child of the given node's linked list of children, and delete the linked
 *        list if it is the last child of the node. This is equivalent to calling `trshavesib` on
 *        the node's children.
 */
void *trshavechild(TNode *node);

/**
 * @brief Append a tree as a sibling to another tree.
 */
void trappendsibtree(TNode *tree, TNode *subtree);

/**
 * @brief Append a tree as a child to another tree.
 */
void trappendchildtree(TNode *tree, TNode *subtree);

/**
 * @brief Get the very top root of a node.
 */
TNode *trgetroot(TNode *node);

/**
 * @brief Print a representation of the tree from the given tip.
 */
void trprint(TNode *tip);

/* * * * * * * *\
 * Linked List *
\* * * * * * * */

/**
 * @brief A node in a linked list.
 */
typedef struct LLNode {
    void *data;
    struct LLNode *next;
} LLNode;

/**
 * @brief A linked list. Operations on it are in functions with a "ll" prefix in its name.
 *        The `len` field will always be set accordingly by modifying functions.
 */
typedef struct {
    int len;
    LLNode *first;
    LLNode *last;
} LinkedList;

/**
 * @brief Create a new linked list with a first node with given data.
 */
LinkedList *llnew(void *data);

/**
 * @brief Free the memory of a linked list.
 */
void lldel(LinkedList *llist);

/**
 * @brief Get the node at an index of a linked list.
 *
 * @param llist list to get node from
 * @param idx index of node
 * @return LLNode* - node at the index, or NULL if the index is out of range
 */
LLNode *llgetitem(LinkedList *llist, int idx);

/**
 * @brief Display a linked list to stdout.
 */
void llprint(LinkedList *llist);

/**
 * @brief Append a new node to the end of a linked list.
 */
void llappend(LinkedList *llist, void *data);

/**
 * @brief Remove the last element of a linked list.
 *
 * @param llist list to shave off
 * @param databuf double-ref that is set to the value of the vdata field of the removed node, or NULL
 */
void llshave(LinkedList *llist, void **databuf);

/**
 * @brief Insert a new node at an index of a linked list.
 *
 * @param llist list to insert into
 * @param idx index of the node to insert
 * @param vdata value of vdata field of the new node
 */
void llinsert(LinkedList *llist, int idx, void *data);

/**
 * @brief Remove a node from an index of a linked list.
 *
 * @param llist list to remove from
 * @param idx index of the node to remove
 * @param databuf double-ref that is set to the value of the vdata field of the removed node, or NULL
 */
void llremove(LinkedList *llist, int idx, void **databuf);

/**
 * @brief Copy a linked list.
 *
 * @param llist list to copy
 * @return LinkedList* - new copied list
 */
LinkedList *llcopy(LinkedList *llist);

/**
 * @brief Count how many occurrences of a vvalue is present in vdata fields of a linked list.
 *
 * @param llist list to search from
 * @param value value to search for
 * @param indices list to store occurrence indices in, or NULL
 * @return int - number of occurrences
 */
int llcount(LinkedList *llist, void *value, int *indices);

/**
 * @brief Return the index of the first occurence of a value in the vdata fields of a linked list.
 *
 * @param llist list to search
 * @param value value to search for
 * @return int - index of first match, or -1 for no match
 */
int llindex(LinkedList *llist, void *vvalue);

/**
 * @brief Reverse a linked list.
 */
void llreverse(LinkedList *llist);

/**
 * @brief Remove a random element from a linked list.
 *
 * @param llist list to pop from
 * @param databuf double-ref that is set to the value of the vdata field of the removed node, or NULL
 */
void llpop(LinkedList *llist, void **databuf);

/**
 * @brief Clear a linked list, setting all idata fields to 0 and vdata fields to NULL
 */
void llclear(LinkedList *llist);

/**
 * @brief Create a flat list out of `vdata` fields of a linked list.
 *
 * @param llist linked list
 * @param arr void double-ref array buffer
 * @param start start of flat list, or -1 for beginning (inclusive)
 * @param stop stop of flat list, or -1 for end (exclusive)
 * @return int* - same void double-ref array pointer passed
 */
void **lltolist(LinkedList *llist, void **varr, int start, int stop);
