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
        printf("Created resources. Textures: %u %u\n", r->textures->len, r->textures->_size);
    }
    return r;
}

void load_resources(void) {
    char *fps[] = {"clr-black.png", "clr-dblue.png", "clr-lorange.png", "clr-lyellow.png",
                   "clr-lgreen.png", "bg-green.png", "clr-white.png", "title.png", "settings.png", NULL};
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

void draw_background(FileName n) {
    SDL_RenderCopy(game->renderer, arget(game->resources->textures, n), NULL, NULL);
}

void free_resources(Resources *resources) {
    for (arsize_t i = 0; i < resources->textures->len; i++)
        SDL_DestroyTexture(arget(resources->textures, i));
    ardel(resources->textures);
    ardel(resources->fonts);
    free(resources);
}
