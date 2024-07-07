/**
 * @file grid.c
 * @brief Functions and structs that create a grid which a maze can be generated from
 *        are declared here. This differs from graph in that it uses cells and walls.
 */

#pragma once

typedef struct Cell {
    struct Wall *upperwall;
    struct Wall *lowerwall;
    struct Wall *rightwall;
    struct Wall *left_wall;
    int data;
} Cell;

/**
 * @brief A wall between two cells, or one cell and NULL.
 *
 * cell1 will always be the cell upper or to the left of the wall, and
 * cell2 will always be the cell lower or to the right of the wall.
 */
typedef struct Wall {
    struct Cell *cell1;
    struct Cell *cell2;
    int exists;
    int data;
} Wall;

/**
 * @brief A grid of cells and walls inbetween them.
 */
typedef struct {
    int width;
    int height;
    int type;
    Cell **cells;
} Grid;

/**
 * @brief Generate a grid of cells with walls that can have a maze generated in.
 *
 * @param width width of the grid
 * @param height height of the grid
 * @return the cell grid that was created.
 */
Grid *generate_grid(int width, int height);

/**
 * @brief Clear a grid, setting all `data` and `exists` fields to 0.
 *
 * @param grid grid to clear.
 */
void clear_grid(Grid *grid);

/**
 * @brief Free all cells of a grid and all associated walls.
 *
 * @param grid grid to free
 * @return number of walls that was freed.
 */
int free_grid(Grid *grid);
