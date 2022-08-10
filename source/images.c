#include "images.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

#include "definitions.h"
#include "destroy.h"
#include "main.h"
#include "maze.h"

void cache_resource(struct Resources *resources, SDL_Renderer *renderer, char *fp) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, fp);
    if (resources->texture_size < ++resources->texture_count * sizeof(SDL_Texture *)) {
        resources->texture_size *= 2;
        resources->textures = realloc(resources->textures, resources->texture_size);
    }
    resources->textures[resources->texture_count - 1] = texture;
}

void free_resources(struct Resources *resources) {
    for (int i = 0; i < resources->texture_count; i++) {
        SDL_DestroyTexture(resources->textures[i]);
    }
    free(resources->textures);
    resources->textures = NULL;
    resources->texture_count = 0;
    resources->texture_size = 0;
}

void draw_grid(struct Resources *resources, SDL_Renderer *renderer, Grid *grid) {
    int winwidth, winheight, cellsize, wallsize, offsetx, offsety;
    SDL_GetWindowSize(SDL_RenderGetWindow(renderer), &winwidth, &winheight);
    cellsize = MIN(winwidth, winheight) / MAX(grid->width, grid->height);
    wallsize = cellsize / 10;
    cellsize -= wallsize;
    offsetx = HALF(winwidth - (cellsize + wallsize) * grid->width) + HALF(wallsize);
    offsety = HALF(winheight - (cellsize + wallsize) * grid->height) + HALF(wallsize);
    // printf("%d, %d, %d\n", cellsize, offsetx, offsety);

    SDL_Texture *base = resources->textures[CLR_BLACK];
    SDL_RenderCopy(renderer, base, NULL, NULL);

    SDL_Texture *cell_img = resources->textures[CLR_DBLUE];
    SDL_Rect rect = {.w = cellsize, .h = cellsize};
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            // printf("(%d, %d), ", x, y);
            rect.x = x * (cellsize + wallsize) + offsetx;
            rect.y = y * (cellsize + wallsize) + offsety;
            SDL_RenderCopy(renderer, cell_img, NULL, &rect);
        }
    }
}