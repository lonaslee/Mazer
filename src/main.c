#include "main.h"

#include <pthread.h>
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
#include "states/states.h"

static int argc;
static char **argv;

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

    int w = 10, h = 10;
    parse_argv(&w, &h);
    game->stage->grid = generate_grid(w, h);
    game->settings->step_interval = 5;

    ScreenLock scrnlock = {.mutex = PTHREAD_MUTEX_INITIALIZER,
                           .clear_cond = PTHREAD_COND_INITIALIZER,
                           .present_cond = PTHREAD_COND_INITIALIZER,
                           .render_cond = PTHREAD_COND_INITIALIZER,
                           .clear_flag = 0,
                           .present_flag = 0,
                           .render_flag = 0};
    pthread_t gen_thrd;
    MazeGenArg arg = {.grid = game->stage->grid, .options = NULL, .lock = &scrnlock};
    pthread_create(&gen_thrd, NULL, gen_aldous_broder, &arg);
    pthread_detach(gen_thrd);

    SDL_Event event;
    while (1) {
        pthread_mutex_lock(&scrnlock.mutex);
        SDL_RenderClear(game->renderer);
        scrnlock.clear_flag = 1;
        pthread_cond_signal(&scrnlock.clear_cond);
        pthread_mutex_unlock(&scrnlock.mutex);

        while (SDL_PollEvent(&event)) {
            on_event(&event);
        }

        switch (game->state) {
            case STATE_IDLE:
                draw_grid(game->stage->grid, NULL, NULL);
                break;

            case STATE_GENERATING:
                pthread_mutex_lock(&scrnlock.mutex);
                while (!scrnlock.render_flag) pthread_cond_wait(&scrnlock.render_cond, &scrnlock.mutex);
                scrnlock.render_flag = 0;
                pthread_mutex_unlock(&scrnlock.mutex);
                break;

            default:
                break;
        }

        pthread_mutex_lock(&scrnlock.mutex);
        SDL_RenderPresent(game->renderer);
        SDL_Delay(1);
        scrnlock.present_flag = 1;
        pthread_cond_signal(&scrnlock.present_cond);
        pthread_mutex_unlock(&scrnlock.mutex);
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

    SDL_Quit();
    IMG_Quit();
    puts(" Done.");
}
