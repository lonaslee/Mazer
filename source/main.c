#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"

/* project headers */
#include "definitions.h"
#include "images.h"
#include "maze.h"

Game *game;

int main(int argc, char *argv[]) {
    puts("Enter.\n");
    srand(time(0));
    atexit(&cleanup);

    game = get_game();

    puts("Loading textures:");
    load_all_textures();
    puts("");

    testfn();

    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            on_event(&event);
        }
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        draw_grid(get_grid_resources(), game->renderer, game->stage->grid, NULL, NULL);
        SDL_RenderPresent(game->renderer);
    }

    return 0;
}

static void testfn(void) {
    Game *game = get_game();
    Grid *grid = generate_grid(10, 10);
    game->stage->grid = grid;
    game->settings->step_interval = 0.1;

    gen_ellers(grid, NULL);
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

static void load_all_textures(void) {
    char *fps[] = {"clr-black.png", "clr-dblue.png", "clr-lorange.png", "clr-lyellow.png",
                   "clr-lgreen.png", "bg-green.png", "clr-white.png", NULL};
    for (int i = 0; fps[i] != NULL; ++i) {
        char ffp[50] = "resources/";
        cache_resource(game->resources, game->renderer, strncat(ffp, fps[i], 38));
        printf("    Loaded image from %s\n", ffp);
    }
}

void on_event(SDL_Event *event) {
    switch (event->type) {
        case SDL_QUIT:
            puts("User exit.");
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            game->stage->keyevent_fn(event);
            break;

        default:
            break;
    }
}

static void on_startscreen_keyevent(SDL_Event *event) {
    int x, y;
    int ms = SDL_GetMouseState(&x, &y);
}

static void cleanup(void) {
    printf("Cleaning up");
    SDL_DestroyWindow(game->win);
    SDL_DestroyRenderer(game->renderer);
    putc('.', stdout);

    free_grid(game->stage->grid);
    free_graph(game->stage->graph);
    free(game->stage);
    putc('.', stdout);

    free_resources(game->resources);
    putc('.', stdout);

    free(game->settings);
    free(get_grid_resources()->textures);
    free(get_grid_resources());

    free(game);
    puts(" Done.");
}