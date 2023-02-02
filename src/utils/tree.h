/**
 * @file tree.c
 * @brief Tree data structure, providing access and modifying functions ("tr" prefixed).
 */
#pragma once

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
