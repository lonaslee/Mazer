#pragma once

#include <stdbool.h>

typedef struct Node {
    struct Node *u, *l, *d, *r;
    bool *uw, *lw, *dw, *rw;
    int x, y;
    long data;
} Node;

typedef struct Graph {
    Node ***nodes;
    int nr;
    int nc;
    long data;
} Graph;

Graph *new_graph(int nr, int nc);
void del_graph(Graph *g);

typedef enum Direction {
    STAY,
    UP,
    LEFT,
    DOWN,
    RIGHT
} Direction;

#define MOVEX(dir) ((dir) == RIGHT ? 1 : (dir) == LEFT ? -1 \
                                                       : 0)
#define MOVEY(dir) ((dir) == UP ? 1 : (dir) == DOWN ? -1 \
                                                    : 0)

#define get(g, x, y) g->nodes[(y)][(x)]

Graph *connect_all(Graph *g);
Graph *disconnect_all(Graph *g);
void connect(Node *a, Node *b);
void surround(Node *n);
