#include "images.h"

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#include "SDL_image.h"
#include "common.h"

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

void destroy_resources(char *type, int num, ...) {
    printf("call\n");
    char *types[] = {"Window", "Render", "Texture", "Cond", "Conditional", "Mutex", "Sem", "Semaphore"};
    for (int i = 0; i < 7; i++) {
        if (strcasecmp(type, types[i]) == 0) {
            switch (i) {
                case 0:
                    DESTROY(SDL_Window, SDL_DestroyWindow)
                case 1:
                    DESTROY(SDL_Renderer, SDL_DestroyRenderer)
                case 2:
                    DESTROY(SDL_Texture, SDL_DestroyTexture)
                case 3:
                case 4:
                    DESTROY(SDL_cond, SDL_DestroyCond)
                case 5:
                    DESTROY(SDL_mutex, SDL_DestroyMutex)
                case 6:
                case 7:
                    DESTROY(SDL_sem, SDL_DestroySemaphore)
                default:
                    PRINT_ERR("Impossible!")
                    return;
            }
        }
    }
    PRINT_ERR("Unrecognised type name in destroy_resources call.")
    return;
}
