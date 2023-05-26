#include "maze_algs.h"

#include <stdlib.h>

#include "common.h"
#include "graph.h"
#include "screen.h"

typedef struct {
    Coord c;
    int visited;
} AlduousBroderState;

void *alduous_broder(Graph *g, void *state) {
    AlduousBroderState *s = state;
    ifn(s) {
        s = calloc(1, sizeof(AlduousBroderState));
        s->c.x = rand() % g->nc;
        s->c.y = rand() % g->nr;
        s->visited = 0;
        surround(get(g, s->c.x, s->c.y));
    }

    Node *p = get(g, s->c.x, s->c.y);
    lunflip(p->data, CELL1);
    if (s->visited == g->nr * g->nc) return NULL;

    AxisDirection dir = choicenz(4,
                                 POS_Y * (s->c.y != g->nr - 1),
                                 NEG_X * (s->c.x != 0),
                                 NEG_Y * (s->c.y != 0),
                                 POS_X * (s->c.x != g->nc - 1));
    s->c.x += MOVEX(dir);
    s->c.y += MOVEY(dir);
    Node *n = get(g, s->c.x, s->c.y);
    lflip(n->data, CELL1);
    if (lisflipped(n->data, 0)) return s;
    lflip(n->data, CELL2);
    lflip(n->data, 0);
    s->visited++;

    ifnn(n->wny) { *(n->wny) = dir != POS_Y; }
    ifnn(n->wnx) { *(n->wnx) = dir != POS_X; }
    ifnn(n->wpy) { *(n->wpy) = dir != NEG_Y; }
    ifnn(n->wpx) { *(n->wpx) = dir != NEG_X; }
    return s;
}

typedef struct {
    Coord c;
} BinaryTreeState;

void *binary_tree(Graph *g, void *state) {
    BinaryTreeState *s = state;
    ifn(s) {
        connect_all(g);
        s = calloc(1, sizeof(BinaryTreeState));
        s->c.x = 0;
        s->c.y = 0;
    }

    Node *n = get(g, s->c.x, s->c.y);
    lunflip(n->data, CELL1);
    lflip(n->data, CELL2);
    if (rand() % 2) {
        ifnn(n->wny) * (n->wny) = false;
        else ifnn(n->wnx) * (n->wnx) = false;

    } else {
        ifnn(n->wnx) * (n->wnx) = false;
        else ifnn(n->wny) * (n->wny) = false;
    }

    if (s->c.x < g->nc - 1) {
        s->c.x++;
    } else {
        s->c.x = 0;
        s->c.y++;
        if (s->c.y == g->nr) return NULL;
    }
    lflip(get(g, s->c.x, s->c.y)->data, CELL1);
    return s;
}
