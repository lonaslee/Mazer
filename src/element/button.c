#include "button.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "common.h"
#include "event.h"

ButtonManager* get_button_manager(void) {
    static ButtonManager* button_manager = NULL;
    if (button_manager == NULL) {
        button_manager = calloc(1, sizeof(ButtonManager));
        button_manager->title = arnew(10);
        button_manager->settings = arnew(10);
        button_manager->maze = arnew(10);
    }
    return button_manager;
}

void free_button_manager(void) {
    for (arsize_t i = 0; i < button_manager->title->len; i++)
        free(arget(button_manager->title, i));
    for (arsize_t i = 0; i < button_manager->settings->len; i++)
        free(arget(button_manager->settings, i));
    for (arsize_t i = 0; i < button_manager->maze->len; i++)
        free(arget(button_manager->maze, i));
    ardel(button_manager->title);
    ardel(button_manager->settings);
    ardel(button_manager->maze);
    free(button_manager);
}

void title_enter_maze(Button* b) {
    logd("title_enter_maze %c", '\n');
    switch_page(MAZE_PAGE);
}

void title_settings(Button* b) {
    logd("title_settings %c", '\n');
    switch_page(SETTINGS_PAGE);
}

void title_quit(Button* b) {
    logd("title_quit %c", '\n');
    exit(EXIT_SUCCESS);
}

void settings_maze_type(Button* b) {
    logd("settings_maze_type %c", '\n');
    game->stage->maze_type++;
    if (game->stage->maze_type == NUM_MAZE_TYPES)
        game->stage->maze_type = 0;
    strcpy(b->text, MAZE_TYPE_NAMES[game->stage->maze_type]);
}

static Button *row_label, *col_label;

void settings_row_inc1(Button* b) {
    
}

void settings_row_inc10(Button* b) {
}

void maze_settings_back(Button* b) {
    logd("maze_settings_back %c", '\n');
    switch_page(TITLE_PAGE);
}

void create_all_buttons(void) {
    create_button(TITLE_PAGE, .25, .5, .5, .1, "Enter Maze", 255, 255, 255, CLR_LGREEN, &title_enter_maze, NULL)
        ->enabled = true;
    create_button(TITLE_PAGE, .25, .62, .5, .1, " Settings ", 255, 255, 255, CLR_LGREEN, &title_settings, NULL)
        ->enabled = true;
    create_button(TITLE_PAGE, .25, .74, .5, .1, "Quit Game ", 255, 255, 255, CLR_LGREEN, &title_quit, NULL)
        ->enabled = true;

    create_button(SETTINGS_PAGE, .2, .4, .6, .1, MAZE_TYPE_NAMES[0], 255, 255, 255, CLR_DBLUE, &settings_maze_type, NULL);
    row_label = create_button(SETTINGS_PAGE, .4, .55, .2, .1, "  10  ", 255, 255, 255, CLR_BLACK, NULL, NULL);
    col_label = create_button(SETTINGS_PAGE, .4, .7, .2, .1, "  10  ", 255, 255, 255, CLR_BLACK, NULL, NULL);
    create_button(SETTINGS_PAGE, .05, .05, .05, .05,
                  "Back", 255, 255, 255, CLR_DBLUE, &maze_settings_back, NULL);

    create_button(MAZE_PAGE, .05, .05, .05, .05,
                  "Back", 255, 255, 255, CLR_DBLUE, &maze_settings_back, NULL);
}

void update_buttons(SDL_MouseButtonEvent e) {
    logd("Update button (%i, %i)\n", e.x, e.y);
    Array* ar;

    if (game->stage->page == TITLE_PAGE)
        ar = button_manager->title;
    else if (game->stage->page == SETTINGS_PAGE)
        ar = button_manager->settings;
    else if (game->stage->page == MAZE_PAGE)
        ar = button_manager->maze;

    for (arsize_t i = 0; i < ar->len; i++) {
        Button* b = arget(ar, i);

        if (!b->enabled) {
            logd("cont %s %c", b->text, '\n');
            continue;
        }
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

void enable_buttons(Page page, bool enable) {
    Array* ar;
    switch (page) {
        case TITLE_PAGE:
            ar = button_manager->title;
            break;
        case SETTINGS_PAGE:
            ar = button_manager->settings;
            break;
        case MAZE_PAGE:
            ar = button_manager->maze;
            break;
    }
    for (arsize_t i = 0; i < ar->len; i++) {
        ((Button*)arget(ar, i))->enabled = enable;
        Button* b = ((Button*)arget(ar, i));
        logd("%s enabled: %i\n", b->text, b->enabled);
    }
}

Button* create_button(Page page, double x, double y, double w, double h,
                      const char* text, int text_r, int text_g, int text_b, FileName background,
                      void (*on_click)(Button*), void (*on_release)(Button*)) {
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
    else if (page == SETTINGS_PAGE)
        arappend(button_manager->settings, b);
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
    else if (game->stage->page == SETTINGS_PAGE)
        ar = button_manager->settings;
    else if (game->stage->page == MAZE_PAGE)
        ar = button_manager->maze;
    for (arsize_t i = 0; i < ar->len; i++)
        draw_button(arget(ar, i));
}
