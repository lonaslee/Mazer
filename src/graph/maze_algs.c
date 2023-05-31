#include "maze_algs.h"

#include <stdlib.h>

#include "common.h"
#include "graph.h"
#include "screen.h"
#include "utils/array.h"
#include "utils/stack.h"

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
    lunflip(p->data, PRIMARY_CELL);
    if (s->visited == g->nr * g->nc) return NULL;

    AxisDirection dir = choicenz(4,
                                 POS_Y * (s->c.y != g->nr - 1),
                                 NEG_X * (s->c.x != 0),
                                 NEG_Y * (s->c.y != 0),
                                 POS_X * (s->c.x != g->nc - 1));
    s->c.x += MOVEX(dir);
    s->c.y += MOVEY(dir);
    Node *n = get(g, s->c.x, s->c.y);
    lflip(n->data, PRIMARY_CELL);
    if (lisflipped(n->data, 0)) return s;
    lflip(n->data, SECONDARY_CELL);
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
    lunflip(n->data, PRIMARY_CELL);
    lflip(n->data, SECONDARY_CELL);
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
    lflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
    return s;
}

typedef struct {
    Node *c;
    Stack *stack;
} RecursiveBacktrackerState;

void *recursive_backtracker(Graph *g, void *state) {
    RecursiveBacktrackerState *s = state;
    ifn(s) {
        s = calloc(1, sizeof(RecursiveBacktrackerState));
        s->stack = sknew(g->nc);
        s->c = get(g, rand() % g->nc, rand() % g->nr);
        surround(s->c);
    }

    AxisDirection dir = choicenz(4, NEG_Y * (s->c->nny && !lisflipped(s->c->nny->data, 0)),
                                 POS_Y * (s->c->npy && !lisflipped(s->c->npy->data, 0)),
                                 NEG_X * (s->c->nnx && !lisflipped(s->c->nnx->data, 0)),
                                 POS_X * (s->c->npx && !lisflipped(s->c->npx->data, 0)));

    if (dir == NONE) {
        Node *t;
        skpop(s->stack, (void **)&t);
        lunflip(t->data, SECONDARY_CELL);
        if (s->stack->len) {
            s->c = s->stack->elements[s->stack->len - 1];
            return s;
        } else {
            return NULL;
        }
    }

    Node *n;
    switch (dir) {
        case NEG_Y:
            n = s->c->nny;
            break;
        case POS_Y:
            n = s->c->npy;
            break;
        case NEG_X:
            n = s->c->nnx;
            break;
        case POS_X:
            n = s->c->npx;
            break;
    }

    ifnn(n->wny) { *(n->wny) = dir != POS_Y; }
    ifnn(n->wnx) { *(n->wnx) = dir != POS_X; }
    ifnn(n->wpy) { *(n->wpy) = dir != NEG_Y; }
    ifnn(n->wpx) { *(n->wpx) = dir != NEG_X; }

    s->c = n;

    lflip(s->c->data, 0);
    skpush(s->stack, s->c);
    lflip(s->c->data, SECONDARY_CELL);
    return s;
}

typedef struct {
    Coord c;
    Array *a;
    int sz;
} SidewinderState;

void *sidewinder(Graph *g, void *state) {
    SidewinderState *s = state;
    ifn(s) {
        s = calloc(1, sizeof(SidewinderState));
        s->a = arnew(5);
        s->c.x = 0;
        s->c.y = 1;
        s->sz = 0;
        connect_all(g);
    }

    int MAX_GROUP_SIZE = AVG(g->nc, g->nr);
    int RAND_WEIGHT = HALF(MAX_GROUP_SIZE);

    for (int x = 0; x < g->nc - 1; x++) {
        *(get(g, x, 0)->wpx) = false;
    }

    printf("sz=%d\n", s->sz);
    if (rand() % RAND_WEIGHT == 0 || s->sz == MAX_GROUP_SIZE || s->c.x == g->nc - 1) {
        if (s->sz == 0) {
            Node *n = get(g, s->c.x, s->c.y);
            ifnn(n->wny) * (n->wny) = false;
        } else {
            int i = rand() % s->sz;
            Node *n = arget(s->a, i);
            ifnn(n->wny) * (n->wny) = false;
        }
        for (int j = 0; j < s->sz; j++)
            lunflip(((Node *)arget(s->a, j))->data, SECONDARY_CELL);
        arclear(s->a);

        s->sz = 0;
    } else {
        Node *t = get(g, s->c.x, s->c.y);
        *(t->wpx) = false;
        lflip(t->data, SECONDARY_CELL);
        arappend(s->a, t);
        s->sz++;
    }
    puts("");

    lunflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
    if (++s->c.x == g->nc) {
        s->c.x = 0;
        if (++s->c.y == g->nr) return NULL;
    }
    lflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
    return s;
}
