#include "maze_common.h"

#include <stdlib.h>
#include <time.h>

#include "SDL.h"
#include "common.h"
#include "grid.h"
#include "images.h"

void draw_grid_step(Grid *grid, Cell *this_cell, Cell **other_cells) {
    if (game->settings->gen_interval == 0) return;
    Game *game = get_game();
    clock_t start = clock();
    SDL_Event event;
    while ((double)(clock() - start) / (double)CLOCKS_PER_SEC < game->settings->gen_interval) {
        while (SDL_PollEvent(&event)) {
            handle_event(&event);
        }
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        draw_grid(grid, this_cell, other_cells);
        SDL_RenderPresent(game->renderer);
    }
}

int is_stuck(Cell *cell) {
    return (!cell->upperwall->cell1 || cell->upperwall->cell1->data) &&
           (!cell->lowerwall->cell2 || cell->lowerwall->cell2->data) &&
           (!cell->left_wall->cell1 || cell->left_wall->cell1->data) &&
           (!cell->rightwall->cell2 || cell->rightwall->cell2->data);
}

int is_surrounded(Cell *cell) {
    return cell->upperwall->exists && cell->lowerwall->exists && cell->left_wall->exists && cell->rightwall->exists;
}

void carve_path(Cell *cell, enum DIRECTION dir) {
    Cell *next = cell_at(cell, dir);
    next->upperwall->exists = dir != DOWN;
    next->lowerwall->exists = dir != UP;
    next->left_wall->exists = dir != RIGHT;
    next->rightwall->exists = dir != LEFT;
}

Cell *cell_at(Cell *cell, enum DIRECTION dir) {
    if (!cell) return NULL;
    switch (dir) {
        case UP:
            return cell->upperwall ? cell->upperwall->cell1 : NULL;
        case DOWN:
            return cell->lowerwall ? cell->lowerwall->cell2 : NULL;
        case LEFT:
            return cell->left_wall ? cell->left_wall->cell1 : NULL;
        case RIGHT:
            return cell->rightwall ? cell->rightwall->cell2 : NULL;
        default:
            return cell;
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
