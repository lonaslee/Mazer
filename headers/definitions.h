/**
 * @file definitions.c
 * @brief Project wide declarations.
 *
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "maze.h"

#define WIN_INIT_WIDTH 800
#define WIN_INIT_HEIGHT 600

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define MAX(a, b) (((a) >= (b)) ? (a) : (b))
#define AVG(a, b) (((a) + (b)) / 2)
#define HALF(a) ((a) / 2)
#define YESNO (rand() % 2)

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__);
#define EXIT_ERR(...)           \
    do {                        \
        PRINT_ERR(__VA_ARGS__); \
        exit(EXIT_FAILURE);     \
    } while (0);
/**
 * @brief The stage of the game.
 */
typedef struct {
    void (*keyevent_fn)(SDL_Event *event);
    void (*mouseevent_fn)(SDL_Event *event);
    Grid *grid;
    Graph *graph;
} GameStage;

/**
 * @brief Cached resources.
 */
typedef struct {
    int texture_count;
    size_t texture_size;
    SDL_Texture **textures;
} Resources;

typedef struct {
    double step_interval;
} Settings;

/**
 * @brief The main game structure.
 */
typedef struct {
    SDL_Window *win;
    SDL_Renderer *renderer;
    GameStage *stage;
    Resources *resources;
    Settings *settings;
} Game;
extern Game *game;

enum FileNames {
    CLR_BLACK,
    CLR_DBLUE,
    CLR_LORANGE,
    CLR_LYELLOW,
    CLR_LGREEN,
    BG_GREEN,
    CLR_WHITE,
};

/**
 * @brief Get the game object. Creates the game on first call, return the same pointer
 *        to it on subsequent calls.
 * @return Game* - The pointer to a game object.
 */
extern Game *get_game(void);

/**
 * @brief Handle events.
 *
 * @param event event to handle
 */
extern void on_event(SDL_Event *event);