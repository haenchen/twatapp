#include <game.h>

#include <assert.h>
#include <stdlib.h>
#include <math.h>

static struct state *state;

static void move_player(enum direction direction);
static int p_current_player(struct object *object);
static void add_bullet(struct object *bullet);
static void move_bullet(struct object *this);

const struct state *init_state(void) {
    assert(state == NULL);
    state = malloc(sizeof *state);
    state->running = 1;
    state->players = create_list();
    state->bullets = create_list();
    return state;
}

void add_player(struct object *player) {
    state->players->append(state->players, player);
}

void add_bullet(struct object *bullet) {
    state->bullets->append(state->bullets, bullet);
}

void handle_input(const struct input *input) {
    struct object *player = find(state->players, p_current_player);
    assert(player != NULL);
    player->obj.player.moving = input->player_moving;
    if (input->player_moving) {
        move_player(input->player_direction);
    }
    if (input->mouse_click) {
        int dir_x = input->mouse_x - player->x;
        int dir_y = input->mouse_y - player->y;
        struct object *bullet = create_bullet(player->x, player->y, dir_x, dir_y,
                                              player->obj.player.client_id,
                                              player->obj.player.weapon);
        add_bullet(bullet);
    }
    if (input->quit) {
        state->running = 0;
    }
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

int p_current_player(struct object *object) {
    return object->obj.player.client_id == state->client_id;
}

void progress_game() {
    foreach(state->bullets, move_bullet);
}

void move_bullet(struct object *this) {
    this->x += this->obj.bullet.dir_x * this->obj.bullet.velocity;
    this->y += this->obj.bullet.dir_y * this->obj.bullet.velocity;
}
