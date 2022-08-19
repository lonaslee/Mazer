#include "maze.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "grid.h"
#include "images.h"

Grid *generate_aldous_broder(Grid *grid) {
    puts("aldous broder");
    grid->type = ALDOUS_BRODER;
    int curx = rand() % grid->width;
    int cury = rand() % grid->height;
    int visited = 0;
    Cell *this_cell = &grid->cells[curx][cury];
    this_cell->upperwall->exists = 1;
    this_cell->lowerwall->exists = 1;
    this_cell->left_wall->exists = 1;
    this_cell->rightwall->exists = 1;
    this_cell->data = 1;

    while (visited < grid->width * grid->height - 1) {
        enum DIRECTION dir = choicenz(4, UP * (cury != (grid->height - 1)),
                                      DOWN * (cury != 0),
                                      LEFT * (curx != 0),
                                      RIGHT * (curx != (grid->width - 1)));
        curx += MOVEX(dir);
        cury += MOVEY(dir);
        this_cell = &grid->cells[curx][cury];
        if (this_cell->data == 1) {
            continue;
        }
        this_cell->data = 1;
        visited++;
        this_cell->upperwall->exists = dir != DOWN;
        this_cell->lowerwall->exists = dir != UP;
        this_cell->left_wall->exists = dir != RIGHT;
        this_cell->rightwall->exists = dir != LEFT;
    }
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

int choicenz(int n, ...) {
    va_list vargs;
    va_start(vargs, n);
    int nums[50];
    int j = 0;
    for (int i = 0; i < n; i++) {
        int varg = va_arg(vargs, int);
        if (varg != 0) nums[j++] = varg;
    }
    return nums[rand() % j];
}