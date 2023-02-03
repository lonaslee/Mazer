#include "images.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

#include "common.h"
#include "destroy.h"
#include "maze.h"
#include "states/states.h"

SDL_Texture *cache_resource(Resources *resources, SDL_Renderer *renderer, const char *fp) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, fp);
    if (texture == NULL) EXIT_ERR("Failed to load image from %s: %s\n", fp, SDL_GetError())
    if (resources->texture_size < ++resources->texture_count * sizeof(SDL_Texture *)) {
        resources->texture_size *= 2;
        resources->textures = realloc(resources->textures, resources->texture_size);
    }
    resources->textures[resources->texture_count - 1] = texture;
    return texture;
}

void free_resources(Resources *resources) {
    for (int i = 0; i < resources->texture_count; i++) {
        SDL_DestroyTexture(resources->textures[i]);
    }
    free(resources->textures);
    free(resources);
}

void draw_grid(Grid *grid, Cell *this_cell, Cell **other_cells) {
    printf("S-");
    int winwidth, winheight, cellsize, wallsize, offsetx, offsety;
    SDL_GetWindowSize(game->win, &winwidth, &winheight);
    cellsize = (MIN(winwidth, winheight) * 0.95) / MAX(grid->width, grid->height);
    wallsize = cellsize / 20;
    if (wallsize < 1) wallsize = 1;
    cellsize -= wallsize;
    offsetx = HALF(winwidth - (cellsize + wallsize) * grid->width) + HALF(wallsize);
    offsety = HALF(winheight - (cellsize + wallsize) * grid->height) + HALF(wallsize);

    if (other_cells == NULL) other_cells = (Cell *[]){NULL};

    SDL_Texture *base = game->resources->textures[BG_GREEN];
    SDL_RenderCopy(game->renderer, base, NULL, NULL);

    SDL_Texture *cell_img = game->resources->textures[CLR_LYELLOW];
    SDL_Texture *cell_img2 = game->resources->textures[CLR_DBLUE];
    SDL_Texture *cell_img3 = game->resources->textures[CLR_LORANGE];

    SDL_Texture *wall_img = game->resources->textures[CLR_BLACK];

    SDL_Rect cell_rect = {.w = cellsize, .h = cellsize};
    SDL_Rect horizontal = {.w = cellsize, .h = wallsize};
    SDL_Rect vertical = {.w = wallsize, .h = cellsize};

    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            cell_rect.x = x * (cellsize + wallsize) + offsetx;
            cell_rect.y = winheight - (y + 1) * (cellsize + wallsize) - offsety;

            if (&grid->cells[x][y] == this_cell) {
                SDL_RenderCopy(game->renderer, cell_img2, NULL, &cell_rect);
            } else {
                for (int i = 0; other_cells[i] != NULL; i++)
                    if (&grid->cells[x][y] == other_cells[i]) {
                        SDL_RenderCopy(game->renderer, cell_img3, NULL, &cell_rect);
                        goto walls;  // can confirm a dinosaur jumped me
                    }
                SDL_RenderCopy(game->renderer, cell_img, NULL, &cell_rect);
            }

        walls:
            horizontal.x = cell_rect.x;
            horizontal.y = cell_rect.y - wallsize;
            SDL_RenderCopy(game->renderer, grid->cells[x][y].upperwall->exists ? wall_img : cell_img, NULL, &horizontal);

            vertical.x = cell_rect.x - wallsize;
            vertical.y = cell_rect.y;
            SDL_RenderCopy(game->renderer, grid->cells[x][y].left_wall->exists ? wall_img : cell_img, NULL, &vertical);

            horizontal.x = cell_rect.x;
            horizontal.y = cell_rect.y + cellsize;
            SDL_RenderCopy(game->renderer, grid->cells[x][y].lowerwall->exists ? wall_img : cell_img, NULL, &horizontal);

            vertical.x = cell_rect.x + cellsize;
            vertical.y = cell_rect.y;
            SDL_RenderCopy(game->renderer, grid->cells[x][y].rightwall->exists ? wall_img : cell_img, NULL, &vertical);
        }
    }
    printf("E\n");
}
