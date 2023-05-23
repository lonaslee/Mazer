#include "maze.h"

#include "common.h"

void gen_maze( MazeGenArg *args, enum MazeType type) {
    switch (type) {
        case ALDOUS_BRODER:         gen_aldous_broder(args);               break;
        case BINARY_TREE:           gen_binary_tree( args);           break;
        case ELLERS:                gen_ellers(args);                      break;
        case HUNT_AND_KILL:         gen_hunt_and_kill( args);         break;
        case SIDEWINDER:            gen_sidewinder( args);            break;
        case WILSONS:               gen_wilsons(args);                     break;
        case RECURSIVE_BACKTRACKER: gen_recursive_backtracker( args); break;
        case RECURSIVE_DIVISION:    gen_recursive_division( args);    break;
        case KRUSKALS:              gen_kruskals( args);              break;
        default:
            break;
    }
}
