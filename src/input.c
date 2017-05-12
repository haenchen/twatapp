#include <input.h>

#include <SDL2/SDL.h>

static enum input get_input(SDL_Scancode scancode);

enum input read_input(int *mouse_x, int *mouse_y) {
    SDL_Event event;
    if (SDL_PollEvent(&event) == 0) {
        return I_NONE;
    }
    switch (event.type) {
        case SDL_KEYDOWN:
            return get_input(event.key.keysym.scancode);
        case SDL_QUIT:
            return I_QUIT;
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(mouse_x, mouse_y);
            return I_CLICK;
        default:
            return I_INVALID;
    }
}

static enum input get_input(SDL_Scancode scancode) {
    switch (scancode) {
        case SDL_SCANCODE_W: return I_UP;
        case SDL_SCANCODE_S: return I_DOWN;
        case SDL_SCANCODE_A: return I_LEFT;
        case SDL_SCANCODE_D: return I_RIGHT;
        case SDL_SCANCODE_ESCAPE: return I_QUIT;
        default: return I_INVALID;
    }
}
