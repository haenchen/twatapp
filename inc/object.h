#pragma once

enum object_type {
    OT_OBSTACLE,
    OT_ITEM,
    OT_PLAYER,
    OT_BULLET,
    OT_FIELD_EFFECT,
};

enum weapon_type {
    WT_SWORD,
};

enum direction {
    D_UP,
    D_DOWN,
    D_LEFT,
    D_RIGHT,
};

enum item_type {
    IT_INDESTRUCTIBLE,
    IT_HEALING,
};

enum field_type {
    FT_BL00D,
};

struct field_effect {
    enum field_type type;
    unsigned ttl;
};

struct obstacle {
    int destructible;
    unsigned health;
};

struct item {
    enum item_type item_type;
};

struct bullet {
    enum weapon_type type;
    int player_id;
    int dmg;
    int dir_x;
    int dir_y;
    unsigned velocity;
};

struct player {
    unsigned health;
    int client_id;
    enum weapon_type weapon;
    enum direction direction;
    int moving;
    unsigned cspeed;
};

struct object {
    int x;
    int y;
    enum object_type type;
    union {
        struct player player;
        struct bullet bullet;
        struct item item;
        struct obstacle obstacle;
        struct field_effect effect;
    } obj;
    void (*print)(struct object *);
};

struct list {
    unsigned length;
    struct element *head;
    struct element *last;
    void (*append)(struct list *, struct object *);
    void (*remove)(struct list *, struct object *);
};

struct element {
    struct object *object;
    int garbage;
    struct element *next;
};

struct object *create_player(int x, int y, int client_id);

struct list *create_list(void);
