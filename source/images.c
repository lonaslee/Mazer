#include "images.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

#include "definitions.h"
#include "destroy.h"
#include "maze.h"

SDL_Texture *load_texture(char *fp) {
    SDL_Surface *surface = IMG_Load(fp);
    if (surface == NULL) PRINT_ERR("Failed to load image from %s: %s", fp, SDL_GetError())
    SDL_Renderer *ren = SDL_CreateSoftwareRenderer(surface);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    return texture;
}

SDL_Surface *draw_surface(void) {
    SDL_Surface *surface = IMG_Load("resources/clr-lorange.png");
    // SDL_Renderer *ren = SDL_CreateSoftwareRenderer(surface);
    // SDL_Texture *texture = IMG_LoadTexture(ren, "resources/clr-black.png");
    // SDL_RenderCopy(ren, texture, NULL, &(SDL_Rect){.h = 100, .w = 70, .x = 100, .y = 100});
    printf("return draw texture\n");
    // SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    // SDL_DestroyRenderer(ren);

    return surface;
}

void draw_grid(SDL_Renderer *renderer, Grid *grid) {
    int winwidth, winheight, cellsize;
    SDL_GetWindowSize(SDL_RenderGetWindow(renderer), &winwidth, &winheight);
    cellsize = MIN(winwidth, winheight) / MAX(grid->width, grid->height);
    cellsize -= 10;

    SDL_Texture *base = IMG_LoadTexture(renderer, "resources/clr-lorange.png");
    if (base == NULL) EXIT_ERR("Failed to load image: %s\n", SDL_GetError())
    SDL_RenderCopy(renderer, base, NULL, NULL);
    SDL_DestroyTexture(base);

    SDL_Texture *cell_img = IMG_LoadTexture(renderer, "resources/clr-dblue.png");
    SDL_Rect rect = {.w = cellsize, .h = cellsize};
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            // printf("(%d, %d), ", x, y);
            rect.x = x * (cellsize + 10);
            rect.y = y * (cellsize + 10);
            SDL_RenderCopy(renderer, cell_img, NULL, &rect);
        }
    }

    // destroy_resources("texture", 2, base, cell_img);
}