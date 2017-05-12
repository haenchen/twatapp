#include <util.h>

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

void check_SDL(int condition, const char *msg) {
    if (!condition) {
        fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
