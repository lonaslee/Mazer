#ifndef MAZE_H
#define MAZE_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "grid.h"

#define RANDDIR (rand() % 4)

#define MOVEX(dir) ((dir == UP || dir == DOWN) ? 0 : (dir == RIGHT) ? 1 \
                                                                    : -1)
#define MOVEY(dir) ((dir == LEFT || dir == RIGHT) ? 0 : (dir == UP) ? 1 \
                                                                    : -1)

enum DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum MazeType {
    ALDOUS_BRODER,
    WILSONS,
    BINARY_TREE,
    SIDEWINDER,
    ELLERS,
    HUNT_AND_KILL,
    RECURSIVE_BACKTRACKER,
    RECURSIVE_DIVISION,
    KRUSKALS,
    PRIMS,
    GROWING_TREE,
    GROWING_BINARY_TREE
};

Grid *generate_aldous_broder(Grid *grid);
int choice(int n, ...);

#endif /* MAZE_H */