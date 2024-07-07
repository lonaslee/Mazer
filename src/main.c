#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

/* project headers */
#include "common.h"
#include "graph/graph.h"
#include "graph/maze_algs.h"
#include "images.h"
#include "input/button.h"

// test
int tmain(int argc_, char *argv_[]) {
    return 0;
}

int main(int argc_, char *argv_[]) {
    puts("Enter.\n");
    atexit(&cleanup);
    srand(time(NULL));

    if (SDL_InitSubSystem(SDL_INIT_EVERYTHING)) EXIT_ERR("Failed to init SDL.")
    if (IMG_Init(IMG_INIT_PNG) < IMG_INIT_PNG) EXIT_ERR("Failed to init SDL image.")
    if (TTF_Init() == -1) EXIT_ERR("Failed to init SDL ttf.")

    game = get_game();
    load_resources();

    button_manager = get_button_manager();

    game->settings->gen_interval = 1;

    game->stage->g = new_graph(10, 10);
    void *state = NULL;
    bool done = false;
    long long loops = 0;

    Button *b = create_button(.25, .5, .5, .1, "Enter Maze", 255, 255, 255, CLR_LGREEN);

    SDL_Event event;
    while (++loops) {
        SDL_RenderClear(game->renderer);

        while (SDL_PollEvent(&event)) {
            on_event(&event);
        }

        if (game->stage->page == TITLE_PAGE) {
            draw_title();
        } else {
            draw_graph(game->stage->g);
            state = ellers(game->stage->g, state);
            if (state == NULL) {
                done = true;
            }

            if (llisflipped(game->stage->flags, NEW_GRAPH)) {
                llunflip(game->stage->flags, NEW_GRAPH);
                del_graph(game->stage->g);
                state = NULL;
                game->stage->g = new_graph(10, 10);
                done = false;
            }
        }
        draw_buttons();

        SDL_RenderPresent(game->renderer);
        SDL_Delay(1);
    }

    return 0;
}

void on_mousedown(SDL_MouseButtonEvent b) {
    
}

void on_keydown(SDL_KeyboardEvent k) {
    switch (k.keysym.sym) {
        case SDLK_F11: {
            bool is_fullscreen = SDL_GetWindowFlags(game->win) & SDL_WINDOW_FULLSCREEN_DESKTOP;
            SDL_SetWindowFullscreen(game->win, is_fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
        } break;

        case SDLK_r: {
            llflip(game->stage->flags, NEW_GRAPH);
        } break;

        default:
            break;
    }
}

void on_event(SDL_Event *event) {
    switch (event->type) {
        case SDL_QUIT:
            puts("User exit.");
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            on_keydown(event->key);
            break;
        case SDL_MOUSEBUTTONDOWN:
            on_mousedown(event->button);
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

    free(game->stage);
    oputc('.');

    free_resources(game->resources);
    oputc('.');

    free(game->settings);
    free(game);

    SDL_Quit();
    IMG_Quit();
    puts(" Done.");
}
