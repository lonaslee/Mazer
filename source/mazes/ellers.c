#include "ellers.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

Grid *gen_ellers(Grid *grid, MazeGenOptions *options) {
    grid->type = ELLERS;
    const int setsize_max = options != NULL ? (options->opts[0] != 0 ? options->opts[0] : INT_MAX) : INT_MAX;
    const int rrandmod = options != NULL ? (options->opts[1] != 0 ? options->opts[1] : 3) : 3;
    const int drandmod = options != NULL ? (options->opts[2] != 0 ? options->opts[2] : 5) : 5;
    for (int x = 0; x < grid->width; x++)
        for (int y = 0; y < grid->height; y++)
            grid->cells[x][y].upperwall->exists = grid->cells[x][y].lowerwall->exists = grid->cells[x][y].left_wall->exists = grid->cells[x][y].rightwall->exists = 1;

    int setnum = 1;
    for (int y = grid->height - 1; y >= 0; y--) {
        // flesh out row
        for (int x = 0; x < grid->width; x++)
            if (!grid->cells[x][y].data) {
                grid->cells[x][y].data = setnum++;
            }
        // join adjacents
        int setsize = 1;
        for (int x = 0; x < grid->width - 1; x++) {
            draw_grid_step(grid, &grid->cells[x][y], (Cell *[]){&grid->cells[x + 1][y], NULL});
            if (grid->cells[x][y].data == grid->cells[x + 1][y].data || !(rand() % rrandmod) || setsize >= setsize_max) { /* SEP */
                setsize = 1;
            } else { /* JOIN */
                grid->cells[x][y].rightwall->exists = 0;
                int mergenum = grid->cells[x + 1][y].data;
                for (int x2 = 0; x2 < grid->width; x2++) {
                    if (grid->cells[x2][y].data == mergenum) {
                        grid->cells[x2][y].data = grid->cells[x][y].data;
                        setsize++;
                    }
                }
            }
        }

        // make down ways
        setsize = 0;
        Cell *cellset[grid->width + 1];
        CLRARR(cellset, grid->width + 1)
        for (int x = 0; x < grid->width; x++) {
            int cursetnum = grid->cells[x][y].data;
            do {
                cellset[setsize++] = &grid->cells[x++][y];
                draw_grid_step(grid, &grid->cells[x][y], cellset);
            } while (x < grid->width && grid->cells[x][y].data == cursetnum);
            x--;

            for (int i = 0; i < setsize; i++) {
                if (!(rand() % drandmod)) {
                    cellset[i]->lowerwall->exists = 0;
                    if (y != 0) cellset[i]->lowerwall->cell2->data = cellset[i]->data;
                }
            }
            int idx = choicerange(0, setsize);
            cellset[idx]->lowerwall->exists = 0;
            if (y != 0) cellset[idx]->lowerwall->cell2->data = cellset[idx]->data;
            CLRARR(cellset, grid->width + 1)
            setsize = 0;
        }
    }

    for (int x = 0; x < grid->width - 1; x++) {
        grid->cells[x][0].lowerwall->exists = 1;
        if (grid->cells[x][0].data != grid->cells[x + 1][0].data) {
            grid->cells[x][0].rightwall->exists = 0;
            int mergenum = grid->cells[x + 1][0].data;
            for (int x2 = 0; x2 < grid->width; x2++) {
                if (grid->cells[x2][0].data == mergenum) {
                    grid->cells[x2][0].data = grid->cells[x][0].data;
                }
            }
        }
        draw_grid_step(grid, &grid->cells[x][0], NULL);
    }
    grid->cells[grid->width - 1][0].lowerwall->exists = 1;
}
