#ifndef DEFINITIONS_H
#define DEFINITOONS_H

#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"

#define WIN_INIT_WIDTH 800
#define WIN_INIT_HEIGHT 600

#define MIN(a, b) ((a <= b) ? a : b)
#define MAX(a, b) ((a >= b) ? a : b)

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__);
#define EXIT_ERR(...)           \
    do {                        \
        PRINT_ERR(__VA_ARGS__); \
        exit(EXIT_FAILURE);     \
    } while (0);

#endif /* DEFINITIONS_H */