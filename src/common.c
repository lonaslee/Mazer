#include "common.h"

#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "images.h"

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
    int *nums = calloc(n, sizeof(int));
    int j = 0;
    int varg;
    for (int i = 0; i < n; i++)
        if ((varg = va_arg(vargs, int)) != 0) nums[j++] = varg;
    if (j == 0) {
        free(nums);
        return 0;
    }
    int res = nums[rand() % j];
    free(nums);
    return res;
}
