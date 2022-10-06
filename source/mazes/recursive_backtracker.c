#include "recursive_backtracker.h"

#include <stdlib.h>

#include "collections.h"
#include "common.h"
#include "grid.h"
#include "maze_common.h"

Grid *gen_recursive_backtracker(Grid *grid, MazeGenOptions *options) {
    grid->type = RECURSIVE_BACKTRACKER;
    Cell *cc = &grid->cells[rand() % grid->width][rand() % grid->height];
    cc->upperwall->exists = cc->lowerwall->exists = cc->left_wall->exists = cc->rightwall->exists = 1;
    cc->data = 1;
    LinkedList *stack = llnew(0, (void *)cc);

    int treesize = 1;
    while (1) {
        void *vstack[stack->len + 1];
        llvtolist(stack, vstack, -1, -1);
        vstack[stack->len] = NULL;
        draw_grid_step(grid, cc, (Cell **)vstack);

        enum DIRECTION dir = choicenz(4, UP * (cc->upperwall->cell1 && cc->upperwall->cell1->data != 1),
                                      DOWN * (cc->lowerwall->cell2 && cc->lowerwall->cell2->data != 1),
                                      LEFT * (cc->left_wall->cell1 && cc->left_wall->cell1->data != 1),
                                      RIGHT * (cc->rightwall->cell2 && cc->rightwall->cell2->data != 1));
        if (dir == STAY) {
            llshave(stack, NULL, NULL);
            if (stack->len) {
                cc = stack->last->vdata;
                continue;
            } else {
                break;
            }
        }
        treesize++;
        carve_path(cc, dir);
        cc = cell_at(cc, dir);
        cc->data = 1;
        llappend(stack, 0, (void *)cc);
    }
}
