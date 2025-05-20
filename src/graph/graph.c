#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "math.h"

Graph *new_graph(int nr, int nc) {
    Graph *g = calloc(1, sizeof(Graph));
    g->nr = nr;
    g->nc = nc;
    g->nodes = calloc(nr, sizeof(Node ***));
    for (int y = 0; y < nr; y++) {
        g->nodes[y] = calloc(nc, sizeof(Node **));
        for (int x = 0; x < nc; x++) {
            Node *n = calloc(1, sizeof(Node));
            g->nodes[y][x] = n;
            n->x = x;
            n->y = y;
        }
    }

    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = g->nodes[y][x];
            if (y != 0) {
                n->nny = g->nodes[y - 1][x];
                n->nny->npy = n;
                n->wny = n->nny->wpy = calloc(1, sizeof(bool));
            }
            if (x != 0) {
                n->nnx = g->nodes[y][x - 1];
                n->nnx->npx = n;
                n->wnx = n->nnx->wpx = calloc(1, sizeof(bool));
            }
        }
    }
    return g;
}

void del_graph(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = g->nodes[y][x];
            if (y != 0) free(n->wny);
            if (x != 0) free(n->wnx);
        }
        for (int x = 0; x < g->nc; x++) {
            free(g->nodes[y][x]);
        }
        free(g->nodes[y]);
    }
    free(g->nodes);
    free(g);
}

void draw_graph(Graph *g) {
    int ww, wh;
    SDL_GetWindowSize(game->win, &ww, &wh);

    int s = round(SM * MIN((ww * 0.95) / g->nc, (wh * 0.95) / g->nr));
    int w = MAX(1, round(WM * s));

    int ox = round(HALF(ww - s * g->nc));
    int oy = round(HALF(wh - s * g->nr));

    SDL_RenderCopy(game->renderer, get_texture(BG_GREEN), NULL, NULL);

    SDL_Texture *generic_cell = get_texture(CLR_LYELLOW);
    SDL_Texture *primary_cell = get_texture(CLR_DBLUE);
    SDL_Texture *secondary_cell = get_texture(CLR_LORANGE);
    SDL_Texture *wall = get_texture(CLR_BLACK);

    SDL_Rect ss = {.x = ox, .y = oy, .w = s * g->nc, .h = s * g->nr};
    SDL_RenderCopy(game->renderer, generic_cell, NULL, &ss);

    int bw = w * BM;

    SDL_Rect hb = {.x = ox, .y = oy - bw, .w = s * g->nc, .h = bw};
    SDL_Rect vb = {.x = ox - bw, .y = oy, .w = bw, .h = s * g->nr};
    SDL_RenderCopy(game->renderer, wall, NULL, &hb);
    SDL_RenderCopy(game->renderer, wall, NULL, &vb);
    hb.y += s * g->nr + bw;
    vb.x += s * g->nc + bw;
    SDL_RenderCopy(game->renderer, wall, NULL, &hb);
    SDL_RenderCopy(game->renderer, wall, NULL, &vb);

    SDL_Rect sr = {.w = s, .h = s};
    SDL_Rect hr = {.w = s, .h = w};
    SDL_Rect vr = {.w = w, .h = s};

    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            sr.x = ox + (x * s);
            sr.y = oy + (y * s);

            if (llisflipped(n->data, PRIMARY_CELL)) {
                SDL_RenderCopy(game->renderer, primary_cell, NULL, &sr);
            } else if (llisflipped(n->data, SECONDARY_CELL)) {
                SDL_RenderCopy(game->renderer, secondary_cell, NULL, &sr);
            }

            if (y != 0 && *(n->wny)) {
                hr.x = sr.x;
                hr.y = sr.y;
                SDL_RenderCopy(game->renderer, wall, NULL, &hr);
            }

            if (x != 0 && *(n->wnx)) {
                vr.x = sr.x - HALF(w);
                vr.y = sr.y;
                SDL_RenderCopy(game->renderer, wall, NULL, &vr);
            }
        }
    }
}

void dir2str(AxisDirection dir, char *buf) {
    memcpy(buf, ((dir) == NEG_Y) ? "NEG_Y" : ((dir) == POS_Y) ? "POS_Y"
                                         : ((dir) == NEG_X)   ? "NEG_X"
                                         : ((dir) == POS_X)   ? "POS_X"
                                                              : "NONE ",
           6);
}

Graph *connect_all(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            if (y != 0) *(n->wny) = true;
            if (x != 0) *(n->wnx) = true;
        }
    }
    return g;
}

Graph *disconnect_all(Graph *g) {
    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            if (y != 0) *(n->wny) = false;
            if (x != 0) *(n->wnx) = false;
        }
    }
    return g;
}

void connect_nodes(Node *a, Node *b) {
    if (a->nny == b) {
        *(a->wny) = false;
    } else if (a->nnx == b) {
        *(a->wnx) = false;
    } else if (a->npy == b) {
        *(a->wpy) = false;
    } else if (a->npx == b) {
        *(a->wpx) = false;
    } else {
        printf("Not able to connect nodes (%d, %d) & (%d, %d).", a->x, a->y, b->x, b->y);
        // abort();
    }
}

void surround(Node *n) {
    ifnn(n->wny) * (n->wny) = true;
    ifnn(n->wnx) * (n->wnx) = true;
    ifnn(n->wpy) * (n->wpy) = true;
    ifnn(n->wpx) * (n->wpx) = true;
}

void extrude_to(Node *n, AxisDirection dir) {
    ifnn(n->wny) { *(n->wny) = dir != POS_Y; }
    ifnn(n->wnx) { *(n->wnx) = dir != POS_X; }
    ifnn(n->wpy) { *(n->wpy) = dir != NEG_Y; }
    ifnn(n->wpx) { *(n->wpx) = dir != NEG_X; }
}
