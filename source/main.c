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

int main(int argc, char *argv[]) {
    puts("Enter.");
    srand(time(0));
    atexit(cleanup);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) EXIT_ERR("Failed to intialize SDL: %s\n", SDL_GetError())
    SDL_Window *win = SDL_CreateWindow("Mazer.exe",
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       WIN_INIT_WIDTH, WIN_INIT_HEIGHT,
                                       SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_FOCUS);
    if (win == NULL) EXIT_ERR("Failed to create window: %s\n", SDL_GetError())

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) EXIT_ERR("Failed to create renderer: %s\n", SDL_GetError());

    game = calloc(1, sizeof(Game));
    game->win = win;
    game->renderer = renderer;

    game->stage.grid = generate_aldous_broder(generate_grid(5, 6));
    print_grid(game->stage.grid);

    printf("%d, %d\n", game->stage.grid->width, game->stage.grid->height);
    for (int x = 0; x < game->stage.grid->width; x++) {
        for (int y = 0; y < game->stage.grid->height; y++) {
            printf("%p, ", game->stage.grid->cells[x][y]);
        }
        puts("");
    }

    SDL_bool quit = SDL_FALSE;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            on_event(&event);
        }
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        draw_grid(game->renderer, game->stage.grid);
        SDL_RenderPresent(game->renderer);
    }
    return 0;
}

void on_event(SDL_Event *event) {
    switch (event->type) {
        case SDL_QUIT:
            puts("User exit.");
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            game->stage.keyevent_fn(event);
            break;

        default:
            break;
    }
}

void on_startscreen_keyevent(SDL_Event *event) {
    int x, y;
    int ms = SDL_GetMouseState(&x, &y);
}

void cleanup(void) {
    printf("Cleaning up...");
    SDL_DestroyWindow(game->win);
    SDL_DestroyRenderer(game->renderer);
    free_grid(game->stage.grid);
    free_graph(game->stage.graph);
    free(game);
    printf(" Done.\n");
}