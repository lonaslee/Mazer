#pragma once

#include "SDL.h"
#include "common.h"

int main(int argc, char *argv[]);

Game *get_game(void);
void on_event(SDL_Event *event);

static void testfn(void);
static void load_all_textures(void);
static void cleanup(void);
