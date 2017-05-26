#include <game.h>

#include <assert.h>
#include <stdlib.h>

static struct state *state;

static void move_player(enum direction direction);

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

void handle_input(const struct input *input) {
    if (input->player_moving) {
        move_player(input->player_direction);
    }
    if (input->mouse_click) {
        printf("%d;%d\n", input->mouse_x, input->mouse_y);
    }
    if (input->quit) {
        state->running = 0;
    }
}

int p_current_player(struct object *object) {
    return object->obj.player.client_id == state->client_id;
}

void move_player(enum direction direction) {
    struct object *player = find(state->players, p_current_player);
    if (player == NULL) {
        return;
    }
    unsigned speed = player->obj.player.cspeed / 10;
    switch (direction) {
        case D_UP: player->y -= speed; break;
        case D_DOWN: player->y += speed; break;
        case D_LEFT: player->x -= speed; break;
        case D_RIGHT: player->x += speed; break;
    }
    player->obj.player.direction = direction;
}
