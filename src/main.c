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
#include "element/button.h"
#include "graph/graph.h"
#include "graph/maze_algs.h"
#include "images.h"

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
    create_all_buttons();

    game->stage->graph = new_graph(10, 10);
    long long loops = 0;

    while (++loops) {
        SDL_RenderClear(game->renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event))
            handle_event(&event);

        if (game->stage->page == TITLE_PAGE) {
            draw_background(TITLE_PNG);
        } else if (game->stage->page == SETTINGS_PAGE) {
            draw_background(SETTINGS_PNG);
        } else if (game->stage->page == MAZE_PAGE) {
            if (!game->stage->generated) {
                switch (game->stage->maze_type) {
                    case ALDOUS_BRODER:
                        game->stage->state = alduous_broder(game->stage->graph, game->stage->state);
                        break;
                    case BINARY_TREE:
                        game->stage->state = binary_tree(game->stage->graph, game->stage->state);
                        break;
                    case RECURSIVE_BACKTRACKER:
                        game->stage->state = recursive_backtracker(game->stage->graph, game->stage->state);
                        break;
                    case SIDEWINDER:
                        game->stage->state = sidewinder(game->stage->graph, game->stage->state);
                        break;
                    case ELLERS:
                        game->stage->state = ellers(game->stage->graph, game->stage->state);
                        break;
                    case HUNT_AND_KILL:
                        game->stage->state = hunt_and_kill(game->stage->graph, game->stage->state);
                        break;
                    default:
                        EXIT_ERR("Unknown maze type %i\n", game->stage->maze_type);
                        break;
                }

                if (game->stage->state == NULL) {
                    game->stage->generated = true;
                    puts("Finish generation.");
                }
            }
            draw_graph(game->stage->graph);
        }
        draw_enabled_buttons();

        SDL_RenderPresent(game->renderer);
        SDL_Delay(1);
    }

    return 0;
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

    free(game);

    SDL_Quit();
    IMG_Quit();
    puts(" Done.");
}
