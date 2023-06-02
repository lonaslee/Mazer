#pragma once

#include <stdbool.h>

typedef struct Node {
    struct Node *nny, *nnx, *npy, *npx;
    bool *wny, *wnx, *wpy, *wpx;
    int x, y;
    long long data;
} Node;

typedef struct Graph {
    Node ***nodes;
    int nr, nc;
    long long data;
} Graph;

Graph *new_graph(int nr, int nc);
void del_graph(Graph *g);

typedef enum AxisDirection {
    NONE,
    POS_Y,
    NEG_X,
    NEG_Y,
    POS_X
} AxisDirection;

void dir2str(AxisDirection dir, char *buf);

#define MOVEX(dir) ((dir) == POS_X ? 1 : (dir) == NEG_X ? -1 \
                                                        : 0)
#define MOVEY(dir) ((dir) == POS_Y ? 1 : (dir) == NEG_Y ? -1 \
                                                        : 0)

#define get(g, x, y) g->nodes[(y)][(x)]

Graph *connect_all(Graph *g);
Graph *disconnect_all(Graph *g);
void connect(Node *a, Node *b);
void surround(Node *n);
void extrude_to(Node *n, AxisDirection dir);
