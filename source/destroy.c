#include "destroy.h"

#include <stdarg.h>
#include <string.h>

#include "SDL.h"
#include "SDL_image.h"
#include "definitions.h"

void destroy_resources(char *type, int num, ...) {
    printf("call\n");
    char *types[] = {"Window", "Render", "Texture", "Cond", "Conditional", "Mutex", "Sem", "Semaphore"};
    for (int i = 0; i < 7; i++) {
        if (strcasecmp(type, types[i]) == 0) {
            switch (i) {
                case 0:
                    DESTROY(SDL_Window, SDL_DestroyWindow)
                case 1:
                    DESTROY(SDL_Renderer, SDL_DestroyRenderer)
                case 2:
                    DESTROY(SDL_Texture, SDL_DestroyTexture)
                case 3:
                case 4:
                    DESTROY(SDL_cond, SDL_DestroyCond)
                case 5:
                    DESTROY(SDL_mutex, SDL_DestroyMutex)
                case 6:
                case 7:
                    DESTROY(SDL_sem, SDL_DestroySemaphore)
                default:
                    PRINT_ERR("Impossible!")
                    return;
            }
        }
    }
    PRINT_ERR("Unrecognised type name in destroy_resources call.")
    return;
}