#include "aldous_broder.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "images.h"
#include "maze_common.h"

void *gen_aldous_broder(void *args) {
    game->state = STATE_GENERATING;
    Grid *grid = ((MazeGenArg *)args)->grid;
    grid->type = ALDOUS_BRODER;
    int cx = rand() % grid->width;
    int cy = rand() % grid->height;
    int visited = 0;
    grid->cells[cx][cy].upperwall->exists = 1;
    grid->cells[cx][cy].lowerwall->exists = 1;
    grid->cells[cx][cy].left_wall->exists = 1;
    grid->cells[cx][cy].rightwall->exists = 1;
    grid->cells[cx][cy].data = 1;

    while (visited < grid->width * grid->height - 1) {
        for (int i = 0; i < game->settings->step_interval; i++) {
            while (game->loopstate != LOOP_MIDDLE)
                ;
            game->state = STATE_GEN_WAIT;
            draw_grid(game->stage->grid, &grid->cells[cx][cy], NULL);
            game->state = STATE_GENERATING;
        }

        enum DIRECTION dir = choicenz(4, UP * (cy != (grid->height - 1)),
                                      DOWN * (cy != 0),
                                      LEFT * (cx != 0),
                                      RIGHT * (cx != (grid->width - 1)));
        cx += MOVEX(dir);
        cy += MOVEY(dir);
        if (grid->cells[cx][cy].data) continue;
        visited++;
        grid->cells[cx][cy].data = 1;
        grid->cells[cx][cy].upperwall->exists = dir != DOWN;
        grid->cells[cx][cy].lowerwall->exists = dir != UP;
        grid->cells[cx][cy].left_wall->exists = dir != RIGHT;
        grid->cells[cx][cy].rightwall->exists = dir != LEFT;

        while (game->loopstate != LOOP_END)
            ;
    }
    game->state = STATE_IDLE;
    puts("EXIT");
    return grid;
}
