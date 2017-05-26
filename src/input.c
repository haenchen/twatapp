#include <input.h>

#include <SDL2/SDL.h>

static void handle_key_press(SDL_KeyboardEvent kb_event, struct input *input);
static void handle_key_release(SDL_KeyboardEvent kb_event);
static void handle_movement(enum direction direction, int press);

static int current_movements[4];

void read_input(struct input *input) {
    SDL_Event event;
    input->mouse_click = 0;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_KEYDOWN: handle_key_press(event.key, input); break;
            case SDL_KEYUP: handle_key_release(event.key); break;
            case SDL_QUIT: input->quit = 1; break;
            case SDL_MOUSEBUTTONDOWN: input->mouse_click = 1; break;
            default: break;
        }
    }
    input->player_moving = current_movements[D_UP] ||
                           current_movements[D_DOWN] ||
                           current_movements[D_LEFT] ||
                           current_movements[D_RIGHT];
    SDL_GetMouseState(&input->mouse_x, &input->mouse_y);
    for (int i = 0; i < sizeof current_movements / sizeof current_movements[0]; ++i) {
        if (current_movements[i]) {
            input->player_direction = i;
        }
    }
}

void handle_key_press(SDL_KeyboardEvent kb_event, struct input *input) {
    if (kb_event.repeat) {
        return;
    }
    switch (kb_event.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE: input->quit = 1; break;
        case SDL_SCANCODE_W: handle_movement(D_UP, 1); break;
        case SDL_SCANCODE_S: handle_movement(D_DOWN, 1); break;
        case SDL_SCANCODE_A: handle_movement(D_LEFT, 1); break;
        case SDL_SCANCODE_D: handle_movement(D_RIGHT, 1); break;
        default: break;
    }
}

static void handle_key_release(SDL_KeyboardEvent kb_event) {
    switch (kb_event.keysym.scancode) {
        case SDL_SCANCODE_W: handle_movement(D_UP, 0); break;
        case SDL_SCANCODE_S: handle_movement(D_DOWN, 0); break;
        case SDL_SCANCODE_A: handle_movement(D_LEFT, 0); break;
        case SDL_SCANCODE_D: handle_movement(D_RIGHT, 0); break;
        default: break;
    }
}

void handle_movement(enum direction direction, int press) {
    if (press) {
        for (int i = 0; i < sizeof current_movements / sizeof current_movements[0]; ++i) {
            current_movements[i] = 0;
        }
    }
    current_movements[direction] = press;
}
