#ifdef MAZER_MAIN_H
#error only main.c should include main.h
#else
#define MAZER_MAIN_H
#endif

#include "SDL.h"

int main(int argc, char *argv[]);

void on_event(SDL_Event *event);

// only main.c should ever include this file, so static decls and no include guard
static void testfn(void);
static void testfn2(void);
static void load_all_textures(void);
static void cleanup(void);
static void parse_argv(int *w, int *h);
