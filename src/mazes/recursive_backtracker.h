#pragma once

#include "common.h"
#include "grid.h"

/**
 * @brief Generate a maze using the recursive backtracker algorithm.
 *
 * @param grid grid to generate maze in
 * @param options this algorithm has no customizeable options, this can be NULL
 * @return Grid* - same maze pointer passed
 */
Grid *gen_recursive_backtracker(Grid *grid, MazeGenOptions *options);
