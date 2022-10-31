#include "collections.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * * * *\
 * Stack *
\* * * * */

Stack *sknew(int lenth) {
    Stack *stack = calloc(1, sizeof(Stack));
    stack->_size = lenth;
    stack->len = 0;
    stack->elements = calloc(lenth, sizeof(void *));
    return stack;
}

void skdel(Stack *stack) {
    free(stack->elements);
    free(stack);
}

void skpush(Stack *stack, void *value) {
    if (stack->len == stack->_size) {
        stack->elements = realloc(stack->elements, (stack->_size *= 2) * sizeof(void *));
        memset(&stack->elements[stack->len], 0, (stack->_size / 2) * sizeof(void *));
    }
    stack->elements[stack->len++] = value;
}

void skpop(Stack *stack, void **databuf) {
    if (!stack->len) {
        if (databuf) *databuf = NULL;
        return;
    }
    stack->len--;
    if (databuf) *databuf = stack->elements[stack->len];
    stack->elements[stack->len] = NULL;
}

void skprint(Stack *stack) {
    printf("\nStack at %p (size: %d, length: %d)\n\n", stack, stack->_size, stack->len);
    for (int i = stack->len - 1; i >= 0; i--)
        printf("%3d  %p\n", i, stack->elements[i]);
    puts("");
}

/* * * * *\
 * Tree  *
\* * * * */

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

/* * * * * * * *\
 * Linked List *
\* * * * * * * */

LinkedList *llnew(void *data) {
    LinkedList *ll = calloc(1, sizeof(LinkedList));
    ll->len = 1;
    ll->first = calloc(1, sizeof(LLNode));
    ll->first->data = data;
    ll->last = ll->first;
    return ll;
}

void lldel(LinkedList *llist) {
    for (LLNode *cur = llist->first; cur != NULL;) {
        LLNode *next = cur->next;
        free(cur);
        cur = next;
    }
    free(llist);
}

LLNode *llgetitem(LinkedList *llist, int idx) {
    int i = 0;
    for (LLNode *cur = llist->first; cur != NULL; cur = cur->next, i++)
        if (i == idx) return cur;
    return NULL;
}

void llprint(LinkedList *llist) {
    printf("\nLinkedList at %p (len: %d)\n\n", llist, llist->len);
    int i;
    LLNode *cur;
    for (i = 0, cur = llist->first; cur != NULL; cur = cur->next, i++)
        printf("%3d  %p\n", i, cur->data);
    puts("");
}

void llappend(LinkedList *llist, void *data) {
    llist->len++;
    llist->last->next = calloc(1, sizeof(LLNode));
    llist->last = llist->last->next;
    llist->last->data = data;
}

void llshave(LinkedList *llist, void **databuf) {
    llist->len--;
    if (!llist->len) {
        if (databuf) *databuf = llist->first->data;
        free(llist->first);
        llist->first = llist->last = NULL;
        return;
    }
    LLNode *slast;
    for (slast = llist->first; slast->next->next != NULL; slast = slast->next)
        ;
    if (databuf) *databuf = slast->next->data;
    free(slast->next);
    slast->next = NULL;
    llist->last = slast;
}

void llinsert(LinkedList *llist, int idx, void *data) {
    llist->len++;
    LLNode *new = calloc(1, sizeof(LLNode));
    new->data = data;
    if (idx == 0) {
        LLNode *prevfirst = llist->first;
        llist->first = new;
        new->next = prevfirst;
    } else {
        LLNode *prev = llgetitem(llist, idx - 1);
        LLNode *prevhere = prev->next;
        prev->next = new;
        new->next = prevhere;
    }
}

void llremove(LinkedList *llist, int idx, void **databuf) {
    llist->len--;
    if (idx == 0) {
        if (databuf) *databuf = llist->first->data;
        LLNode *newfirst = llist->first->next;
        free(llist->first);
        llist->first = newfirst;
    } else if (idx == llist->len - 1) {
        llshave(llist, databuf);
    } else {
        LLNode *prev = llgetitem(llist, idx - 1);
        if (databuf) *databuf = prev->next->data;
        LLNode *newhere = prev->next->next;
        free(prev->next);
        prev->next = newhere;
    }
}

LinkedList *llcopy(LinkedList *llist) {
    LinkedList *cpy = llnew(NULL);
    LLNode *old;
    LLNode *new;
    for (old = llist->first, new = cpy->first; old != NULL; old = old->next) {
        llappend(cpy, NULL);
        memcpy(new->data, &old->data, sizeof(void *));
    }
    llremove(cpy, 0, NULL);
    return cpy;
}

int llcount(LinkedList *llist, void *value, int *indices) {
    int count = 0;
    int i = 0;
    for (LLNode *cur = llist->first; cur != NULL; cur = cur->next, i++)
        if (cur->data == value) {
            if (indices) indices[count] = i;
            count++;
        }
    return count;
}

int llindex(LinkedList *llist, void *value) {
    int i = 0;
    for (LLNode *cur = llist->first; cur != NULL; cur = cur->next, i++) {
        if (cur->data == value) return i;
    }
    return -1;
}

void llreverse(LinkedList *llist) {
    LLNode *prev, *cur, *next;
    for (prev = NULL, cur = llist->first, next = NULL; cur != NULL; prev = cur, cur = next) {
        next = cur->next;
        cur->next = prev;
    }
    llist->first = prev;
    llist->last = llgetitem(llist, llist->len - 1);
}

void llpop(LinkedList *llist, void **databuf) {
    llremove(llist, rand() % llist->len, databuf);
}

void llclear(LinkedList *llist) {
    for (LLNode *cur = llist->first; cur != NULL; cur = cur->next)
        cur->data = NULL;
}

void **lltolist(LinkedList *llist, void **arr, int start, int stop) {
    if (start == -1) start = 0;
    if (stop == -1) stop = llist->len;
    LLNode *cur;
    int i;
    for (i = 0, cur = llgetitem(llist, start); cur != NULL && i < stop; cur = cur->next, i++)
        arr[i] = cur->data;
    return arr;
}
