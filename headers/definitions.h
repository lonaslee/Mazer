#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>

#include "SDL.h"
#include "maze.h"

#define WIN_INIT_WIDTH 800
#define WIN_INIT_HEIGHT 600

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define MAX(a, b) (((a) >= (b)) ? (a) : (b))
#define AVG(a, b) (((a) + (b)) / 2)
#define HALF(a) ((a) / 2)

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__);
#define EXIT_ERR(...)           \
    do {                        \
        PRINT_ERR(__VA_ARGS__); \
        exit(EXIT_FAILURE);     \
    } while (0);

typedef struct {
    void (*keyevent_fn)(SDL_Event *event);
    void (*mouseevent_fn)(SDL_Event *event);
    Grid *grid;
    Graph *graph;
} GameStage;

typedef struct {
    int texture_count;
    size_t texture_size;
    SDL_Texture **textures;
} Resources;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *renderer;
    GameStage *stage;
    Resources *resources;
} Game;
extern Game *game;

enum FileNames {
    CLR_BLACK,
    CLR_DBLUE,
    CLR_LORANGE,
    CLR_LYELLOW,
    CLR_LGREEN,
    BG_GREEN,
};

Game *get_game(void);

#endif /* DEFINITIONS_H */