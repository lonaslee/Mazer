#include "maze_algs.h"

#include <stdlib.h>

#include "common.h"
#include "graph.h"

typedef struct AlduousBroderState {
    int cx, cy;
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
    if (s->visited == g->nr * g->nc - 1) return NULL;

    printf("\r(%d, %d) - %d", s->cx, s->cy, s->visited);

    Direction dir = choicenz(4,
                             UP * (s->cy != g->nr - 1),
                             LEFT * (s->cx != 0),
                             DOWN * (s->cy != 0),
                             RIGHT * (s->cx != g->nc - 1));
    Node *p = get(g, s->cx, s->cy);
    s->cx += MOVEX(dir);
    s->cy += MOVEY(dir);
    Node *n = get(g, s->cx, s->cy);
    if (n->data == 1) return s;
    n->data = 1;
    s->visited++;
    connect(p, n);
    return s;
}
