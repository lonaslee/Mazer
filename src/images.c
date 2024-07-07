#include "images.h"

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#include "SDL_image.h"
#include "common.h"

Resources *get_resources(void) {
    static Resources *r = NULL;
    if (r == NULL) {
        r = calloc(1, sizeof(Resources));
        r->textures = arnew(25);
        r->fonts = arnew(5);
    }
    return r;
}

void load_resources(void) {
    char *fps[] = {"clr-black.png", "clr-dblue.png", "clr-lorange.png", "clr-lyellow.png",
                   "clr-lgreen.png", "bg-green.png", "clr-white.png", "title.png", NULL};
    puts("Loading textures.");
    for (int i = 0; fps[i] != NULL; ++i) {
        char ffp[50] = "resources/";
        cache_texture(get_resources(), game->renderer, strncat(ffp, fps[i], 38));
        logd("    Loaded image from %s\n", ffp);
    }
    puts("Loading fonts.");
    char *p = "resources/gnufreefront/FreeSerif.otf";
    cache_font(get_resources(), p, 20);
    puts("Loading complete.");
}

SDL_Texture *cache_texture(Resources *resources, SDL_Renderer *renderer, const char *fp) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, fp);
    if (texture == NULL) EXIT_ERR("Failed to load image from %s: %s\n", fp, SDL_GetError())
    arappend(resources->textures, texture);
    return texture;
}

TTF_Font *cache_font(Resources *r, const char *fp, int pt) {
    TTF_Font *f = TTF_OpenFont(fp, pt);
    if (f == NULL) EXIT_ERR("Failed to load font from %s: %s\n", fp, TTF_GetError());
    arappend(r->fonts, f);
    return f;
}

SDL_Texture *get_texture(FileName n) {
    return arget(game->resources->textures, n);
}

TTF_Font *get_font(FontName n) {
    return arget(game->resources->fonts, n);
}

void free_resources(Resources *resources) {
    for (arsize_t i = 0; i < resources->textures->len; i++)
        SDL_DestroyTexture(arget(resources->textures, i));
    ardel(resources->textures);
    ardel(resources->fonts);
    free(resources);
}

void draw_title() {
    int winwidth, winheight;
    SDL_GetWindowSize(game->win, &winwidth, &winheight);

    SDL_Texture *base = arget(game->resources->textures, TITLE_SVG);
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
