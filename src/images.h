/**
 * @file images.c
 * @brief Image related functions and macroes are here, including screen interaction functions.
 */
#pragma once

#include <SDL.h>

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

typedef enum {
    CLR_BLACK,
    CLR_DBLUE,
    CLR_LORANGE,
    CLR_LYELLOW,
    CLR_LGREEN,
    BG_GREEN,
    CLR_WHITE,
    TITLE_SVG,
} FileName;

/**
 * @brief Cached resources.
 */
typedef struct {
    int texture_count;
    size_t texture_size;
    SDL_Texture **textures;
} Resources;

/**
 * @brief Render the title page.
 */
void draw_title();

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
