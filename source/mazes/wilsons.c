#include "wilsons.h"

#include <stdlib.h>

#include "common.h"
#include "grid.h"
#include "maze_common.h"

Grid *gen_wilsons(Grid *grid, MazeGenOptions *options) {
    grid->type = WILSONS;
    int x1 = rand() % grid->width;
    int y1 = rand() % grid->height;
    grid->cells[x1][y1].data = 5;
    grid->cells[x1][y1].upperwall->exists = grid->cells[x1][y1].lowerwall->exists = grid->cells[x1][y1].left_wall->exists = grid->cells[x1][y1].rightwall->exists = 1;

    int treesize = 1;
    int ix = 0;
    while (treesize < grid->width * grid->height) {
        int ox;
        int oy;
        do {
            ox = rand() % grid->width;
            oy = rand() % grid->height;
        } while (grid->cells[ox][oy].data != 0);

        int cx = ox, cy = oy;
        List *path = list_new(10);
        draw_grid_step(grid, &grid->cells[ox][oy], (Cell **)path->elements);
        while (grid->cells[cx][cy].data != 5) {
            list_append_unique(path, &grid->cells[cx][cy]);
            enum DIRECTION dir = choicenz(4, UP * (cy != (grid->height - 1)),
                                          DOWN * (cy != 0),
                                          LEFT * (cx != 0),
                                          RIGHT * (cx != (grid->width - 1)));
            grid->cells[cx][cy].data = dir;
            cx += MOVEX(dir);
            cy += MOVEY(dir);
            if (grid->cells[cx][cy].data != 0 && grid->cells[cx][cy].data != 5) {
                Cell *ocell = &grid->cells[cx][cy];
                for (; path->elements[path->idx - 1] != ocell; path->idx--) {
                    ((Cell *)path->elements[path->idx - 1])->data = 0;
                    path->elements[path->idx - 1] = NULL;
                }
            }
            draw_grid_step(grid, &grid->cells[ox][oy], (Cell **)path->elements);
        }

        Cell *c = path->elements[0];
        enum DIRECTION lastdir = c->data;
        c->upperwall->exists = c->lowerwall->exists = c->left_wall->exists = c->rightwall->exists = 1;
        c->data = 5;
        treesize += path->idx;
        for (int i = 1; i < path->idx; i++) {
            draw_grid_step(grid, (Cell *)path->elements[i], (Cell **)path->elements);
            Cell *c = path->elements[i];
            c->upperwall->exists = lastdir != DOWN;
            c->lowerwall->exists = lastdir != UP;
            c->left_wall->exists = lastdir != RIGHT;
            c->rightwall->exists = lastdir != LEFT;
            lastdir = c->data;
            c->data = 5;
        }
        switch (lastdir) {
            case UP:
                grid->cells[cx][cy].lowerwall->exists = 0;
                break;
            case DOWN:
                grid->cells[cx][cy].upperwall->exists = 0;
                break;
            case LEFT:
                grid->cells[cx][cy].rightwall->exists = 0;
                break;
            case RIGHT:
                grid->cells[cx][cy].left_wall->exists = 0;
                break;
        }
        draw_grid_step(grid, (Cell *)path->elements[path->idx - 1], (Cell **)path->elements);
    }
    return grid;
}
