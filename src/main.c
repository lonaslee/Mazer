#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"

/* project headers */
#include "common.h"
#include "graph/graph.h"
#include "graph/maze_algs.h"
#include "images.h"
#include "screen.h"

static int argc;
static char **argv;

// test
int tmain(int argc_, char *argv_[]) {
    return 0;
}

int main(int argc_, char *argv_[]) {
    puts("Enter.\n");
    atexit(&cleanup);
    srand(time(0));
    argc = argc_;
    argv = argv_;

    if (SDL_InitSubSystem(SDL_INIT_EVERYTHING)) EXIT_ERR("Failed to init SDL.")
    if (IMG_Init(IMG_INIT_PNG) < IMG_INIT_PNG) EXIT_ERR("Failed to init SDL image.")

    game = get_game();

    puts("Loading textures:");
    load_all_textures();
    puts("");

    int w = 5, h = 5;
    parse_argv(&w, &h);

    game->stage->g = new_graph(h, w);
    void *state = NULL;
    bool done = false;
    long long loops = 0;

    SDL_Event event;
    while (++loops) {
        SDL_RenderClear(game->renderer);

        while (SDL_PollEvent(&event)) {
            on_event(&event);
        }

        draw_graph(game->stage->g);
        if (loops % game->settings->gen_interval == 0) {
            if (!done)
                state = hunt_and_kill(game->stage->g, state);
            if (state == NULL)
                done = true;
        }

        if (llisflipped(game->stage->flags, NEW_GRAPH)) {
            llunflip(game->stage->flags, NEW_GRAPH);
            del_graph(game->stage->g);
            game->stage->g = new_graph(h, w);
            done = false;
        }

        SDL_RenderPresent(game->renderer);
        SDL_Delay(1);
    }

    return 0;
}

static void parse_argv(int *w, int *h) {
    for (int i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }

    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'w':
                case 'W': {
                    if (strlen(argv[i]) == 2)
                        *w = atoi(argv[++i]);
                    else
                        *w = atoi(&argv[i][2]);
                    break;
                }
                case 'h':
                case 'H': {
                    if (strlen(argv[i]) == 2)
                        *h = atoi(argv[++i]);
                    else
                        *h = atoi(&argv[i][2]);
                    break;
                }
                default:
                    EXIT_ERR("Unrecognised option '-%c'. Exit.\n", argv[i][1]);
                    break;
            }
        }
    }
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

    free_grid(game->stage->grid);
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
