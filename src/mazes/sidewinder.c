#include "sidewinder.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

Grid *gen_sidewinder(Grid *grid, MazeGenOptions *options) {
    grid->type = SIDEWINDER;
    const int group_size_max = options != NULL ? (options->opts[0] != 0 ? options->opts[0] : 5) : 5;
    const int randmod = options != NULL ? (options->opts[1] != 0 ? options->opts[1] : 3) : 3;
    int group = 0;
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            grid->cells[x][y].upperwall->exists = 1;
            grid->cells[x][y].lowerwall->exists = 1;
            grid->cells[x][y].left_wall->exists = 1;
            grid->cells[x][y].rightwall->exists = 1;
        }
    }

    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            Cell *cell_group[group_size_max];  // FIXME this should be one more for the NULL terminator but it works so I'm scared to touch it
            int g_size;
            for (g_size = 1; g_size <= group; g_size++) {
                cell_group[g_size - 1] = &grid->cells[x - g_size][y];
            }
            cell_group[g_size - 1] = NULL;
            draw_grid_step(grid, &grid->cells[x][y], cell_group);
            if (!(rand() % randmod) || group == group_size_max || x == grid->width - 1) {
                if (group == 0)
                    grid->cells[x][y].upperwall->exists = 0;
                else
                    grid->cells[x - choicerange(0, group)][y].upperwall->exists = 0;
                group = 0;
            } else {
                group++;
                grid->cells[x][y].rightwall->exists = 0;
            }
        }
    }

    for (int x = 0; x < grid->width; x++) {
        grid->cells[x][grid->height - 1].upperwall->exists = 1;
        grid->cells[x][grid->height - 1].rightwall->exists = 0;
    }
    grid->cells[grid->width - 1][grid->height - 1].rightwall->exists = 1;
    return grid;
}
