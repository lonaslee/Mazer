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
                n->nny = g->nodes[y - 1][x];
                n->nny->npy = n;
                n->wny = n->nny->wpy = calloc(1, sizeof(bool));
            }
            if (x != 0) {
                n->nnx = g->nodes[y][x - 1];
                n->nnx->npx = n;
                n->wnx = n->nnx->wpx = calloc(1, sizeof(bool));
            }
        }
    }
    return g;
}

void del_graph(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = g->nodes[y][x];
            if (y != 0) free(n->wny);
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

void dir2str(AxisDirection dir, char *buf) {
    memcpy(buf, ((dir) == NEG_Y) ? "NEG_Y" : ((dir) == POS_Y) ? "POS_Y"
                                         : ((dir) == NEG_X)   ? "NEG_X"
                                         : ((dir) == POS_X)   ? "POS_X"
                                                              : "NONE ",
           6);
}

Graph *connect_all(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            if (y != 0) *(n->wny) = true;
            if (x != 0) *(n->wnx) = true;
        }
    }
    return g;
}

Graph *disconnect_all(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            if (y != 0) *(n->wny) = false;
            if (x != 0) *(n->wnx) = false;
        }
    }
    return g;
}

void connect(Node *a, Node *b) {
    if (a->nny == b) {
        *(a->wny) = false;
    } else if (a->nnx == b) {
        *(a->wnx) = false;
    } else if (a->npy == b) {
        *(a->wpy) = false;
    } else if (a->npx == b) {
        *(a->wpx) = false;
    } else {
        printf("Not able to connect nodes (%d, %d) & (%d, %d).", a->x, a->y, b->x, b->y);
        abort();
    }
}

void surround(Node *n) {
    ifnn(n->wny) * (n->wny) = true;
    ifnn(n->wnx) * (n->wnx) = true;
    ifnn(n->wpy) * (n->wpy) = true;
    ifnn(n->wpx) * (n->wpx) = true;
}

void extrude_to(Node *n, AxisDirection dir) {
    ifnn(n->wny) { *(n->wny) = dir != POS_Y; }
    ifnn(n->wnx) { *(n->wnx) = dir != POS_X; }
    ifnn(n->wpy) { *(n->wpy) = dir != NEG_Y; }
    ifnn(n->wpx) { *(n->wpx) = dir != NEG_X; }
}
