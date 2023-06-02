#include "screen.h"

#include "common.h"
#include "math.h"

void draw_graph(Graph *g) {
    int ww, wh;
    SDL_GetWindowSize(game->win, &ww, &wh);

    int s = round(SM * MIN((ww * 0.95) / g->nc, (wh * 0.95) / g->nr));
    int w = MAX(1, round(WM * s));

    int ox = round(HALF(ww - s * g->nc));
    int oy = round(HALF(wh - s * g->nr));

    SDL_RenderCopy(game->renderer, game->resources->textures[BG_GREEN], NULL, NULL);

    SDL_Texture *generic_cell = game->resources->textures[CLR_LYELLOW];
    SDL_Texture *primary_cell = game->resources->textures[CLR_DBLUE];
    SDL_Texture *secondary_cell = game->resources->textures[CLR_LORANGE];
    SDL_Texture *wall = game->resources->textures[CLR_BLACK];

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
