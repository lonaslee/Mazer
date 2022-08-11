#ifndef MAZER_MAIN_H
#define MAZER_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "definitions.h"
#include "maze.h"

int main(int argc, char *argv[]);
void on_event(SDL_Event *event);
void load_all_textures(void);
void cleanup(void);

#endif /* MAZER_MAIN_H */