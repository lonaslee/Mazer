#pragma once

#include "common.h"
#include "grid.h"

/**
 * @brief Generate a maze using the Sidewinder Algorithm.
 *
 * @param grid grid to generate maze in
 * @param options this algorithm has 2 options: max group size and rand mod number
 * @return Grid* - same maze pointer passed
 */
Grid *gen_sidewinder(Grid *grid, MazeGenOptions *options);