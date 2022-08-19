#include "images.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

#include "definitions.h"
#include "destroy.h"
#include "maze.h"

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
    resources->textures = NULL;
    resources->texture_count = 0;
    resources->texture_size = 0;
}

void draw_grid(Resources *resources, SDL_Renderer *renderer, Grid *grid, int xpos, int ypos) {
    int winwidth, winheight, cellsize, wallsize, offsetx, offsety;
    SDL_GetWindowSize(SDL_RenderGetWindow(renderer), &winwidth, &winheight);
    cellsize = (MIN(winwidth, winheight) * 0.95) / MAX(grid->width, grid->height);
    wallsize = cellsize / 20;
    cellsize -= wallsize;
    offsetx = HALF(winwidth - (cellsize + wallsize) * grid->width) + HALF(wallsize);
    offsety = HALF(winheight - (cellsize + wallsize) * grid->height) + HALF(wallsize);
    // printf("%d, %d, %d\n", cellsize, offsetx, offsety);

    SDL_Texture *base = resources->textures[BG_GREEN];
    SDL_RenderCopy(renderer, base, NULL, NULL);

    SDL_Texture *cell_img = resources->textures[CLR_LYELLOW];
    SDL_Texture *cell_img2 = resources->textures[CLR_LGREEN];
    SDL_Texture *wall_img = resources->textures[CLR_BLACK];
    SDL_Texture *wall_img2 = resources->textures[CLR_LORANGE];
    SDL_Rect cell_rect = {.w = cellsize, .h = cellsize},
             horizontal = {.w = cellsize, .h = wallsize},
             vertical = {.w = wallsize, .h = cellsize};
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            printf("(%d, %d)\n", x, y);
            cell_rect.x = x * (cellsize + wallsize) + offsetx;
            cell_rect.y = winheight - (y + 1) * (cellsize + wallsize) - offsety;
            printf("%d, %d\n", cell_rect.x, cell_rect.y);
            if (x == xpos && y == ypos)
                SDL_RenderCopy(renderer, cell_img, NULL, &cell_rect);
            else
                SDL_RenderCopy(renderer, cell_img2, NULL, &cell_rect);
            if (grid->cells[x][y].upperwall->exists) {
                horizontal.x = cell_rect.x;
                horizontal.y = cell_rect.y - wallsize;
                SDL_RenderCopy(renderer, wall_img2, NULL, &horizontal);
            }
            if (grid->cells[x][y].left_wall->exists) {
                vertical.x = cell_rect.x - wallsize;
                vertical.y = cell_rect.y;
                SDL_RenderCopy(renderer, wall_img, NULL, &vertical);
            }
            if (grid->cells[x][y].lowerwall->exists) {
                horizontal.x = cell_rect.x;
                horizontal.y = cell_rect.y + cellsize;
                SDL_RenderCopy(renderer, wall_img, NULL, &horizontal);
            }
            if (grid->cells[x][y].rightwall->exists) {
                vertical.x = cell_rect.x + cellsize;
                vertical.y = cell_rect.y;
                SDL_RenderCopy(renderer, wall_img, NULL, &vertical);
            }
        }
    }
}

void draw_grid_step(Resources *resources, SDL_Renderer *renderer, Grid *grid, Cell *cell_mem, Cell *um, Cell *dm, Cell *lm, Cell *rm) {
    puts("call step");
    int winwidth, winheight, cellsize, wallsize, offsetx, offsety;
    SDL_GetWindowSize(SDL_RenderGetWindow(renderer), &winwidth, &winheight);
    cellsize = (MIN(winwidth, winheight) * .95) / MAX(grid->width, grid->height);
    wallsize = cellsize / 20;
    cellsize -= wallsize;
    offsetx = HALF(winwidth - (cellsize + wallsize) * grid->width) + HALF(wallsize);
    offsety = HALF(winheight - (cellsize + wallsize) * grid->height) + HALF(wallsize);
    // printf("%d, %d, %d\n", cellsize, offsetx, offsety);
    puts("bcpy");
    SDL_Texture *base = resources->textures[BG_GREEN];
    SDL_RenderCopy(renderer, base, NULL, NULL);
    puts("rcpy");

    SDL_Texture *cell_img = resources->textures[CLR_LYELLOW];
    SDL_Texture *wall_img = resources->textures[CLR_BLACK];
    SDL_Texture *current_cell_img = resources->textures[CLR_DBLUE];
    SDL_Rect cell_rect = {.w = cellsize, .h = cellsize},
             horizontal = {.w = cellsize, .h = wallsize},
             vertical = {.w = wallsize, .h = cellsize};
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            printf("(%d, %d), ", x, y);
            cell_rect.x = x * (cellsize + wallsize) + offsetx;
            cell_rect.y = y * (cellsize + wallsize) + offsety;
            if (&grid->cells[x][y] == cell_mem || &grid->cells[x][y] == um || &grid->cells[x][y] == dm || &grid->cells[x][y] == lm || &grid->cells[x][y] == rm)
                SDL_RenderCopy(renderer, current_cell_img, NULL, &cell_rect);
            else
                SDL_RenderCopy(renderer, cell_img, NULL, &cell_rect);
            if (grid->cells[x][y].upperwall->exists) {
                horizontal.x = cell_rect.x;
                horizontal.y = cell_rect.y - wallsize;
                SDL_RenderCopy(renderer, wall_img, NULL, &horizontal);
            }
            if (grid->cells[x][y].left_wall->exists) {
                vertical.x = cell_rect.x - wallsize;
                vertical.y = cell_rect.y;
                SDL_RenderCopy(renderer, wall_img, NULL, &vertical);
            }
            if (grid->cells[x][y].lowerwall->exists) {
                horizontal.x = cell_rect.x;
                horizontal.y = cell_rect.y + cellsize;
                SDL_RenderCopy(renderer, wall_img, NULL, &horizontal);
            }
            if (grid->cells[x][y].rightwall->exists) {
                vertical.x = cell_rect.x + cellsize;
                vertical.y = cell_rect.y;
                SDL_RenderCopy(renderer, wall_img, NULL, &vertical);
            }
        }
    }
}