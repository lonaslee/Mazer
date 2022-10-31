#include "kruskals.h"

#include <stdlib.h>

#include "collections.h"
#include "common.h"
#include "grid.h"
#include "limits.h"
#include "maze_common.h"

static int cmp(const void *a, const void *b) {
    return rand() % 2;
}

Grid *gen_kruskals(Grid *grid, MazeGenOptions *options) {
    grid->type = KRUSKALS;
    Stack *walls = sknew(grid->width * grid->height * 2 + grid->width + grid->height);
    TNode *sets[grid->width * grid->height];
    int idx = 0;
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            sets[idx] = trnew((void *)&grid->cells[x][y]);
            grid->cells[x][y].data = idx;
            idx++;

            if (y != grid->height - 1)
                skpush(walls, grid->cells[x][y].upperwall);
            else
                grid->cells[x][y].upperwall->exists = 1;
            if (x != grid->width - 1)
                skpush(walls, grid->cells[x][y].rightwall);
            else
                grid->cells[x][y].rightwall->exists = 1;
        }
    }
    for (int x = 0; x < grid->width; x++) grid->cells[x][0].lowerwall->exists = 1;
    for (int y = 0; y < grid->height; y++) grid->cells[0][y].left_wall->exists = 1;
    for (int i = 0; i < walls->len; i++) ((Wall *)walls->elements[i])->exists = 1;
    qsort(walls->elements, walls->len, sizeof(Wall *), cmp);

    while (walls->len) {
        Wall *wall;
        skpop(walls, (void **)&wall);
        draw_grid_step(grid, NULL, (Cell *[]){wall->cell1, wall->cell2, NULL});

        if (trgetroot(sets[wall->cell1->data]) != trgetroot(sets[wall->cell2->data])) {
            trappendchildtree(trgetroot(sets[wall->cell1->data]), trgetroot(sets[wall->cell2->data]));
            wall->exists = 0;
        }
    }
}
