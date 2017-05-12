#include <game.h>

#include <assert.h>
#include <stdlib.h>

static struct state *state;

const struct state *init_state(void) {
    assert(state == NULL);
    state = malloc(sizeof *state);
    state->running = 1;
    state->players = create_list();
    return state;
}

void add_player(struct object *player) {
    state->players->append(state->players, player);
}
