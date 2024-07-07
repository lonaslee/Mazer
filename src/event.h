#pragma once

#include "SDL.h"
#include "common.h"

void handle_event(SDL_Event *event);

void on_mousedown(SDL_MouseButtonEvent b);

void on_keydown(SDL_KeyboardEvent k);
void on_keyup(SDL_KeyboardEvent k);

void switch_page(Page to);
