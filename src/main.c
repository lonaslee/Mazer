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
#include "input/button.h"
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
    srand(time(NULL));
    argc = argc_;
    argv = argv_;

    if (SDL_InitSubSystem(SDL_INIT_EVERYTHING)) EXIT_ERR("Failed to init SDL.")
    if (IMG_Init(IMG_INIT_PNG) < IMG_INIT_PNG) EXIT_ERR("Failed to init SDL image.")

    game = get_game();

    button_manager = get_button_manager();

    load_all_textures();

    int opts[3] = {5, 5, 0};
    parse_argv(opts);
    game->settings->gen_interval = opts[2];

    game->stage->g = new_graph(opts[1], opts[0]);
    void *state = NULL;
    bool done = false;
    long long loops = 0;

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
                game->stage->g = new_graph(opts[1], opts[0]);
                done = false;
            }
        }

        SDL_RenderPresent(game->renderer);
        SDL_Delay(1);
    }

    return 0;
}

static void parse_argv(int *is) {
    for (int i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }

    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'w':
                case 'W': {
                    if (strlen(argv[i]) == 2)
                        is[0] = atoi(argv[++i]);
                    else
                        is[0] = atoi(&argv[i][2]);
                } break;
                case 'h':
                case 'H': {
                    if (strlen(argv[i]) == 2)
                        is[1] = atoi(argv[++i]);
                    else
                        is[1] = atoi(&argv[i][2]);
                } break;
                case 'i':
                case 'I': {
                    if (strlen(argv[i]) == 2)
                        is[2] = atoi(argv[++i]);
                    else
                        is[2] = atoi(&argv[i][2]);
                } break;
                default:
                    EXIT_ERR("Unrecognised option '-%c'. Exit.\n", argv[i][1]);
                    break;
            }
        }
    }
}

static void load_all_textures(void) {
    char *fps[] = {"clr-black.png", "clr-dblue.png", "clr-lorange.png", "clr-lyellow.png",
                   "clr-lgreen.png", "bg-green.png", "clr-white.png", "title.png", NULL};
    puts("Loading textures:");
    for (int i = 0; fps[i] != NULL; ++i) {
        char ffp[50] = "resources/";
        cache_resource(game->resources, game->renderer, strncat(ffp, fps[i], 38));
        printf("    Loaded image from %s\n", ffp);
    }
    puts("Loading complete.");
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
