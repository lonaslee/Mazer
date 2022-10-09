/**
 * @file maze.h
 * @brief All maze generation algorithm implementations and maze object creators are here.
 */
#pragma once

#include "common.h"
#include "grid.h"

/**
 * @brief Generate a maze on a grid.
 *
 * @param grid grid to generate maze in
 * @param options options for the algorithm
 * @param type type of maze to generate
 */
void gen_maze(Grid *grid, MazeGenOptions *options, enum MazeType type);
