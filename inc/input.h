#pragma once

enum input {
    I_UP,
    I_DOWN,
    I_LEFT,
    I_RIGHT,
    I_CLICK,
    I_QUIT,
    I_INVALID,
    I_NONE,
};

enum input read_input(int *mouse_x, int *mouse_y);
