#include "grid.h"

#include <stdio.h>
#include <stdlib.h>

Grid *generate_grid(int width, int height) {
    Cell **grid = calloc(width, sizeof(Cell *));
    for (int i = 0; i < width; i++) {
        Cell *column = calloc(height, sizeof(Cell));
        grid[i] = column;
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            grid[x][y].data = 0;
            // printf("\n(%d, %d)\n", x, y);

            if (grid[x][y].upperwall == NULL) {
                if (y != height - 1) {
                    if (grid[x][y + 1].lowerwall == NULL) {
                        Wall *wall = calloc(1, sizeof(Wall));
                        grid[x][y].upperwall = wall;
                        grid[x][y + 1].lowerwall = wall;
                        wall->cell1 = &grid[x][y + 1];
                        wall->cell2 = &grid[x][y];
                    } else {
                        grid[x][y].upperwall = grid[x][y + 1].lowerwall;
                    }
                } else {
                    Wall *wall = calloc(1, sizeof(Wall));
                    grid[x][y].upperwall = wall;
                    wall->cell2 = &grid[x][y];
                }
                // printf("upper\n");
            }
            if (grid[x][y].lowerwall == NULL) {
                if (y != 0) {
                    if (grid[x][y - 1].upperwall == NULL) {
                        Wall *wall = calloc(1, sizeof(Wall));
                        grid[x][y].lowerwall = wall;
                        grid[x][y - 1].upperwall = wall;
                        wall->cell1 = &grid[x][y];
                        wall->cell2 = &grid[x][y - 1];
                    } else {
                        grid[x][y].lowerwall = grid[x][y - 1].upperwall;
                    }
                } else {
                    Wall *wall = calloc(1, sizeof(Wall));
                    grid[x][y].lowerwall = wall;
                    wall->cell1 = &grid[x][y];
                }
                // printf("lower\n");
            }
            if (grid[x][y].left_wall == NULL) {
                if (x != 0) {
                    if (grid[x - 1][y].rightwall == NULL) {
                        Wall *wall = calloc(1, sizeof(Wall));
                        grid[x][y].left_wall = wall;
                        grid[x - 1][y].rightwall = wall;
                        wall->cell1 = &grid[x - 1][y];
                        wall->cell2 = &grid[x][y];
                    } else {
                        grid[x][y].left_wall = grid[x - 1][y].rightwall;
                    }
                } else {
                    Wall *wall = calloc(1, sizeof(Wall));
                    grid[x][y].left_wall = wall;
                    wall->cell2 = &grid[x][y];
                }
                // printf("left\n");
            }
            if (grid[x][y].rightwall == NULL) {
                if (x != width - 1) {
                    if (grid[x + 1][y].left_wall == NULL) {
                        Wall *wall = calloc(1, sizeof(Wall));
                        grid[x][y].rightwall = wall;
                        grid[x + 1][y].left_wall = wall;
                        wall->cell1 = &grid[x][y];
                        wall->cell2 = &grid[x + 1][y];
                    } else {
                        grid[x][y].rightwall = grid[x + 1][y].left_wall;
                    }
                } else {
                    Wall *wall = calloc(1, sizeof(Wall));
                    grid[x][y].rightwall = wall;
                    wall->cell1 = &grid[x][y];
                }
                // printf("right\n");
            }
        }
    }

    Grid *gridobj = calloc(1, sizeof(Grid));
    gridobj->width = width;
    gridobj->height = height;
    gridobj->cells = grid;
    return gridobj;
}

int free_grid(Grid *grid) {
    if (grid == NULL) return 0;
    int num = 0;
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            if (grid->cells[x][y].upperwall != NULL) {
                num++;
                free(grid->cells[x][y].upperwall);
                grid->cells[x][y].upperwall = NULL;
                if (y != grid->height - 1) grid->cells[x][y + 1].lowerwall = NULL;
            }
            if (grid->cells[x][y].lowerwall != NULL) {
                num++;
                free(grid->cells[x][y].lowerwall);
                grid->cells[x][y].lowerwall = NULL;
                if (y != 0) grid->cells[x][y - 1].upperwall = NULL;
            }
            if (grid->cells[x][y].left_wall != NULL) {
                num++;
                free(grid->cells[x][y].left_wall);
                grid->cells[x][y].left_wall = NULL;
                if (x != 0) grid->cells[x - 1][y].rightwall = NULL;
            }
            if (grid->cells[x][y].rightwall != NULL) {
                num++;
                free(grid->cells[x][y].rightwall);
                grid->cells[x][y].rightwall = NULL;
                if (x != grid->width - 1) grid->cells[x + 1][y].left_wall = NULL;
            }
        }
    }
    free(grid);
    return num;
}

void print_grid(Grid *grid) {
    Wall *last;
    puts("");
    for (int x = 0; x < grid->width; x++)
        printf(" %s", grid->cells[x][0].upperwall != NULL ? "-" : "N");
    puts("");

    for (int y = 1; y < grid->height; y++) {
        printf("%s %s ", grid->cells[0][y].left_wall != NULL ? "|" : "N", grid->cells[0][y].rightwall != NULL ? "|" : "N");
        for (int x = 1; x < grid->width; x++) {
            printf("%s ", grid->cells[x][y].left_wall == grid->cells[x - 1][y].rightwall ? "|" : "N");
        }
        puts("");

        if (y < grid->height - 1) {
            for (int x = 0; x < grid->width; x++) {
                printf(" %s", grid->cells[x][y].lowerwall == grid->cells[x][y - 1].upperwall ? "-" : "N");
            }
            puts("");
        }
    }

    for (int x = 0; x < grid->width; x++)
        printf(" %s", grid->cells[x][0].lowerwall != NULL ? "-" : "N");
    puts("");
}
