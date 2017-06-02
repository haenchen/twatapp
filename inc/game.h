#pragma once

#include <object.h>
#include <input.h>

struct state {
    int running;
    int client_id;
    struct list *players;
    struct list *bullets;
};

const struct state *init_state(void);
void add_player(struct object *player);
void add_bullet(struct object *bullet);
void handle_input(const struct input *input);
