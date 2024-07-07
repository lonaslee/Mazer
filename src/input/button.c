#include "button.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "common.h"

ButtonManager* get_button_manager(void) {
    static ButtonManager* button_manager = NULL;
    if (button_manager == NULL) {
        button_manager = calloc(1, sizeof(ButtonManager));
        button_manager->all = llnew();
    }
    return button_manager;
}

void free_button_manager(void) {
    LLFOREACH(n, button_manager->all) {
        free(n->data);
    }
    lldel(button_manager->all);
    free(button_manager);
}

void update_buttons(SDL_MouseButtonEvent e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        LLFOREACH(n, button_manager->all) {
            Button* b = n->data;
            if (e.x > b->true_rect.x && e.y > b->true_rect.y)
            {
                /* code */
            }
            
        }
    } else {
    }
}

Button* create_button(double x, double y, double w, double h, char* text, int text_r, int text_g, int text_b, FileName background) {
    Button* b = calloc(1, sizeof(Button));
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->clicked = false;
    b->released = false;
    b->held = 0;
    if (text != NULL) {
        b->text = calloc(50, sizeof(char));
        strcpy(b->text, text);
        b->text_r = text_r;
        b->text_g = text_g;
        b->text_b = text_b;
    }
    b->background = background;

    llappend(button_manager->all, b);
    return b;
}

/**
 * @brief Free a button and remove it from the manager.
 */
void delete_button(Button* b) {
    llsize_t i = llindex(button_manager->all, b);
    llremove(button_manager->all, i);

    logd("Deleting button %s at %p\n", b->text, b);
    free(b->text);
    free(b);
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

void draw_buttons() {
    LLFOREACH(n, button_manager->all) {
        draw_button(n->data);
    }
}
