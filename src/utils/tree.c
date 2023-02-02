#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

TNode *trnew(void *data) {
    TNode *tip = malloc(sizeof(TNode));
    tip->data = data;
    tip->nextsib = NULL;
    tip->child = NULL;
    tip->parent = tip;
    return tip;
}

void trdel(TNode *node) {
    TNode *cur, *next;
    for (cur = node, next = cur->nextsib; cur != NULL;) {
        if (cur->child) trdel(cur->child);
        free(cur);
        cur = next;
        if (next) next = next->nextsib;
    }
}

TNode *trappendsib(TNode *node, void *data) {
    TNode *next = trnew(data);
    if (node->parent != node)
        next->parent = node->parent;
    TNode *cur;
    for (cur = node; cur->nextsib != NULL; cur = cur->nextsib)
        ;
    cur->nextsib = next;
    return next;
}

void *trshavesib(TNode *node) {
    TNode *slast;
    for (slast = node; slast->nextsib->nextsib != NULL; slast = slast->nextsib)
        ;
    void *databuf = slast->nextsib->data;
    trdel(slast->nextsib);
    slast->nextsib = NULL;
    return databuf;
}

TNode *trappendchild(TNode *node, void *data) {
    if (!node->child) {
        node->child = trnew(data);
        node->child->parent = node;
        return node->child;
    }
    return trappendsib(node->child, data);
}

void *trshavechild(TNode *node) {
    TNode *child = node->child;
    if (!child) return NULL;

    TNode *slast;
    for (slast = child; slast->nextsib->nextsib != NULL; slast = slast->nextsib)
        ;
    void *databuf = slast->nextsib->data;
    trdel(slast->nextsib);
    slast->nextsib = NULL;
    if (slast == node->child) node->child = NULL;
    return databuf;
}

TNode *trgetroot(TNode *node) {
    TNode *root = node;
    while (root->parent != root) root = root->parent;
    return root;
}

void trappendsibtree(TNode *tree, TNode *subtree) {
    if (tree->parent != tree)
        subtree->parent = tree->parent;
    TNode *cur;
    for (cur = tree; cur->nextsib != NULL; cur = cur->nextsib)
        ;
    cur->nextsib = subtree;
}

void trappendchildtree(TNode *tree, TNode *subtree) {
    if (!tree->child) {
        subtree->parent = tree;
        tree->child = subtree;
    } else {
        trappendsibtree(tree->child, subtree);
    }
}

static void indent(int level) {
    for (int i = 0; i < level; i++) printf("\t");
}

static void priv_trprint(TNode *tip, int level) {
    TNode *cur;
    for (cur = tip->child; cur != NULL; cur = cur->nextsib) {
        indent(level);
        printf("   %c\n", 179);
        indent(level);
        printf("   %c%c%c ", 195, 196, 196);
        printf("%5p\n", cur->data);
        if (cur->child)
            priv_trprint(cur, level + 1);
    }
}

void trprint(TNode *tip) {
    printf("\nTree at %p\n\n", tip);
    printf("\t%5p\n", tip->data);
    if (tip->child)
        priv_trprint(tip, 1);
    puts("");
}
