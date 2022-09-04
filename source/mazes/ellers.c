#include "ellers.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

Grid *gen_ellers(Grid *grid, MazeGenOptions *options) {  // FIXME broken
    puts("ellers");
    grid->type = ELLERS;
    const int max_gs = options != NULL ? (options->opts[0] != 0 ? options->opts[0] : 5) : 5;
    const int randmod = options != NULL ? (options->opts[1] != 0 ? options->opts[1] : 3) : 3;
    int group_num = 1;  // the number of groups. increment every time a new group is made

    /* First row setup */ {
        int yh = grid->height - 1;
        for (int x = 0; x < grid->width; x++) {
            grid->cells[x][yh].data = group_num++;
            grid->cells[x][yh].upperwall->exists = 1;
        }
        grid->cells[0][yh].left_wall->exists = 1;
        grid->cells[grid->width - 1][yh].rightwall->exists = 1;

        int cur_gs = 1;
        Cell *cg[max_gs + 1];
        CLRARR(cg, max_gs + 1);
        cg[0] = &grid->cells[0][grid->height - 1];
        for (int x = 1; x < grid->width; x++) {
            if (!(rand() % randmod) || cur_gs == max_gs) { /* Split */
                grid->cells[x][yh].left_wall->exists = 1;
                // for (int i = 0; i < cur_gs; i++) {
                //     cg[i]->lowerwall->exists = (rand() % max_gs) != 0;
                // }
                cg[choicerange(0, cur_gs)]->lowerwall->exists = 0;
                grid_step(grid, &grid->cells[x][yh], cg);
                CLRARR(cg, max_gs + 1);
                cur_gs = 1;
                cg[0] = &grid->cells[x][yh];
            } else { /* Join */
                grid->cells[x][yh].data = grid->cells[x - 1][yh].data;
                cg[cur_gs++] = &grid->cells[x][yh];
                grid_step(grid, &grid->cells[x][yh], cg);
            }
        }
        for (int i = 0; i < cur_gs; i++) {
            cg[i]->lowerwall->exists = (rand() % max_gs) != 0;
        }
        cg[choicerange(0, cur_gs)]->lowerwall->exists = 0;
        grid_step(grid, &grid->cells[grid->width - 1][yh], cg);
    }

    for (int y = grid->height - 2; y >= 0; y--) {
        printf("y %d ========\n", y);
        for (int x = 0; x < grid->width; x++)
            grid->cells[x][y].data = grid->cells[x][y].upperwall->exists ? group_num++ : grid->cells[x][y + 1].data;

        int cur_gs = 1;
        Cell *cg[max_gs + 1];
        CLRARR(cg, max_gs + 1);
        cg[0] = &grid->cells[0][y];
        for (int x = 1; x < grid->width; x++) {
            printf("x %d -----\n", x);
            if (!grid->cells[x][y].upperwall->exists && !grid->cells[x - 1][y].upperwall->exists && grid->cells[x][y + 1].data == grid->cells[x - 1][y + 1].data) {
                puts("dup");
                grid->cells[x][y].left_wall->exists = 1;
            }

            if (!(rand() % randmod) || cur_gs == max_gs) { /* Split */
                printf("Split %d\n", cur_gs);
                grid->cells[x][y].left_wall->exists = 1;
                // for (int i = 0; i < cur_gs; i++) {
                //     printf("%d, ", i);
                //     cg[i]->lowerwall->exists = (rand() % max_gs) != 0;
                // }
                puts("");
                cg[choicerange(0, cur_gs)]->lowerwall->exists = 0;
                grid_step(grid, &grid->cells[x][y], cg);
                CLRARR(cg, max_gs + 1);
                cur_gs = 1;
                cg[0] = &grid->cells[x][y];
            } else { /* Join */
                printf("Join %d\n", cur_gs);
                grid->cells[x][y].data = grid->cells[x - 1][y].data;
                cg[cur_gs++] = &grid->cells[x][y];
                grid_step(grid, &grid->cells[x][y], cg);
            }
        }
        // for (int i = 0; i < cur_gs; i++) {
        //     printf("%d, ", i);
        //     cg[i]->lowerwall->exists = (rand() % max_gs) != 0;
        // }
        puts("");
        cg[choicerange(0, cur_gs)]->lowerwall->exists = 0;
        grid_step(grid, &grid->cells[grid->width - 1][y], cg);
    }

    for (int x = 1; x < grid->width; x++) {
        if (grid->cells[x][0].data != grid->cells[x - 1][0].data) {
            grid->cells[x][0].left_wall->exists = 0;
            grid->cells[x][0].data = grid->cells[x - 1][0].data;
        }
    }

    print_grid(grid);
}