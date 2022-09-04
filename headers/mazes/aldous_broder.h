#pragma once

#include "common.h"
#include "grid.h"

/**
 * @brief Generate a maze using the Aldous-Broder Algorithm.
 *
 * @param grid grid to generate maze in
 * @param options this algorithm has no customizeable options, this can be NULL
 * @return Grid* - same maze pointer passed
 */
Grid *gen_aldous_broder(Grid *grid, MazeGenOptions *options);