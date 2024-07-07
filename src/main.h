#ifdef MAZER_MAIN_H
#error only main.c should include main.h
#else
#define MAZER_MAIN_H
#endif

#include "SDL.h"

int main(int argc, char *argv[]);

// only main.c should ever include this file, so static decls and no include guard
static void cleanup(void);
