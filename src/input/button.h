#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "images.h"
#include "utils/linkedlist.h"

typedef struct {
    double x, y, w, h;
    bool clicked;               /** Was it clicked this frame. */
    bool released;              /** Was it released this frame. */
    long held;                  /** Length it has been held in seconds, or 0 for released. */
    char *text;                 /** Maximum of 49 letters, or NULL */
    int text_r, text_g, text_b; /** Text color RGB */
    FileName background;
} Button;

typedef struct {
    LinkedList *all;
} ButtonManager;

ButtonManager *button_manager;

/**
 * @brief Retrieve or create it if called first time.
 */
ButtonManager *get_button_manager(void);

/**
 * @brief Free the manager and all buttons.
 */
void free_button_manager(void);

Button *create_button(double x, double y, double w, double h, char *text, int text_r, int text_g, int text_b, FileName background);

void delete_button(Button *b);

void draw_button(Button *b);

/**
 * @brief Render all current buttons.
 */
void draw_buttons();
