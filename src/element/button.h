#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "common.h"
#include "images.h"
#include "utils/array.h"

typedef struct Button {
    double x, y, w, h;          /** Percentages of the screen */
    SDL_Rect true_rect;         /** Rect last drawn to screen, always updating. */
    Uint32 held;                /** Length it has been held in millis, or 0 for released. */
    char *text;                 /** Maximum of 49 letters, or NULL */
    int text_r, text_g, text_b; /** Text color RGB */
    FileName background;
    void (*on_click)(struct Button *);
    void (*on_release)(struct Button *);
    bool enabled;
} Button;

typedef struct {
    Array *title;
    Array *settings;
    Array *maze;
} ButtonManager;

extern ButtonManager *button_manager;

/**
 * @brief Retrieve or create it if called first time.
 */
ButtonManager *get_button_manager(void);

/**
 * @brief Free the manager and all buttons.
 */
void free_button_manager(void);

void create_all_buttons(void);

void update_buttons(SDL_MouseButtonEvent e);

void enable_buttons(Page page, bool enable);

Button *create_button(Page page, double x, double y, double w, double h,
                      const char *text, int text_r, int text_g, int text_b, FileName background,
                      void (*on_click)(Button *), void (*on_release)(Button *));

void draw_button(Button *b);

/**
 * @brief Render all current buttons.
 */
void draw_enabled_buttons();
