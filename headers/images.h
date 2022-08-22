/**
 * @file images.c
 * @brief Image related functions and macroes are here, including screen interaction functions.
 */
#pragma once

#include <SDL.h>

#include "common.h"
#include "maze.h"

/**
 * @brief Sets the pre-declared variable texture pointer to a texture struct.
 *
 * @param renderer the renderer to use
 * @param texture the texture pointer
 * @param fp the file path of the texture to load
 */
#define TEXTURE_FROM_FILE(renderer, texture, fp)                                                                     \
    do {                                                                                                             \
        SDL_Surface *surface = IMG_Load(fp);                                                                         \
        if (surface == NULL) EXIT_ERR("Failed to load image from file: %s\n%s\n", fp, SDL_GetError());               \
        texture = SDL_CreateTextureFromSurface(renderer, surface);                                                   \
        SDL_FreeSurface(surface);                                                                                    \
        if (texture == NULL) PRINT_ERR("Failed to create texture from loaded surface: %s\n%s\n", fp, SDL_GetError()) \
    } while (0);

#define DRAW_TEXTURE(renderer, texture, xpos, ypos)              \
    do {                                                         \
        SDL_Rect rect = {.x = (xpos), .y = (ypos)};              \
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h); \
        SDL_RenderCopy(renderer, texture, NULL, &rect);          \
    } while (0);

#define DRAW_TEXTURE_WH(renderer, texture, xpos, ypos, width, height)            \
    do {                                                                         \
        SDL_Rect rect = {.x = (xpos), .y = (ypos), .w = (width), .h = (height)}; \
        SDL_RenderCopy(renderer, texture, NULL, &rect);                          \
    } while (0);

/**
 * @brief Draw a grid / maze on the screen using resources and renderer.
 *
 * @param resources resources object to get images from. this gets the background from index 0 of textures, three cell images from indices 1, 2, 3, and one wall image from index 4
 * @param renderer renderer to use
 * @param grid grid to draw
 * @param this_cell cell to highlight, or NULL
 * @param other_cells NULL terminated sequence of other cells to highlight, or NULL
 */
void draw_grid(Resources *resources, SDL_Renderer *renderer, Grid *grid, Cell *this_cell, Cell **other_cells);

/**
 * @brief Cache a resource by reading from a filepath. This expands the resource object's array when needed.
 *
 * @param resources resources object to store read texture
 * @param renderer renderer to use when reading the file
 * @param fp filepath to read from
 * @return SDL_Texture* - The texture that was read and cached.
 */
SDL_Texture *cache_resource(Resources *resources, SDL_Renderer *renderer, const char *fp);

/**
 * @brief Free a resources object. This frees the object itself and all cached resources within it.
 *
 * @param resources resources object to free
 */
void free_resources(Resources *resources);
