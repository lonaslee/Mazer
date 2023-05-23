#include "hunt_and_kill.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

void *gen_hunt_and_kill(void *args) {
    MazeGenArg *arg = args;
    Grid *grid = arg->grid;
    grid->type = HUNT_AND_KILL;

    int cx = rand() % grid->width;
    int cy = rand() % grid->height;
    grid->cells[cx][cy].data = 1;
    grid->cells[cx][cy].upperwall->exists = grid->cells[cx][cy].lowerwall->exists = grid->cells[cx][cy].left_wall->exists = grid->cells[cx][cy].rightwall->exists = 1;

    while (cx != -1) {
        while (!is_stuck(&grid->cells[cx][cy])) {
            grid->cells[cx][cy].data = 1;
            draw_grid_step(grid, &grid->cells[cx][cy], NULL);
            enum DIRECTION dir = choicenz(4, UP * (cy != (grid->height - 1) && grid->cells[cx][cy + 1].data != 1),
                                          DOWN * (cy != 0 && grid->cells[cx][cy - 1].data != 1),
                                          LEFT * (cx != 0 && grid->cells[cx - 1][cy].data != 1),
                                          RIGHT * (cx != (grid->width - 1) && grid->cells[cx + 1][cy].data != 1));
            carve_path(&grid->cells[cx][cy], dir);
            cx += MOVEX(dir);
            cy += MOVEY(dir);
        }
        grid->cells[cx][cy].data = 1;
        draw_grid_step(grid, &grid->cells[cx][cy], NULL);

        for (int y = grid->height - 1; y >= 0; y--) {
            for (int x = 0; x < grid->width; x++) {
                if (!grid->cells[x][y].data) {
                    if (grid->cells[x][y].rightwall->cell2 && grid->cells[x][y].rightwall->cell2->data) {
                        carve_path(&grid->cells[x + 1][y], LEFT);
                    } else if (grid->cells[x][y].left_wall->cell1 && grid->cells[x][y].left_wall->cell1->data) {
                        carve_path(&grid->cells[x - 1][y], RIGHT);
                    } else if (grid->cells[x][y].upperwall->cell1 && grid->cells[x][y].upperwall->cell1->data) {
                        carve_path(&grid->cells[x][y + 1], DOWN);
                    } else if (grid->cells[x][y].lowerwall->cell2 && grid->cells[x][y].lowerwall->cell2->data) {
                        carve_path(&grid->cells[x][y - 1], UP);
                    } else
                        continue;
                    cx = x;
                    cy = y;
                    draw_grid_step(grid, &grid->cells[x][y], NULL);
                    goto esc;
                }
            }
        }
        cx = -1;
        cy = -1;
    esc:;
    }
    return grid;
}
