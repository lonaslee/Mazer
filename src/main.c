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
    create_all_buttons();

    game->settings->gen_interval = 1;

    game->stage->graph = new_graph(10, 10);
    void *state = NULL;
    long long loops = 0;

    while (++loops) {
        SDL_RenderClear(game->renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event))
            handle_event(&event);

        if (game->stage->page == TITLE_PAGE) {
            draw_background(TITLE_SVG);
        } else if (game->stage->page == SETTINGS_PAGE) {
            draw_background(CLR_BLACK);
        } else if (game->stage->page == MAZE_PAGE) {
            if (!game->stage->generated) {
                state = ellers(game->stage->graph, state);
                if (state == NULL)
                    game->stage->generated = true;
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

    free(game->settings);
    free(game);

    SDL_Quit();
    IMG_Quit();
    puts(" Done.");
}
