#include "recursive_division.h"

#include "maze_common.h"

static void divide(Grid *grid, int ox, int oy, int width, int height);

void *gen_recursive_division(void *args) {
    MazeGenArg *arg = args;
    Grid *grid = arg->grid;
    grid->type = RECURSIVE_DIVISION;

    for (int x = 0; x < grid->width; x++)
        grid->cells[x][0].lowerwall->exists = grid->cells[x][grid->height - 1].upperwall->exists = 1;
    for (int y = 0; y < grid->height; y++)
        grid->cells[0][y].left_wall->exists = grid->cells[grid->width - 1][y].rightwall->exists = 1;

    divide(grid, 0, 0, grid->width, grid->height);
    return grid;
}

static void divide(Grid *grid, int ox, int oy, int width, int height) {
    Cell *cs[width * height + 1];
    int idx = 0;
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) cs[idx++] = &grid->cells[ox + x][oy + y];
    cs[idx] = NULL;
    if (width < 2 || height < 2) {
        draw_grid_step(grid, NULL, cs);
        return;
    }
    draw_grid_step(grid, &grid->cells[ox][oy], cs);

    if (width > height || (width == height && rand() % 2)) {
        int x = width / 2;
        for (int y = 0; y < height; y++) grid->cells[x + ox][y + oy].left_wall->exists = 1;
        grid->cells[x + ox][rand() % height + oy].left_wall->exists = 0;

        divide(grid, ox, oy, x, height);
        divide(grid, ox + x, oy, width - x, height);
    } else {
        int y = height / 2;
        for (int x = 0; x < width; x++) grid->cells[x + ox][y + oy].lowerwall->exists = 1;
        grid->cells[rand() % width + ox][y + oy].lowerwall->exists = 0;
        divide(grid, ox, oy, width, y);
        divide(grid, ox, oy + y, width, height - y);
    }
}
