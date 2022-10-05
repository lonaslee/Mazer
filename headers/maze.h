/**
 * @file maze.h
 * @brief All maze generation algorithm implementations and maze object creators are here.
 */
#pragma once

#include "grid.h"

/* Maze generator implementations */
#include "aldous_broder.h"
#include "binary_tree.h"
#include "ellers.h"
#include "hunt_and_kill.h"
#include "recursive_backtracker.h"
#include "sidewinder.h"
#include "wilsons.h"

/**
 * @brief Generate a maze on a grid.
 *
 * @param grid grid to generate maze in
 * @param options options for the algorithm
 * @param type type of maze to generate
 */
void gen_maze(Grid *grid, MazeGenOptions *options, enum MazeType type);
