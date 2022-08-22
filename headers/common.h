/**
 * @file common.c
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

#define SETARR(arr, len, val)       \
    for (int i = 0; i < len; i++) { \
        arr[i] = val;               \
    }
#define CLRARR(arr, len) SETARR(arr, len, NULL)
#define ZFLARR(arr, len) SETARR(arr, len, 0)

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
/**
 * @brief The main game object.
 *
 */
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

typedef struct List {
    int len;
    int idx;
    void **elements;
    struct List *(*new_list)(int len);
    void (*del)(struct List *list);
    void (*append)(struct List *list, void *value);
    void (*join)(struct List *dest_list, struct List *source_list);
} List;

List *list_new(int len);
void list_del(List *list);
void list_append(List *list, void *value);
void list_join(List *dest_list, List *source_list);

/**
 * @brief Get the game object. Creates the game on first call, return the same pointer
 *        to it on subsequent calls.
 * @return Game* - The pointer to a game object.
 */
Game *get_game(void);

/**
 * @brief Get the grid resources object. The grid resources object contains textures used by print_grid and other
 *        functions to display a grid. This function creates the resource object on first call, return
 *        the same pointer to it on subsequent calls.
 *
 * @return Resources* - The pointer to a resources object.
 */
Resources *get_grid_resources(void);

/**
 * @brief Pick a random non-zero number out of the given arguments. This takes a max of 20 arguments.
 *
 * @param n amount of integers being passed
 * @param ... integer choices to pick from, max 20 vlas
 * @return int - random non-zero integer from arguments
 */
int choicenz(int n, ...);

/**
 * @brief Pick a random number out of the given arguments.
 *
 * @param n amount of integers being passed
 * @param ... integer choices to pick from
 * @return int - random integer from arguments
 */
int choice(int n, ...);

/**
 * @brief Handle events.
 *
 * @param event event to handle
 */
extern void on_event(SDL_Event *event);