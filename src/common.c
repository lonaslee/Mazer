#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

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
        game->stage->page = TITLE_PAGE;

        game->settings = calloc(1, sizeof(Settings));
        game->settings->gen_interval = 1;

        game->resources = get_resources();
    }
    return game;
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
