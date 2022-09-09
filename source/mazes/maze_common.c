#include "maze_common.h"

#include <stdlib.h>
#include <time.h>

#include "SDL.h"
#include "common.h"
#include "grid.h"
#include "images.h"

void draw_grid_step(Grid *grid, Cell *this_cell, Cell **other_cells) {
    Game *game = get_game();
    if (game->settings->step_interval == 0) return;
    clock_t start = clock();
    SDL_Event event;
    while ((double)(clock() - start) / (double)CLOCKS_PER_SEC < game->settings->step_interval) {
        while (SDL_PollEvent(&event)) {
            on_event(&event);
        }
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        draw_grid(get_grid_resources(), game->renderer, grid, this_cell, other_cells);
        SDL_RenderPresent(game->renderer);
    }
}

void carve_path(Grid *grid, int x, int y, enum DIRECTION dir) {
    Cell next = grid->cells[x + MOVEX(dir)][y + MOVEY(dir)];
    if (dir != STAY)
        next.upperwall->exists = next.lowerwall->exists = next.rightwall->exists = next.left_wall->exists = 1;
    switch (dir) {
        case UP:
            next.lowerwall->exists = 0;
            break;
        case DOWN:
            next.upperwall->exists = 0;
            break;
        case LEFT:
            next.rightwall->exists = 0;
            break;
        case RIGHT:
            next.left_wall->exists = 0;
            break;
        default:
            break;
    }
}

void print_grid(Grid *grid) {
    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            printf("%3d ", grid->cells[x][y].data);
        }
        oputc('\n');
    }
    oputc('\n');
}
