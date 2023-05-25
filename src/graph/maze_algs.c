#include "maze_algs.h"

#include <stdlib.h>

#include "common.h"
#include "graph.h"

typedef struct AlduousBroderState {
    int cx;
    int cy;
    int visited;
} AlduousBroderState;

void *alduous_broder(Graph *g, void *state) {
    AlduousBroderState *s = state;
    if (s == NULL) {
        s = calloc(1, sizeof(AlduousBroderState));
        s->cx = rand() % g->nc;
        s->cy = rand() % g->nr;
        s->visited = 0;
        surround(get(g, s->cx, s->cy));
    }

    Node *p = get(g, s->cx, s->cy);
    lunflip(p->data, 31);
    if (s->visited == g->nr * g->nc) return NULL;

    AxisDirection dir = choicenz(4,
                                 POS_Y * (s->cy != g->nr - 1),
                                 NEG_X * (s->cx != 0),
                                 NEG_Y * (s->cy != 0),
                                 POS_X * (s->cx != g->nc - 1));
    s->cx += MOVEX(dir);
    s->cy += MOVEY(dir);
    Node *n = get(g, s->cx, s->cy);
    lflip(n->data, 31);
    if (lisflipped(n->data, 0)) return s;
    lflip(n->data, 30);
    lflip(n->data, 0);
    s->visited++;

    ifnn(n->wny) { *(n->wny) = dir != POS_Y; }
    ifnn(n->wnx) { *(n->wnx) = dir != POS_X; }
    ifnn(n->wpy) { *(n->wpy) = dir != NEG_Y; }
    ifnn(n->wpx) { *(n->wpx) = dir != NEG_X; }
    return s;
}
