/**
 * @file maze.h
 * @brief All maze generation algorithm implementations and maze object creators are here.
 */
#pragma once

#include "common.h"
#include "grid.h"

/* Maze generator implementations */
#include "aldous_broder.h"
#include "binary_tree.h"
#include "ellers.h"
#include "hunt_and_kill.h"
#include "kruskals.h"
#include "recursive_backtracker.h"
#include "recursive_division.h"
#include "sidewinder.h"
#include "wilsons.h"

/**
 * @brief Generate a maze on a grid.
 */
void gen_maze(Grid *grid, MazeGenOptions *args, MazeType type);
