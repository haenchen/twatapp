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
void handle_input(const struct input *input);
void progress_game();
