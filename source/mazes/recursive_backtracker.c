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
    Stack *stack = sknew(10);
    skpush(stack, cc);

    int treesize = 1;
    while (1) {
        draw_grid_step(grid, cc, (Cell **)stack->elements);

        enum DIRECTION dir = choicenz(4, UP * (cc->upperwall->cell1 && cc->upperwall->cell1->data != 1),
                                      DOWN * (cc->lowerwall->cell2 && cc->lowerwall->cell2->data != 1),
                                      LEFT * (cc->left_wall->cell1 && cc->left_wall->cell1->data != 1),
                                      RIGHT * (cc->rightwall->cell2 && cc->rightwall->cell2->data != 1));
        if (dir == STAY) {
            skpop(stack, NULL);
            if (stack->len) {
                cc = stack->elements[stack->len - 1];
                continue;
            } else {
                break;
            }
        }
        treesize++;
        carve_path(cc, dir);
        cc = cell_at(cc, dir);
        cc->data = 1;
        skpush(stack, cc);
    }
}
