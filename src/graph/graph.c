#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

Graph *new_graph(int nr, int nc) {
    Graph *g = calloc(1, sizeof(Graph));
    g->nr = nr;
    g->nc = nc;
    g->nodes = calloc(nr, sizeof(Node ***));
    for (int y = 0; y < nr; y++) {
        g->nodes[y] = calloc(nc, sizeof(Node **));
        for (int x = 0; x < nc; x++) {
            Node *n = calloc(1, sizeof(Node));
            g->nodes[y][x] = n;
            n->x = x;
            n->y = y;
        }
    }

    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = g->nodes[y][x];
            if (y != 0) {
                n->u = g->nodes[y - 1][x];
                n->u->d = n;
                n->uw = n->u->dw = calloc(1, sizeof(bool));
            }
            if (x != 0) {
                n->l = g->nodes[y][x - 1];
                n->l->r = n;
                n->lw = n->l->rw = calloc(1, sizeof(bool));
            }
        }
    }
    return g;
}

void del_graph(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = g->nodes[y][x];
            if (y != 0) free(n->uw);
            if (x != 0) free(n->lw);
        }
        for (int x = 0; x < g->nc; x++) {
            free(g->nodes[y][x]);
        }
        free(g->nodes[y]);
    }
    free(g->nodes);
    free(g);
}

Graph *connect_all(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            if (y != 0) *(n->uw) = true;
            if (x != 0) *(n->lw) = true;
        }
    }
    return g;
}

Graph *disconnect_all(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            if (y != 0) *(n->uw) = false;
            if (x != 0) *(n->lw) = false;
        }
    }
    return g;
}

void connect(Node *a, Node *b) {
    if (a->u == b) {
        *(a->uw) = false;
    } else if (a->l == b) {
        *(a->lw) = false;
    } else if (a->d == b) {
        *(a->dw) = false;
    } else if (a->r == b) {
        *(a->rw) = false;
    } else {
        printf("Not able to connect nodes (%d, %d) & (%d, %d).", a->x, a->y, b->x, b->y);
        abort();
    }
}

void surround(Node *n) {
    *(n->uw) = *(n->lw) = *(n->dw) = *(n->rw) = true;
}
