#include "binary_tree.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

void *gen_binary_tree(void *args) {
    MazeGenArg *arg = args;
    Grid *grid = arg->grid;
    grid->type = BINARY_TREE;

    Lock *statelock = get_state_lock();
    pthread_mutex_lock(statelock->mutex);
    game->state = STATE_GENERATING;
    statelock->flag = 1;
    pthread_cond_signal(statelock->cond);
    pthread_mutex_unlock(statelock->mutex);

    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            draw_grid_step(grid, &grid->cells[x][y], NULL);
            if (YESNO) {
                grid->cells[x][y].upperwall->exists = 0;
                grid->cells[x][y].left_wall->exists = 1;
            } else {
                grid->cells[x][y].left_wall->exists = 0;
                grid->cells[x][y].upperwall->exists = 1;
            }
            grid->cells[x][y].lowerwall->exists = 1;
            grid->cells[x][y].rightwall->exists = 1;
        }
    }
    for (int x = 0; x < grid->width; x++) {
        grid->cells[x][grid->height - 1].upperwall->exists = 1;
        grid->cells[x][grid->height - 1].left_wall->exists = 0;
    }
    for (int y = 0; y < grid->height; y++) {
        grid->cells[0][y].left_wall->exists = 1;
        grid->cells[0][y].upperwall->exists = 0;
    }
    grid->cells[0][grid->height - 1].upperwall->exists = 1;
    draw_grid_step(grid, NULL, NULL);
    game->state = STATE_FIN_GEN;
    return grid;
}
