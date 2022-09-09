/**
 * @file maze_common.c
 * @brief Declarations used by maze generating functions declared here.
 */
#pragma once

#include "grid.h"

enum DIRECTION {
    STAY,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#define MOVEX(dir) (((dir) == UP || (dir) == DOWN || (dir) == STAY) ? 0 : ((dir) == RIGHT) ? 1 \
                                                                                           : -1)
#define MOVEY(dir) (((dir) == LEFT || (dir) == RIGHT || (dir) == STAY) ? 0 : ((dir) == UP) ? 1 \
                                                                                           : -1)

/**
 * @brief Maze generation functions call this function on every "step" of the algorithm to display the step.
 *
 * @param grid the grid where the maze is being generated
 * @param this_cell pointer to the current cell of the iteration of the algorithm
 * @param other_cells NULL terminated list of other cells to display
 * @param type the algorithm being used
 */
void draw_grid_step(Grid *grid, Cell *this_cell, Cell **other_cells);

/* For testing */
void print_grid(Grid *grid);

void carve_path(Grid *grid, int x, int y, enum DIRECTION dir);
