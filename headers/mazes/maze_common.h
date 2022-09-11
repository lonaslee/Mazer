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

#define MOVEX(dir) ((dir) == RIGHT ? 1 : (dir) == LEFT ? -1 \
                                                       : 0)
#define MOVEY(dir) ((dir) == UP ? 1 : (dir) == DOWN ? -1 \
                                                    : 0)

/**
 * @brief Check if all cells around the given cell have occupied `data` fields.
 *
 * @param cell cell to check
 * @return int
 */
int is_stuck(Cell *cell);

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
