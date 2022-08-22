#include "maze.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graph.h"
#include "grid.h"
#include "images.h"

Grid *gen_aldous_broder(Grid *grid, MazeGenOptions *options) {
    puts("aldous broder");
    grid->type = ALDOUS_BRODER;
    int curx = rand() % grid->width;
    int cury = rand() % grid->height;
    int visited = 0;
    Cell *this_cell = &grid->cells[curx][cury];
    this_cell->upperwall->exists = 1;
    this_cell->lowerwall->exists = 1;
    this_cell->left_wall->exists = 1;
    this_cell->rightwall->exists = 1;
    this_cell->data = 1;

    while (visited < grid->width * grid->height - 1) {
        grid_step(grid, &grid->cells[curx][cury], NULL, ALDOUS_BRODER);
        enum DIRECTION dir = choicenz(4, UP * (cury != (grid->height - 1)),
                                      DOWN * (cury != 0),
                                      LEFT * (curx != 0),
                                      RIGHT * (curx != (grid->width - 1)));
        curx += MOVEX(dir);
        cury += MOVEY(dir);
        this_cell = &grid->cells[curx][cury];
        if (this_cell->data == 1) continue;
        this_cell->data = 1;
        visited++;
        this_cell->upperwall->exists = dir != DOWN;
        this_cell->lowerwall->exists = dir != UP;
        this_cell->left_wall->exists = dir != RIGHT;
        this_cell->rightwall->exists = dir != LEFT;
    }
    return grid;
}

Grid *gen_wilsons(Grid *grid) {
}

Grid *gen_binary_tree(Grid *grid, MazeGenOptions *options) {
    puts("binary tree");
    grid->type = BINARY_TREE;
    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            grid_step(grid, &grid->cells[x][y], NULL, BINARY_TREE);
            if (YESNO) {
                grid->cells[x][y].upperwall->exists = 0;
                grid->cells[x][y].left_wall->exists = 1;
            } else {
                grid->cells[x][y].left_wall->exists = 0;
                grid->cells[x][y].upperwall->exists = 1;
            }
            grid->cells[x][y].lowerwall->exists = 1;
            grid->cells[x][y].rightwall->exists = 1;
        }
    }
    for (int x = 0; x < grid->width; x++) {
        grid->cells[x][grid->height - 1].upperwall->exists = 1;
        grid->cells[x][grid->height - 1].left_wall->exists = 0;
    }
    for (int y = 0; y < grid->height; y++) {
        grid->cells[0][y].left_wall->exists = 1;
        grid->cells[0][y].upperwall->exists = 0;
    }
    grid->cells[0][grid->height - 1].upperwall->exists = 1;
    return grid;
}

Grid *gen_sidewinder(Grid *grid, MazeGenOptions *options) {
    puts("sidewinder");
    grid->type = SIDEWINDER;
    const int group_size_max = options != NULL ? (options->opts[0] != 0 ? options->opts[0] : 5) : 5;
    int randdiv = group_size_max - (group_size_max == 5 ? 2 : 0);
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
            Cell *cell_group[group_size_max];
            int g_size;
            for (g_size = 1; g_size <= group; g_size++) {
                cell_group[g_size - 1] = &grid->cells[x - g_size][y];
            }
            cell_group[g_size - 1] = NULL;
            grid_step(grid, &grid->cells[x][y], cell_group, SIDEWINDER);
            if (!(rand() % randdiv) || group == group_size_max || x == grid->width - 1) {
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
}

Grid *gen_ellers(Grid *grid, MazeGenOptions *options) {  // FIXME broken
    int group_size_max = options != NULL ? (options->opts[0] != 0 ? options->opts[0] : 5) : 5;
    int randdiv = options != NULL ? (options->opts[1] != 0 ? options->opts[1] : 2) : 2;
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            grid->cells[x][y].upperwall->exists = 1;
            grid->cells[x][y].lowerwall->exists = 1;
            grid->cells[x][y].left_wall->exists = 1;
            grid->cells[x][y].rightwall->exists = 1;
        }
    }

    int groupnum = 1;
    for (int y = grid->height - 1; y > 0; y--) {
        /* First iteration to get rid of cells not in a set */ {
            for (int x = 0; x < grid->width; x++) {
                if (grid->cells[x][y].data == 0)
                    grid->cells[x][y].data = groupnum++;
                // grid_step(grid, &grid->cells[x][y], NULL, ELLERS);
            }
        }
        puts("second");
        List *groups = list_new(10);
        List *cur_group = groups->new_list(group_size_max);
        /* Second iteration to join cells into sets randomly */ {
            int cur_group_size = 0;
            for (int x = 0; x < grid->width; x++) {
                Cell *cptr_group[group_size_max];
                for (int i = 0; i < cur_group->idx; i++)
                    cptr_group[i] = (Cell *)cur_group->elements[i];
                grid_step(grid, &grid->cells[x][y], cptr_group, ELLERS);
                cur_group_size++;
                cur_group->append(cur_group, &grid->cells[x][y]);
                if (!(rand() % randdiv) || cur_group_size == group_size_max || x == grid->width - 1 || grid->cells[x][y].data == grid->cells[x + 1][y].data) {
                    groups->append(groups, cur_group);
                    cur_group = groups->new_list(group_size_max);
                    grid->cells[x][y].rightwall->exists = 1;
                    cur_group_size = 0;
                } else {
                    grid->cells[x][y].rightwall->exists = 0;
                    grid->cells[x + 1][y].data = grid->cells[x][y].data;
                }
            }
        }

        /* Third iteration to create downward passages */ {
            for (int i = 0; i < groups->idx; i++) {
                List *group = (List *)groups->elements[i];
                Cell *cptr_group[group_size_max];
                for (int i = 0; i < group->idx; i++)
                    cptr_group[i] = (Cell *)group->elements[i];
                grid_step(grid, (Cell *)&group->elements[0], cptr_group, ELLERS);
                ((Cell *)group->elements[rand() % group->idx])->lowerwall->exists = 0;
                for (int x = 0; x < group->idx; x++) {
                    if (!(rand() % group_size_max)) ((Cell *)group->elements[x])->lowerwall->exists = 0;
                }
                group->del(group);
            }
        }
    }
    puts("here");
    for (int x = 0; x < grid->width; x++) {
        if (grid->cells[x][0].data == 0)
            grid->cells[x][0].data = groupnum++;
        grid->cells[x][0].lowerwall->exists = 1;
    }
    puts("here");
    for (int x = 0; x < grid->width - 1; x++) {
        if (grid->cells[x][0].data != grid->cells[x + 1][0].data) {
            grid->cells[x][0].rightwall->exists = 0;
            grid->cells[x + 1][0].data = grid->cells[x][0].data;
            x++;
        }
    }

    puts("");
    for (int y = grid->height - 1; y >= 0; y--) {
        for (int x = 0; x < grid->width; x++) {
            printf("%3d ", grid->cells[x][y].data);
        }
        puts("");
    }
}

void grid_step(Grid *grid, Cell *this_cell, Cell **other_cells, enum MazeType type) {
    Game *game = get_game();
    if (game->settings->step_interval == 0) return;
    clock_t start = clock();
    SDL_Event event;
    while ((double)(clock() - start) / CLOCKS_PER_SEC < game->settings->step_interval) {
        while (SDL_PollEvent(&event)) {
            on_event(&event);
        }
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        draw_grid(get_grid_resources(), game->renderer, grid, this_cell, other_cells);
        SDL_RenderPresent(game->renderer);
    }
}
