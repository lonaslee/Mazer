#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"

/* project headers */
#include "common.h"
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
    Grid *grid = generate_grid(4, 4);
    game->stage->grid = grid;
    game->settings->step_interval = 0.2;

    MazeGenOptions mgo = {.numof = 2};
    mgo.opts[0] = 100;
    mgo.opts[1] = 15;

    gen_maze(grid, NULL, KRUSKALS);
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

        default:
            break;
    }
}

static void cleanup(void) {
    printf("Cleaning up");
    SDL_DestroyWindow(game->win);
    SDL_DestroyRenderer(game->renderer);
    oputc('.');

    free_grid(game->stage->grid);
    free(game->stage);
    oputc('.');

    free_resources(game->resources);
    oputc('.');

    free(game->settings);
    free(game);
    puts(" Done.");
}
