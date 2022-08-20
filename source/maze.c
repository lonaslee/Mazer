#include "maze.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graph.h"
#include "grid.h"
#include "images.h"

Grid *gen_aldous_broder(Grid *grid, MazeGenOptions *options) {
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
        grid_step(grid, &grid->cells[curx][cury], ALDOUS_BRODER);
        enum DIRECTION dir = choicenz(4, UP * (cury != (grid->height - 1)),
                                      DOWN * (cury != 0),
                                      LEFT * (curx != 0),
                                      RIGHT * (curx != (grid->width - 1)));
        curx += MOVEX(dir);
        cury += MOVEY(dir);
        this_cell = &grid->cells[curx][cury];
        if (this_cell->data == 1) continue;
        this_cell->data = 1;
        visited++;
        this_cell->upperwall->exists = dir != DOWN;
        this_cell->lowerwall->exists = dir != UP;
        this_cell->left_wall->exists = dir != RIGHT;
        this_cell->rightwall->exists = dir != LEFT;
    }
    return grid;
}

Grid *gen_wilsons(Grid *grid) {
}

Grid *gen_binary_tree(Grid *grid, MazeGenOptions *options) {
    puts("binary tree");
    grid->type = BINARY_TREE;
    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            grid_step(grid, &grid->cells[x][y], BINARY_TREE);
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
    return grid;
}

Grid *gen_sidewinder(Grid *grid, MazeGenOptions *options) {
    puts("sidewinder");
    grid->type = SIDEWINDER;
    int group_size_max;
    if (options != NULL)
        group_size_max = options->opts[0] != 0 ? options->opts[0] : 5;
    else
        group_size_max = 5;
    int randdiv = group_size_max - (group_size_max == 5 ? 2 : 0);
    int group = 0;
    printf("%d, %d\n", group_size_max, randdiv);
    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            grid_step(grid, &grid->cells[x][y], SIDEWINDER);
            printf("i, ");
            if (!(rand() % randdiv) || group == group_size_max) {
                if (group == 0)
                    grid->cells[x][y].upperwall->exists = 1;
                else
                    grid->cells[x][choicerange(0, group + 1)].upperwall->exists = 1;
                group = 0;
            } else {
                group++;
                grid->cells[x][y].rightwall->exists = 0;
                printf("p");
            }
        }
    }
}

void grid_step(Grid *grid, Cell *cur_cell, enum MazeType type) {
    Game *game = get_game();
    if (game->settings->step_interval == 0) return;
    clock_t start = clock();
    SDL_Event event;
    while ((double)(clock() - start) / CLOCKS_PER_SEC < game->settings->step_interval) {
        while (SDL_PollEvent(&event)) {
            on_event(&event);
        }
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        draw_grid(game->resources, game->renderer, grid, cur_cell);
        SDL_RenderPresent(game->renderer);
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
    int nums[20];
    int j = 0;
    for (int i = 0; i < n; i++) {
        int varg = va_arg(vargs, int);
        if (varg != 0) nums[j++] = varg;
    }
    return nums[rand() % j];
}