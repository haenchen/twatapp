#pragma once

#include <object.h>

struct input {
    enum direction player_direction;
    int player_moving;
    int mouse_x;
    int mouse_y;
    int mouse_click;
    int quit;
};

void read_input(struct input *input);
