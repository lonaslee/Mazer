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

struct GameStage {
    void (*keyevent_fn)(SDL_Event *event);
    void (*mouseevent_fn)(SDL_Event *event);
    Grid *grid;
    Graph *graph;
};

struct GameInputState {
};

struct Resources {
    int texture_count;
    size_t texture_size;
    SDL_Texture **textures;
};

typedef struct {
    SDL_Window *win;
    SDL_Renderer *renderer;
    struct GameStage stage;
    struct GameInputState input_state;
    struct Resources *resources;
} Game;
Game *game;

enum FileNames {
    CLR_BLACK,
    CLR_DBLUE,
    CLR_LORANGE,
};

int main(int argc, char *argv[]);
void on_event(SDL_Event *event);
void load_all_textures(void);
void cleanup(void);

#endif /* MAZER_MAIN_H */