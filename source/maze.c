#include "maze.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"
#include "node.h"

Grid *generate_aldous_broder(Grid *grid) {
    grid->type = ALDOUS_BRODER;
    int curx = rand() % (grid->width - 1);
    int cury = rand() % (grid->height - 1);
    int visited_num = 0;
    Cell cell = grid->cells[curx][cury];
    cell.upperwall->exists = 1;
    cell.lowerwall->exists = 1;
    cell.left_wall->exists = 1;
    cell.rightwall->exists = 1;
    cell.data = 1;

    while (visited_num != grid->width * grid->height) {
        enum DIRECTION dir = RANDDIR;
        if (curx == 0 && dir == LEFT) dir = choice(3, UP, DOWN, RIGHT);
        if (cury == 0 && dir == DOWN) dir = choice(3, UP, LEFT, RIGHT);
        if (curx == (grid->width - 1) && dir == RIGHT) dir = choice(3, UP, DOWN, LEFT);
        if (cury == (grid->height - 1) && dir == UP) dir = choice(3, DOWN, LEFT, RIGHT);
        curx += MOVEX(dir);
        cury += MOVEY(dir);
        cell = grid->cells[curx][cury];
        if (cell.data == 1) continue;
        cell.data = 1;
        visited_num++;
        cell.upperwall->exists = 1;
        cell.lowerwall->exists = 1;
        cell.left_wall->exists = 1;
        cell.rightwall->exists = 1;
        switch (dir) {
            case UP:
                cell.lowerwall->exists = 0;
                break;
            case DOWN:
                cell.upperwall->exists = 0;
                break;
            case LEFT:
                cell.rightwall->exists = 0;
                break;
            case RIGHT:
                cell.left_wall->exists = 0;
                break;
        }
    }
    return grid;
}

int choice(int n, ...) {
    va_list vargs;
    va_start(vargs, n);
    int randnum = rand() % n;
    for (int i = 0; i < n; i++) {
        int varg = va_arg(vargs, int);
        if (i == randnum) return varg;
    }
}
