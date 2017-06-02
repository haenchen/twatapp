#include <SDL2/SDL.h>

#include <game.h>
#include <graphics.h>
#include <input.h>
#include <object.h>

enum magic_numbers {
    MAGIC_33 = 33
};

int main(void) {
    struct object *player = create_player(0, 0, 0);
    player->print(player);
    const struct state *state = init_state();
    add_player(player);
    init_graphics();
    struct input input = {.player_moving = 0,
                          .quit = 0,
                          .mouse_click = 0
                         };
    while (state->running) {
        read_input(&input);
        handle_input(&input);
        progress_game();
        render_game(state);
        SDL_Delay(MAGIC_33);
    }
    return 0;
}
