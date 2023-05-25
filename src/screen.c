#include "screen.h"

#include "common.h"
#include "math.h"

void draw_graph(Graph *g) {
    int ww, wh;
    SDL_GetWindowSize(game->win, &ww, &wh);

    int s = round(MIN((ww * 0.95) / g->nc, (wh * 0.95) / g->nr));
    int w = s * 0.05;
    s *= 0.9;

    int ox = HALF(ww - (s + w) * g->nc) + HALF(w);
    int oy = HALF(wh - (s + w) * g->nr) + HALF(w);

    SDL_RenderCopy(game->renderer, game->resources->textures[BG_GREEN], NULL, NULL);

    SDL_Texture *cell_img = game->resources->textures[CLR_LYELLOW];
    SDL_Texture *cell_img2 = game->resources->textures[CLR_DBLUE];
    SDL_Texture *cell_img3 = game->resources->textures[CLR_LORANGE];
    SDL_Texture *wall_img = game->resources->textures[CLR_BLACK];

    SDL_Rect ss = {.x = ox, .y = oy, .w = s * g->nc, .h = s * g->nr};
    SDL_RenderCopy(game->renderer, cell_img, NULL, &ss);

    SDL_Rect sr = {.w = s, .h = s};
    SDL_Rect hr = {.w = s, .h = w};
    SDL_Rect vr = {.w = w, .h = s};

    for (int y = 0; y < g->nr; y++) {
        for (int x = 0; x < g->nc; x++) {
            Node *n = get(g, x, y);
            sr.x = ox + (x * s);
            sr.y = oy + (y * s);

            if (lisflipped(n->data, 31)) {
                SDL_RenderCopy(game->renderer, cell_img2, NULL, &sr);
            } else if (lisflipped(n->data, 30)) {
                SDL_RenderCopy(game->renderer, cell_img3, NULL, &sr);
            }

            if (y != 0 && *(n->wpy)) {
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
