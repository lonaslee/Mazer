#include "common.h"

#include <stdlib.h>

#include "SDL.h"

List *list_new(int len) {
    List *list = calloc(1, sizeof(List));
    list->len = len;
    list->idx = 0;
    list->elements = calloc(len, sizeof(void *));

    list->new_list = &list_new;
    list->del = &list_del;
    list->append = &list_append;
    list->join = &list_join;
    return list;
}

void list_del(List *list) {
    free(list->elements);
    free(list);
}

void list_append(List *list, void *value) {
    if (list->idx > list->len) {
        list->len *= 2;
        list->elements = realloc(list->elements, list->len * sizeof(void *));
    }
    list->elements[list->idx++] = value;
}

void list_join(List *dest_list, List *source_list) {
    for (int i = 0; i < source_list->idx; i++)
        list_append(dest_list, source_list->elements[i]);
}

Game *get_game(void) {
    static Game *game = NULL;
    if (game == NULL) {
        SDL_Window *win;
        if ((win = SDL_CreateWindow("Mazer.exe",
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    WIN_INIT_WIDTH, WIN_INIT_HEIGHT,
                                    SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_FOCUS)) == NULL)
            EXIT_ERR("Failed to create window: %s\n", SDL_GetError())

        SDL_Renderer *renderer;
        if ((renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == NULL)
            EXIT_ERR("Failed to create renderer: %s\n", SDL_GetError());

        game = calloc(1, sizeof(Game));
        game->win = win;
        game->renderer = renderer;

        game->stage = calloc(1, sizeof(GameStage));

        game->settings = calloc(1, sizeof(Settings));
        game->settings->step_interval = 0;

        game->resources = calloc(1, sizeof(Resources));
        game->resources->texture_count = 0;
        game->resources->texture_size = 10 * sizeof(SDL_Texture *);
        game->resources->textures = calloc(10, sizeof(SDL_Texture *));
    }
    return game;
}

Resources *get_grid_resources(void) {
    static Resources *grid_resources = NULL;
    if (grid_resources == NULL) {
        grid_resources = calloc(1, sizeof(Resources));
        grid_resources->texture_count = 5;
        grid_resources->texture_size = 5 * sizeof(SDL_Texture *);
        grid_resources->textures = calloc(5, sizeof(SDL_Texture *));
        grid_resources->textures[0] = game->resources->textures[BG_GREEN];
        grid_resources->textures[1] = game->resources->textures[CLR_LYELLOW];
        grid_resources->textures[2] = game->resources->textures[CLR_DBLUE];
        grid_resources->textures[3] = game->resources->textures[CLR_LORANGE];
        grid_resources->textures[4] = game->resources->textures[CLR_BLACK];
    };
    return grid_resources;
}

int choice(int n, ...) {
    va_list vargs;
    va_start(vargs, n);
    int randnum = rand() % n;
    for (int i = 0; i < n; i++) {
        int varg = va_arg(vargs, int);
        if (i == randnum) return varg;
    }
}

int choicenz(int n, ...) {
    va_list vargs;
    va_start(vargs, n);
    int nums[20];
    int j = 0;
    for (int i = 0; i < n; i++) {
        int varg = va_arg(vargs, int);
        if (varg != 0) nums[j++] = varg;
    }
    return nums[rand() % j];
}