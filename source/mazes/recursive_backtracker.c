#include "recursive_backtracker.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "linkedlist.h"
#include "maze_common.h"

Grid *gen_recursive_backtracker(Grid *grid, MazeGenOptions *options) {
    grid->type = RECURSIVE_BACKTRACKER;
    int cx = rand() % grid->width;
    int cy = rand() % grid->height;
    Cell *cc = &grid->cells[cx][cy];
    cc->data = 1;
    LinkedList *stack = llnew(0, (void *)cc);

    int treesize = 1;
    while (treesize < grid->width * grid->height) {
        void *vstack[stack->len + 1];
        llvtolist(stack, vstack, -1, -1);
        vstack[stack->len] = NULL;
        draw_grid_step(grid, cc, (Cell **)vstack);

        enum DIRECTION dir = choicenz(4, UP * (cy != (grid->height - 1) && grid->cells[cx][cy + 1].data != 1),
                                      DOWN * (cy != 0 && grid->cells[cx][cy - 1].data != 1),
                                      LEFT * (cx != 0 && grid->cells[cx - 1][cy].data != 1),
                                      RIGHT * (cx != (grid->width - 1) && grid->cells[cx + 1][cy].data != 1));
        printf("dir: %d  |  (%d, %d)\n", dir, cx, cy);
        if (dir == STAY) {
            puts("SH");
            // TODO change cx & cy, or use cc to escape loop
            llshave(stack, NULL, NULL);
            cc = stack->last->vdata;
            continue;
        }
        treesize++;

        carve_path(grid, cx, cy, dir);
        cx += MOVEX(dir);
        cy += MOVEY(dir);
        cc = &grid->cells[cx][cy];
        cc->data = 1;
        llappend(stack, 0, (void *)cc);
    }
}
