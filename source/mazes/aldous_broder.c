#include "aldous_broder.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

Grid *gen_aldous_broder(Grid *grid, MazeGenOptions *options) {
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
        draw_grid_step(grid, &grid->cells[curx][cury], NULL);
        enum DIRECTION dir = choicenz(4, UP * (cury != (grid->height - 1)),
                                      DOWN * (cury != 0),
                                      LEFT * (curx != 0),
                                      RIGHT * (curx != (grid->width - 1)));
        curx += MOVEX(dir);
        cury += MOVEY(dir);
        this_cell = &grid->cells[curx][cury];
        if (this_cell->data) continue;
        this_cell->data = 1;
        visited++;
        this_cell->upperwall->exists = dir != DOWN;
        this_cell->lowerwall->exists = dir != UP;
        this_cell->left_wall->exists = dir != RIGHT;
        this_cell->rightwall->exists = dir != LEFT;
    }
    return grid;
}