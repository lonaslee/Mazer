#include "screen.h"

#include "common.h"
#include "math.h"

void draw_graph(Graph *g) {
    int ww, wh;
    SDL_GetWindowSize(game->win, &ww, &wh);

    int s = round(MIN((ww * 0.95) / g->nc, (wh * 0.95) / g->nr));
    int w = s * WM;
    s *= SM;

    int ox = HALF(ww - (s + w) * g->nc) + HALF(w);
    int oy = HALF(wh - (s + w) * g->nr) + HALF(w);

    SDL_RenderCopy(game->renderer, game->resources->textures[BG_GREEN], NULL, NULL);

    SDL_Texture *cell_img = game->resources->textures[CLR_LYELLOW];
    SDL_Texture *cell_img2 = game->resources->textures[CLR_DBLUE];
    SDL_Texture *cell_img3 = game->resources->textures[CLR_LORANGE];
    SDL_Texture *wall_img = game->resources->textures[CLR_BLACK];

    SDL_Rect ss = {.x = ox, .y = oy, .w = s * g->nc, .h = s * g->nr};
    SDL_RenderCopy(game->renderer, cell_img, NULL, &ss);

    int bw = w * BM;

    SDL_Rect hb = {.x = ox, .y = oy - bw, .w = s * g->nc, .h = bw};
    SDL_Rect vb = {.x = ox - bw, .y = oy, .w = bw, .h = s * g->nr};
    SDL_RenderCopy(game->renderer, wall_img, NULL, &hb);
    SDL_RenderCopy(game->renderer, wall_img, NULL, &vb);
    hb.y += s * g->nr + bw;
    vb.x += s * g->nc + bw;
    SDL_RenderCopy(game->renderer, wall_img, NULL, &hb);
    SDL_RenderCopy(game->renderer, wall_img, NULL, &vb);

    SDL_Rect sr = {.w = s, .h = s};
    SDL_Rect hr = {.w = s, .h = w};
    SDL_Rect vr = {.w = w, .h = s};

    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            sr.x = ox + (x * s);
            sr.y = oy + (y * s);

            if (lisflipped(n->data, CELL1)) {
                SDL_RenderCopy(game->renderer, cell_img2, NULL, &sr);
            } else if (lisflipped(n->data, CELL2)) {
                SDL_RenderCopy(game->renderer, cell_img3, NULL, &sr);
            }

            if (y != 0 && *(n->wny)) {
                hr.x = sr.x;
                hr.y = sr.y;
                SDL_RenderCopy(game->renderer, wall_img, NULL, &hr);
            }

            if (x != 0 && *(n->wnx)) {
                vr.x = sr.x - HALF(w);
                vr.y = sr.y;
                SDL_RenderCopy(game->renderer, wall_img, NULL, &vr);
            }
        }
    }
}
