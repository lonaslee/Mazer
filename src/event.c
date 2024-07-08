#include "event.h"

#include <stdbool.h>

#include "SDL.h"
#include "common.h"
#include "element/button.h"

void handle_event(SDL_Event *event) {
    switch (event->type) {
        case SDL_QUIT:
            puts("User exit.");
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            on_keydown(event->key);
            break;
        case SDL_KEYUP:
            on_keyup(event->key);
            break;

        case SDL_MOUSEBUTTONDOWN:
            on_mousedown(event->button);
            break;

        default:
            break;
    }
}

void on_mousedown(SDL_MouseButtonEvent b) {
    update_buttons(b);
}

void on_keydown(SDL_KeyboardEvent k) {
    switch (k.keysym.sym) {
        case SDLK_ESCAPE: {
            if (SDL_GetWindowFlags(game->win) & SDL_WINDOW_FULLSCREEN_DESKTOP)
                SDL_SetWindowFullscreen(game->win, 0);
            else if (game->stage->page == SETTINGS_PAGE)
                switch_page(TITLE_PAGE);
        } break;

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

void on_keyup(SDL_KeyboardEvent k) {
}

void switch_page(Page to) {
    Page from = game->stage->page;
    game->stage->page = to;
    enable_buttons(from, false);
    enable_buttons(to, true);
    logd("switched %i to %i\n", from, to);

    if (to == MAZE_PAGE) {
        del_graph(game->stage->graph);
        game->stage->graph = new_graph(game->stage->nc, game->stage->nr);
    }
}
