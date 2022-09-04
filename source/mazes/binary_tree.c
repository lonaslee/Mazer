#include "binary_tree.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

Grid *gen_binary_tree(Grid *grid, MazeGenOptions *options) {
    grid->type = BINARY_TREE;
    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            grid_step(grid, &grid->cells[x][y], NULL);
            if (YESNO) {
                grid->cells[x][y].upperwall->exists = 0;
                grid->cells[x][y].left_wall->exists = 1;
            } else {
                grid->cells[x][y].left_wall->exists = 0;
                grid->cells[x][y].upperwall->exists = 1;
            }
            grid->cells[x][y].lowerwall->exists = 1;
            grid->cells[x][y].rightwall->exists = 1;
        }
    }
    for (int x = 0; x < grid->width; x++) {
        grid->cells[x][grid->height - 1].upperwall->exists = 1;
        grid->cells[x][grid->height - 1].left_wall->exists = 0;
    }
    for (int y = 0; y < grid->height; y++) {
        grid->cells[0][y].left_wall->exists = 1;
        grid->cells[0][y].upperwall->exists = 0;
    }
    grid->cells[0][grid->height - 1].upperwall->exists = 1;
    return grid;
}