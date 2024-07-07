#include "button.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "common.h"

ButtonManager* get_button_manager(void) {
    static ButtonManager* button_manager = NULL;
    if (button_manager == NULL) {
        button_manager = calloc(1, sizeof(ButtonManager));
        button_manager->title = arnew(10);
        button_manager->maze = arnew(10);
    }
    return button_manager;
}

void free_button_manager(void) {
    for (arsize_t i = 0; i < button_manager->title->len; i++)
        free(arget(button_manager->title, i));
    for (arsize_t i = 0; i < button_manager->maze->len; i++)
        free(arget(button_manager->maze, i));
    ardel(button_manager->title);
    ardel(button_manager->maze);
    free(button_manager);
}

void title_enter_maze(Button* b) {
    logd("title_enter_maze %c", '\n');
    if (game->stage->page == TITLE_PAGE) {
        game->stage->page = MAZE_PAGE;
        b->enabled = false;
    }
}

void create_all_buttons(void) {
    create_button(TITLE_PAGE, .25, .5, .5, .1,
                  "Enter Maze", 255, 255, 255, CLR_LGREEN, &title_enter_maze, NULL)
        ->enabled = true;
}

void update_buttons(SDL_MouseButtonEvent e) {
    logd("Update button (%i, %i)\n", e.x, e.y);
    arsize_t len;
    if (game->stage->page == TITLE_PAGE)
        len = button_manager->title->len;
    else if (game->stage->page == MAZE_PAGE)
        len = button_manager->maze->len;

    for (arsize_t i = 0; i < len; i++) {
        Button* b;
        if (game->stage->page == TITLE_PAGE)
            b = arget(button_manager->title, i);
        else if (game->stage->page == MAZE_PAGE)
            b = arget(button_manager->maze, i);

        if (!b->enabled) continue;
        if (e.x > b->true_rect.x && e.y > b->true_rect.y && e.x < b->true_rect.x + b->true_rect.w && e.y < b->true_rect.y + b->true_rect.h) {
            if (e.type = SDL_MOUSEBUTTONDOWN) {
                logd("Button %s clicked\n", b->text);
                b->held = e.timestamp;
                if (b->on_click != NULL) b->on_click(b);
            } else {
                logd("Button %s released\n", b->text);
                b->held = 0;
                if (b->on_release != NULL) b->on_release(b);
            }
            break;
        }
    }
}

Button* create_button(Page page, double x, double y, double w, double h, char* text, int text_r, int text_g, int text_b, FileName background, void (*on_click)(void*), void (*on_release)(void*)) {
    Button* b = calloc(1, sizeof(Button));
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->held = 0;
    if (text != NULL) {
        b->text = calloc(50, sizeof(char));
        strcpy(b->text, text);
        b->text_r = text_r;
        b->text_g = text_g;
        b->text_b = text_b;
    }
    b->background = background;
    b->enabled = false;
    b->on_click = on_click;
    b->on_release = on_release;

    if (page == TITLE_PAGE)
        arappend(button_manager->title, b);
    else if (page == MAZE_PAGE)
        arappend(button_manager->maze, b);
    return b;
}

void draw_button(Button* b) {
    int winwidth, winheight;
    SDL_GetWindowSize(game->win, &winwidth, &winheight);

    SDL_Rect r = {.x = winwidth * b->x, .y = winheight * b->y, .w = winwidth * b->w, .h = winheight * b->h};
    SDL_Rect border = {.x = winwidth * b->x - 5, .y = winheight * b->y - 5, .w = winwidth * b->w + 10, .h = winheight * b->h + 10};
    border.x = MAX(border.x, 0);
    border.y = MAX(border.y, 0);

    b->true_rect.x = r.x;
    b->true_rect.y = r.y;
    b->true_rect.w = r.w;
    b->true_rect.h = r.h;

    SDL_RenderCopy(game->renderer, arget(game->resources->textures, CLR_BLACK), NULL, &border);
    SDL_RenderCopy(game->renderer, arget(game->resources->textures, b->background), NULL, &r);

    if (b->text != NULL) {
        SDL_Color text_color = {b->text_r, b->text_g, b->text_b};
        SDL_Surface* surface_msg = TTF_RenderText_Solid(arget(game->resources->fonts, SERIF), b->text, text_color);
        SDL_Texture* msg = SDL_CreateTextureFromSurface(game->renderer, surface_msg);

        SDL_RenderCopy(game->renderer, msg, NULL, &r);
    }
}

void draw_enabled_buttons() {
    Array* ar;
    if (game->stage->page == TITLE_PAGE)
        ar = button_manager->title;
    else if (game->stage->page == MAZE_PAGE)
        ar = button_manager->maze;
    for (arsize_t i = 0; i < ar->len; i++)
        draw_button(arget(ar, i));
}
