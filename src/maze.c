#include "maze.h"

#include "common.h"

void gen_maze(Grid *grid, MazeGenOptions *args, MazeType type) {
    switch (type) {
        case ALDOUS_BRODER:
            gen_aldous_broder(grid, args);
            break;
        case BINARY_TREE:
            gen_binary_tree(grid, args);
            break;
        case ELLERS:
            gen_ellers(grid, args);
            break;
        case HUNT_AND_KILL:
            gen_hunt_and_kill(grid, args);
            break;
        case SIDEWINDER:
            gen_sidewinder(grid, args);
            break;
        case WILSONS:
            gen_wilsons(grid, args);
            break;
        case RECURSIVE_BACKTRACKER:
            gen_recursive_backtracker(grid, args);
            break;
        case RECURSIVE_DIVISION:
            gen_recursive_division(grid, args);
            break;
        case KRUSKALS:
            gen_kruskals(grid, args);
            break;
        default:
            break;
    }
}
