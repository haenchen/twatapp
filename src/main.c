#include <SDL2/SDL.h>

#include <game.h>
#include <graphics.h>
#include <input.h>
#include <object.h>

int main(void) {
    struct object *player = create_player(0, 0, 0);
    player->print(player);
    const struct state *state = init_state();
    add_player(player);
    init_graphics();
    int mouse_x;
    int mouse_y;
    enum input input;
    while (state->running) {
        do {
            input = read_input(&mouse_x, &mouse_y);
            handle_input(input, mouse_x, mouse_y);
        } while (input != I_NONE);
        render_game(state);
        SDL_Delay(33);
    }
    return 0;
}
