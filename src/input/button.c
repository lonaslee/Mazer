#include "button.h"

#include "common.h"
#include "utils/array.h"

ButtonManager* get_button_manager(void) {
    static ButtonManager* button_manager = NULL;
    if (button_manager == NULL) {
        button_manager = calloc(1, sizeof(ButtonManager));
        button_manager->all = arnew(50);
    }
    return button_manager;
}

Button* create_button(int x, int y, int w, int h, char* text, FileName background) {
    Button* b = calloc(1, sizeof(Button));
    SDL_Rect* r = calloc(1, sizeof(SDL_Rect));
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
    b->rect = r;
    b->clicked = false;
    b->released = false;
    b->held = 0;
    b->text = calloc(50, sizeof(char));
    strcpy(b->text, text);

    arappend(button_manager->all, b);
    return b;
}

void draw_button(Button* b) {
    int winwidth, winheight;
    SDL_GetWindowSize(game->win, &winwidth, &winheight);

    SDL_Rect r = {.x = winwidth * b->rect->x, .y = winheight * b->rect->y, .w = winwidth * b->rect->w, .h = winheight * b->rect->h};

    SDL_Texture* bg = game->resources->textures[b->background];
    SDL_RenderCopy(game->renderer, bg, NULL, &r);
}

void draw_buttons() {
    
}
