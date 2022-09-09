#include "hunt_and_kill.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

static void hunt(Grid *grid, int *xpos, int *ypos, enum DIRECTION *dir);
static void kill(Grid *grid, int *cx, int *cy, enum DIRECTION dir);

Grid *gen_hunt_and_kill(Grid *grid, MazeGenOptions *options) {  // FIXME hunt direction
    grid->type = HUNT_AND_KILL;
    int cx = rand() % grid->width;
    int cy = rand() % grid->height;
    enum DIRECTION dir = STAY;
    grid->cells[cx][cy].upperwall->exists = grid->cells[cx][cy].lowerwall->exists = grid->cells[cx][cy].left_wall->exists = grid->cells[cx][cy].rightwall->exists = 1;
    while (cx != -1) {
        grid->cells[cx][cy].upperwall->exists = dir != DOWN;
        grid->cells[cx][cy].lowerwall->exists = dir != UP;
        grid->cells[cx][cy].left_wall->exists = dir != RIGHT;
        grid->cells[cx][cy].rightwall->exists = dir != LEFT;
        puts("KILL BEG");
        kill(grid, &cx, &cy, dir);
        puts("KILL END; HUNT BEG");
        hunt(grid, &cx, &cy, &dir);
        puts("HUNT END");
    }
}

static inline int is_stuck(Cell *cell) {
    return (!cell->upperwall->cell1 || cell->upperwall->cell1->data) && (!cell->lowerwall->cell2 || cell->lowerwall->cell2->data) && (!cell->left_wall->cell1 || cell->left_wall->cell1->data) && (!cell->rightwall->cell2 || cell->rightwall->cell2->data);
}

static void kill(Grid *grid, int *cx, int *cy, enum DIRECTION dir) {
    do {
        grid->cells[*cx][*cy].data = 1;
        draw_grid_step(grid, &grid->cells[*cx][*cy], NULL);
        carve_path(grid, *cx, *cy, dir);
        *cx += MOVEX(dir);
        *cy += MOVEY(dir);
        dir = choicenz(4, UP * (*cy != (grid->height - 1) && grid->cells[*cx][*cy + 1].data != 1),
                       DOWN * (*cy != 0 && grid->cells[*cx][*cy - 1].data != 1),
                       LEFT * (*cx != 0 && grid->cells[*cx - 1][*cy].data != 1),
                       RIGHT * (*cx != (grid->width - 1) && grid->cells[*cx + 1][*cy].data != 1));
    } while (!is_stuck(&grid->cells[*cx][*cy]));
    grid->cells[*cx][*cy].data = 1;
    print_grid(grid);
}

static void hunt(Grid *grid, int *xpos, int *ypos, enum DIRECTION *dir) {
    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            if (!grid->cells[x][y].data) {
                if (x != grid->width - 1 && grid->cells[x][y].rightwall->cell2->data)
                    *dir = LEFT;
                else if (x != 0 && grid->cells[x][y].left_wall->cell1->data)
                    *dir = RIGHT;
                else if (y != grid->height - 1 && grid->cells[x][y].upperwall->cell1->data)
                    *dir = UP;
                else if (y != 0 && grid->cells[x][y].lowerwall->cell2->data)
                    *dir = DOWN;
                else
                    continue;
                printf("HUNT RET (%d, %d) %d\n", x, y, *dir);
                *xpos = x;
                *ypos = y;
                return;
            }
        }
    }
    puts("HUNT RET DEF");
    *xpos = -1;
    *ypos = -1;
}
