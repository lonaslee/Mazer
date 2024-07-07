#include "images.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

#include "common.h"
#include "destroy.h"

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

void draw_title() {
    int winwidth, winheight;
    SDL_GetWindowSize(game->win, &winwidth, &winheight);

    SDL_Texture *base = game->resources->textures[TITLE_SVG];
    SDL_RenderCopy(game->renderer, base, NULL, NULL);
}
