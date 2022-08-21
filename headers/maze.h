/**
 * @file maze.c
 * @brief All maze generation algorithm implementations and maze object creators are here.
 */
#pragma once

#include "graph.h"
#include "grid.h"

#define MOVEX(dir) ((dir == UP || dir == DOWN) ? 0 : (dir == RIGHT) ? 1 \
                                                                    : -1)
#define MOVEY(dir) ((dir == LEFT || dir == RIGHT) ? 0 : (dir == UP) ? 1 \
                                                                    : -1)
#define choicearr(n, array) (array[rand() % (n)])
#define choicerange(start, stop) ((rand() % ((stop) - (start))) + (start))
/**
 * @brief A structure to hold data on a custom maze generation options for an algorithm.
 *        Algorithms expect all arguments to be in order inside the opts field.
 */
typedef struct {
    int numof;
    int opts[100];
} MazeGenOptions;

enum DIRECTION {
    STAY,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum MazeType {
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
};

/**
 * @brief Generate a maze using the Aldous-Broder Algorithm.
 *
 * @param grid grid to generate maze in
 * @param options this algorithm has no customizeable options, this can be NULL
 * @return Grid* - same maze pointer passed
 */
Grid *gen_aldous_broder(Grid *grid, MazeGenOptions *options);

/**
 * @brief Generate a maze using the Binary Tree Algorithm.
 *
 * @param grid grid to generate maze in
 * @param options this algorithm has no customizeable options, this can be NULL
 * @return Grid* - same maze pointer passed
 */
Grid *gen_binary_tree(Grid *grid, MazeGenOptions *options);

/**
 * @brief Generate a maze using the Sidewinder Algorithm.
 *
 * @param grid grid to generate maze in
 * @param options this algorithm has 1 option: max group size
 * @return Grid* - same maze pointer passed
 */
Grid *gen_sidewinder(Grid *grid, MazeGenOptions *options);

/**
 * @brief Generate a maze using Eller's Algorithm.
 *
 * @param grid grid to generate maze in
 * @param options this algorithm has 1 option: max gropu size
 * @return Grid* - same maze pointer passed
 */
Grid *gen_ellers(Grid *grid, MazeGenOptions *options);

/**
 * @brief Maze generation functions call this function on every "step" of the algorithm.
 *
 * @param grid the grid where the maze is being generated
 * @param this_cell pointer to the current cell of the iteration of the algorithm
 * @param other_cells NULL terminated list of other cells to display
 * @param type the algorithm being used
 */
void grid_step(Grid *grid, Cell *this_cell, Cell **other_cells, enum MazeType type);

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
