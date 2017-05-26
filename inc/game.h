#pragma once

#include <object.h>
#include <input.h>

struct state {
    int running;
    int client_id;
    struct list *players;
};

const struct state *init_state(void);
void add_player(struct object *player);
void handle_input(const struct input *input);
