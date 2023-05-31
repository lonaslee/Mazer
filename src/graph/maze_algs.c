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

#define AB_VISITED 0

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
    llunflip(p->data, PRIMARY_CELL);
    if (s->visited == g->nr * g->nc) return NULL;

    AxisDirection dir = choicenz(4,
                                 POS_Y * (s->c.y != g->nr - 1),
                                 NEG_X * (s->c.x != 0),
                                 NEG_Y * (s->c.y != 0),
                                 POS_X * (s->c.x != g->nc - 1));
    s->c.x += MOVEX(dir);
    s->c.y += MOVEY(dir);
    Node *n = get(g, s->c.x, s->c.y);
    llflip(n->data, PRIMARY_CELL);
    if (llisflipped(n->data, AB_VISITED)) return s;
    llflip(n->data, SECONDARY_CELL);
    llflip(n->data, AB_VISITED);
    s->visited++;

    ifnn(n->wny) { *(n->wny) = dir != POS_Y; }
    ifnn(n->wnx) { *(n->wnx) = dir != POS_X; }
    ifnn(n->wpy) { *(n->wpy) = dir != NEG_Y; }
    ifnn(n->wpx) { *(n->wpx) = dir != NEG_X; }
    return s;
}

#undef AB_VISITED

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
    llunflip(n->data, PRIMARY_CELL);
    llflip(n->data, SECONDARY_CELL);
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
    llflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
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

    AxisDirection dir = choicenz(4, NEG_Y * (s->c->nny && !llisflipped(s->c->nny->data, 0)),
                                 POS_Y * (s->c->npy && !llisflipped(s->c->npy->data, 0)),
                                 NEG_X * (s->c->nnx && !llisflipped(s->c->nnx->data, 0)),
                                 POS_X * (s->c->npx && !llisflipped(s->c->npx->data, 0)));

    if (dir == NONE) {
        Node *t;
        skpop(s->stack, (void **)&t);
        llunflip(t->data, SECONDARY_CELL);
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

    llflip(s->c->data, 0);
    skpush(s->stack, s->c);
    llflip(s->c->data, SECONDARY_CELL);
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

    int MAX_GROUP_SIZE = MAX(AVG(g->nc, g->nr), 3);
    int RAND_WEIGHT = MAX(HALF(MAX_GROUP_SIZE), 3);

    for (int x = 0; x < g->nc - 1; x++) {
        *(get(g, x, 0)->wpx) = false;
    }

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
            llunflip(((Node *)arget(s->a, j))->data, SECONDARY_CELL);
        arclear(s->a);

        s->sz = 0;
    } else {
        Node *t = get(g, s->c.x, s->c.y);
        *(t->wpx) = false;
        llflip(t->data, SECONDARY_CELL);
        arappend(s->a, t);
        s->sz++;
    }

    llunflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
    if (++s->c.x == g->nc) {
        s->c.x = 0;
        if (++s->c.y == g->nr) {
            ardel(s->a);
            return NULL;
        }
    }
    llflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
    return s;
}

typedef struct {
    Coord c;
    short l;
    int sn;
    int ss;
    Array *set;
} EllersState;

void *ellers(Graph *g, void *state) {
    EllersState *s = state;
    ifn(s) {
        s = calloc(1, sizeof(EllersState));
        s->c.x = 0;
        s->c.y = 0;
        s->l = 0;
        s->sn = 1;
        connect_all(g);
        s->set = arnew(1);
    }

    int RRAND_WEIGHT = 3;
    int DRAND_WEIGHT = 5;
    int MAX_SET_SIZE = INT_MAX;

    switch (s->l) {
        case 0: {
            for (int x = 0; x < g->nc; x++) {
                Node *n = get(g, x, s->c.y);
                if (llgetint(n->data) == 0)
                    llsetint(n->data, s->sn++);
            }
            s->ss = 1;
            s->l++;
            llflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
        } break;
        case 1: {
            Node *t = get(g, s->c.x, s->c.y);
            llunflip(t->data, PRIMARY_CELL);
            Node *n = get(g, s->c.x + 1, s->c.y);
            int td = llgetint(t->data);
            int nd = llgetint(n->data);
            if (td == nd || rand() % RRAND_WEIGHT == 0 || s->ss >= MAX_SET_SIZE) {
                s->ss = 1;
            } else {
                *(t->wpx) = false;
                for (int x = 0; x < g->nc; x++) {
                    Node *c = get(g, x, s->c.y);
                    if (llgetint(c->data) == nd) {
                        llsetint(c->data, td);
                        s->ss++;
                    }
                }
            }

            if (++s->c.x == g->nc - 1) {
                s->c.x = 0;
                s->l++;
                s->ss = 0;
            }
            llflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
        } break;
        case 2: {
            for (int i = 0; i < s->set->len; i++)
                llunflip(((Node *)arget(s->set, i))->data, SECONDARY_CELL);
            arclear(s->set);
            if (s->c.x >= g->nc) {
                s->c.x = 0;
                s->l = 0;
                if (++s->c.y == g->nr) {
                    ardel(s->set);
                    s->c.y--;

                    for (int x = 0; x < g->nc - 1; x++) {
                        Node *n = get(g, x, s->c.y);
                        if (llgetint(n->data) != llgetint(n->npx->data)) {
                            *(n->wpx) = false;
                            int mn = llgetint(n->npx->data);
                            for (int xx = 0; xx < g->nc; xx++) {
                                Node *t = get(g, xx, s->c.y);
                                if (llgetint(t->data) == mn) {
                                    llsetint(t->data, n->data);
                                }
                            }
                        }
                    }

                    return NULL;
                }
                return s;
            } else {
                llunflip(get(g, s->c.x, s->c.y)->data, PRIMARY_CELL);
            }

            int curset = llgetint(get(g, s->c.x, s->c.y)->data);
            do {
                Node *n = get(g, s->c.x, s->c.y);
                if (llgetint(n->data) != curset) {
                    s->c.x--;
                    break;
                }
                arappend(s->set, n);
                llflip(n->data, SECONDARY_CELL);
                s->c.x++;
            } while (s->c.x < g->nc);
            int idx = choicerange(0, s->set->len);
            for (int i = 0; i < s->set->len; i++) {
                if (rand() % DRAND_WEIGHT == 0 || i == idx) {
                    Node *n = arget(s->set, i);
                    if (s->c.y != g->nr - 1) {
                        *(n->wpy) = false;
                        llsetint(n->npy->data, curset);
                    }
                }
            }
            s->c.x++;
        } break;
        default:
            WTF;
    }

    return s;
}