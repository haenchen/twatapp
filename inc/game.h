#pragma once

#include <object.h>

struct state {
    int running;
    struct list *players;
};

const struct state *init_state(void);
void add_player(struct object *player);
