/**
 * @file common.c
 * @brief Project wide declarations.
 *
 */
#pragma once

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "graph/graph.h"
#include "grid.h"

#define WIN_INIT_WIDTH 800
#define WIN_INIT_HEIGHT 600

#define oputc(char) putc((char), stdout)

#define MIN(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define MAX(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define AVG(a, b) (((a) + (b)) / 2)
#define HALF(a) ((a) / 2)
#define YESNO (rand() % 2)

#define llflip(n, p) ((n) |= (1LL << (p)))
#define llunflip(n, p) ((n) &= ~(1LL << (p)))
#define lltoggle(n, p) ((n) ^= (1LL << (p)))
#define llisflipped(n, p) ((n & (1LL << (p))) != 0)
#define llsetint(n, i) ((n) = ((n)&0xFFFFFFFF00000000) | (long long)i)
#define llgetint(n) ((int)((n)&0xFFFFFFFF))

#define isn(p) ((p) == NULL)
#define isnn(p) ((p) != NULL)
#define ifn(p) if (isn(p))
#define ifnn(p) if (isnn(p))

#define SETARR(arr, len, val)           \
    do {                                \
        for (int i = 0; i < len; i++) { \
            arr[i] = val;               \
        }                               \
    } while (0);

#define CLRARR(arr, len) SETARR(arr, len, NULL)
#define ZFLARR(arr, len) SETARR(arr, len, 0)

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__);
#define EXIT_ERR(...)           \
    do {                        \
        PRINT_ERR(__VA_ARGS__); \
        exit(EXIT_FAILURE);     \
    } while (0);
#define WTF EXIT_ERR("Impossible failure!")

/**
 * @brief Select a random element from array of n length.
 *
 * @param n length of the array
 * @param array array to select from
 */
#define choicearr(n, array) (array[rand() % (n)])

/**
 * @brief Select a random integer from a range. First number is included, last number excluded.
 *        Both numbers must be positive and unique.
 *
 * @param start start of the range, inclusive
 * @param stop end of the range, non-inclusive
 */
#define choicerange(start, stop) ((rand() % ((stop) - (start))) + (start))

/**
 * @brief Select a random integer from a range, exclusive on both ends.
 *
 * @param start start of the range, non-inclusive
 * @param stop end of the range, non-inclusive
 */
#define nincchoicerange(start, stop) choicerange(((start) + 1), (stop))

typedef enum MazeType {
    ALDOUS_BRODER,
    WILSONS,
    BINARY_TREE,
    SIDEWINDER,
    ELLERS,
    HUNT_AND_KILL,
    RECURSIVE_BACKTRACKER,
    RECURSIVE_DIVISION,
    KRUSKALS,
    PRIMS,
    GROWING_TREE,
    GROWING_BINARY_TREE
} MazeType;

/**
 * @brief The stage of the game.
 */
typedef struct {
    Grid *grid;
    Graph *g;
    long long flags;
} GameStage;

typedef enum {
    NEW_GRAPH
} StageFlags;

/**
 * @brief Cached resources.
 */
typedef struct {
    int texture_count;
    size_t texture_size;
    SDL_Texture **textures;
} Resources;

typedef struct {
    int gen_interval;  //< in loops
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

typedef struct Coord {
    int x, y;
} Coord;

/**
 * @brief Lock for threading screen modifications.
 */
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t clear_cond;
    pthread_cond_t present_cond;
    pthread_cond_t render_cond;
    int clear_flag;
    int present_flag;
    int render_flag;
} ScreenLock;

/**
 * @brief Lazy get the screen lock.
 */
ScreenLock *get_screen_lock(void);

/**
 * @brief Lock for threading.
 */
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int flag;
} Lock;

/**
 * @brief Lazy get the state lock.
 */
Lock *get_state_lock(void);

/**
 * @brief Grid and options to be cast into a void* for
 *        threading maze generation.
 */
typedef struct {
    Grid *grid;
    int numof;
    int options[100];
} MazeGenArg;

typedef struct {
    int numof;
    int opts[100];
} MazeGenOptions;

/**
 * @brief The main game object.
 */
Game *game;

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
 * @brief Pick a random non-zero number out of the given arguments.
 *
 * @param n amount of integers being passed
 * @param ... integer choices to pick from
 * @return int - random non-zero integer from arguments, or 0 if all arguments are 0
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
 * @brief Get a random number, usable across different threads.
 */
int getrand();

/**
 * @brief Handle events.
 *
 * @param event event to handle
 */
extern void on_event(SDL_Event *event);
