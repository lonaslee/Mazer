#include "maze.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "grid.h"
#include "images.h"

Grid *generate_aldous_broder(Grid *grid) {
    printf("%d, %d\n", grid->width, grid->height);
    grid->type = ALDOUS_BRODER;
    int curx = rand() % (grid->width - 1);
    int cury = rand() % (grid->height - 1);
    int visited_num = 0;
    Cell *cell = &grid->cells[curx][cury];
    cell->upperwall->exists = 1;
    cell->lowerwall->exists = 1;
    cell->left_wall->exists = 1;
    cell->rightwall->exists = 1;
    cell->data = 1;

    while (visited_num != grid->width * grid->height - 1) {
        enum DIRECTION dir = RANDDIR;
        if (curx == 0 && dir == LEFT) dir = choice(3, UP, DOWN, RIGHT);
        if (cury == 0 && dir == DOWN) dir = choice(3, UP, LEFT, RIGHT);
        if (curx == (grid->width - 1) && dir == RIGHT) dir = choice(3, UP, DOWN, LEFT);
        if (cury == (grid->height - 1) && dir == UP) dir = choice(3, DOWN, LEFT, RIGHT);
        printf("%d, (%d, %d)", dir, curx, cury);
        curx += MOVEX(dir);
        cury += MOVEY(dir);
        cell = &grid->cells[curx][cury];
        printf("(%d, %d) s%d\n", curx, cury, cell->data);
        if (cell->data == 1) {
            // printf("skip %d\n", visited_num);
            // SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            // SDL_RenderClear(game->renderer);
            // draw_grid_step(game->resources, game->renderer, grid, cell, NULL, NULL, NULL, NULL);
            // SDL_RenderPresent(game->renderer);
            // _sleep(100);
            continue;
        }
        cell->data = 1;
        visited_num++;
        cell->upperwall->exists = 1;
        cell->lowerwall->exists = 1;
        cell->left_wall->exists = 1;
        cell->rightwall->exists = 1;
        switch (dir) {
            case UP:
                cell->lowerwall->exists = 0;
                break;
            case DOWN:
                cell->upperwall->exists = 0;
                break;
            case LEFT:
                cell->rightwall->exists = 0;
                break;
            case RIGHT:
                cell->left_wall->exists = 0;
                break;
        }
        // SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        // SDL_RenderClear(game->renderer);
        // draw_grid_step(game->resources, game->renderer, grid, cell, NULL, NULL, NULL, NULL);
        // SDL_RenderPresent(game->renderer);
        // _sleep(1000);
        printf("%d, %d, %d, %d\n", cell->lowerwall->exists, cell->upperwall->exists, cell->rightwall->exists, cell->left_wall->exists);
    }
    return grid;
}

void aldous_broder(Grid *grid) {
    puts("ab");
    grid->type = ALDOUS_BRODER;
    int curx = rand() % grid->width;
    int cury = rand() % grid->height;
    printf("s: (%d, %d)\n", curx, cury);
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
        printf("(%d, %d), %d\n", curx, cury, dir);
        this_cell = &grid->cells[curx][cury];
        if (this_cell->data == 1) {printf("%d skip\n", this_cell->data);continue;}
        this_cell->data = 1;
        visited++;
        this_cell->upperwall->exists = dir != DOWN;
        this_cell->lowerwall->exists = dir != UP;
        this_cell->left_wall->exists = dir != RIGHT;
        this_cell->rightwall->exists = dir != LEFT;
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        draw_grid_step(game->resources, game->renderer, grid, this_cell, NULL, NULL, NULL, NULL);
        SDL_RenderPresent(game->renderer);
        // _sleep(1000);
        puts("i");
    }
    printf("v::: %d\n" , visited);
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
    printf("\n::: ");
    for (int i = 0; i < n; i++) {
        int varg = va_arg(vargs, int);
        printf("%d, ", varg);
        if (varg != 0) nums[j++] = varg;
    }
    puts("");
    return nums[rand() % j];
}