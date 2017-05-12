#include <SDL2/SDL.h>

#include <game.h>
#include <graphics.h>
#include <object.h>

int main(void) {
    struct object *player = create_player(0, 0, 0);
    player->print(player);
    const struct state *state = init_state();
    add_player(player);
    init_graphics();
    render_game(state);
    SDL_Delay(10000);
    return 0;
}
