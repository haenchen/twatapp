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

void handle_input(enum input input, int mouse_x, int mouse_y) {
    switch (input) {
        case I_UP: move_player(D_UP);
                   break;
        case I_DOWN: move_player(D_DOWN);
                     break;
        case I_LEFT: move_player(D_LEFT);
                     break;
        case I_RIGHT: move_player(D_RIGHT);
                      break;
        case I_CLICK: printf("%d;%d\n", mouse_x, mouse_y);
                      break;
        case I_QUIT: state->running = 0;
                     break;
        case I_INVALID: 
        case I_NONE: 
        default:
                     return;
    }
}

void move_player(enum direction direction) {
    struct object *player = NULL;
    for (struct element *current = state->players->head;
            current != NULL;
            current = current->next) {
        if (current->garbage ||
                current->object->obj.player.client_id != state->client_id) {
            continue;
        }
        player = current->object;
        break;
    }
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
