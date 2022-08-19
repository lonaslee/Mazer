#ifndef MAZER_MAIN_H
#define MAZER_MAIN_H

#include "SDL.h"
#include "definitions.h"

int main(int argc, char *argv[]);
Game *get_game(void);
static void testfn(void);
static void on_event(SDL_Event *event);
static void load_all_textures(void);
static void cleanup(void);

#endif /* MAZER_MAIN_H */