#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

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
                n->wpy = n->u->wny = calloc(1, sizeof(bool));
            }
            if (x != 0) {
                n->l = g->nodes[y][x - 1];
                n->l->r = n;
                n->wnx = n->l->wpx = calloc(1, sizeof(bool));
            }
        }
    }
    return g;
}

void del_graph(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = g->nodes[y][x];
            if (y != 0) free(n->wpy);
            if (x != 0) free(n->wnx);
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
            if (y != 0) *(n->wpy) = true;
            if (x != 0) *(n->wnx) = true;
        }
    }
    return g;
}

Graph *disconnect_all(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            if (y != 0) *(n->wpy) = false;
            if (x != 0) *(n->wnx) = false;
        }
    }
    return g;
}

void connect(Node *a, Node *b) {
    if (a->u == b) {
        *(a->wpy) = false;
    } else if (a->l == b) {
        *(a->wnx) = false;
    } else if (a->d == b) {
        *(a->wny) = false;
    } else if (a->r == b) {
        *(a->wpx) = false;
    } else {
        printf("Not able to connect nodes (%d, %d) & (%d, %d).", a->x, a->y, b->x, b->y);
        abort();
    }
}

void surround(Node *n) {
    ifnn(n->wpy) * (n->wpy) = true;
    ifnn(n->wnx) * (n->wnx) = true;
    ifnn(n->wny) * (n->wny) = true;
    ifnn(n->wpx) * (n->wpx) = true;
}
