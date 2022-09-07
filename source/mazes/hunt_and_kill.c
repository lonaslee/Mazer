#include "hunt_and_kill.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

static void hunt(Grid *grid, int *x, int *y);

Grid *gen_hunt_and_kill(Grid *grid, MazeGenOptions *options) {
    grid->type = HUNT_AND_KILL;
    int cx = rand() % grid->width;
    int cy = rand() % grid->height;
    grid->cells[cx][cy].upperwall->exists = 1;
    grid->cells[cx][cy].lowerwall->exists = 1;
    grid->cells[cx][cy].left_wall->exists = 1;
    grid->cells[cx][cy].rightwall->exists = 1;
    grid->cells[cx][cy].data = 1;

    enum DIRECTION dir;
    do {
        hunt(grid, &cx, &cy);
        // TODO kill
    } while (dir != STAY);

    return grid;
}

static void hunt(Grid *grid, int *xpos, int *ypos) {
    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            if (!grid->cells[x][y].data) {
                if (x != grid->width - 1 && grid->cells[x][y].rightwall->cell2->data)
                    ;
                else if (x != 0 && grid->cells[x][y].left_wall->cell1->data)
                    ;
                else if (y != grid->height && grid->cells[x][y].upperwall->cell1)
                    ;
                else if (y != 0 && grid->cells[x][y].lowerwall->cell2)
                    ;
                else
                    continue;
                *xpos = x;
                *ypos = y;
                return;
            }
        }
    }
    *xpos = NULL;
    *ypos = NULL;
}
