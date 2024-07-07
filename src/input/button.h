#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "common.h"
#include "utils/array.h"

typedef struct {
    SDL_Rect *rect;
    bool clicked;  /** Was it clicked this frame. */
    bool released; /** Was it released this frame. */
    long held;     /** Length it has been held in seconds, or 0 for released. */
    char *text;    /** Maximum of 49 letters. */
    FileName background;
} Button;

typedef struct {
    Array *all;
} ButtonManager;

ButtonManager *button_manager;

/**
 * @brief Retrieve or create it if called first time.
 */
ButtonManager *get_button_manager(void);

Button *create_button(int x, int y, int w, int h, char *text, FileName background);
