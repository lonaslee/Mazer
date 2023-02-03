#include "aldous_broder.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "images.h"
#include "maze_common.h"

void *gen_aldous_broder(void *args) {
    game->state = STATE_GENERATING;
    MazeGenArg *mga = args;
    Grid *grid = mga->grid;
    ScreenLock *lock = mga->lock;

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
            pthread_mutex_lock(&lock->mutex);
            // wait for render clear
            while (!lock->clear_flag) pthread_cond_wait(&lock->clear_cond, &lock->mutex);
            lock->clear_flag = 0;

            draw_grid(game->stage->grid, &grid->cells[cx][cy], NULL);
            lock->render_flag = 1;
            pthread_cond_signal(&lock->render_cond);

            // wait for render present
            while (!lock->present_flag) pthread_cond_wait(&lock->present_cond, &lock->mutex);
            lock->present_flag = 0;

            pthread_mutex_unlock(&lock->mutex);
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
    }
    game->state = STATE_IDLE;
    return grid;
}
