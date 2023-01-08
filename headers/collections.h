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
