#pragma once

#include "common.h"
#include "grid.h"

/**
 * @brief Generate a maze using Kruskal's Algorithm.
 *
 * @param grid grid to generate maze in
 * @param options this algorithm has no options, can be NULL
 * @return Grid* - same maze pointer passed
 */
Grid *gen_kruskals(Grid *grid, MazeGenOptions *options);
