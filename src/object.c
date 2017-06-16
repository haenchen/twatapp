#include <object.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum {
#include<config/constants.h>
};

static struct object *create_object(int x, int y, enum object_type type);
static void print_player(struct object *this);
static void print_bullet(struct object *this);
static void list_append(struct list *this, struct object *object);
static void list_remove(struct list *this, struct object *object);

static unsigned current_id = 0;

struct object *create_bullet(int x, int y, int dir_x, int dir_y, int player_id, enum weapon_type type) {
    struct object *this = create_object(x, y, OT_BULLET);
    this->obj.bullet.type = type;
    this->obj.bullet.player_id = player_id;
    this->obj.bullet.dmg = 1;
    double length = sqrt(dir_x * dir_x + dir_y * dir_y);
    this->obj.bullet.dir_x = dir_x / length;
    this->obj.bullet.dir_y = dir_y / length;
    this->obj.bullet.velocity = 10;
    this->print = print_bullet;
    return this;
}
struct object *create_player(int x, int y, int client_id) {
    struct object *this = create_object(x, y, OT_PLAYER);
    this->obj.player.client_id = client_id;
    this->obj.player.health = 100;
    this->obj.player.weapon = WT_SWORD;
    this->obj.player.direction = D_DOWN;
    this->obj.player.moving = 0;
    this->obj.player.cspeed = 50;
    this->print = print_player;
    return this;
}

struct object *create_object(int x, int y, enum object_type type) {
    struct object *this = malloc(sizeof *this);
    if (current_id == C_MAX_OBJECTS_PER_PLAYER) {
        exit(EXIT_FAILURE);
    }
    this->id = current_id++;
    this->x = x;
    this->y = y;
    this->type = type;
    this->print = NULL;
    return this;
}

void print_player(struct object *this) {
    printf("<player object at %p>\n", this);
}

void print_bullet(struct object *this) {
    printf("<bullet object at %p>\n", this);
    printf("\tposition (%3.3g | %3.3g)\n", this->x, this->y);
    printf("\tdirection (%3.3g | %3.3g)\n", this->obj.bullet.dir_x, this->obj.bullet.dir_y);
}

struct list *create_list(void) {
    struct list *this = malloc(sizeof *this);
    this->length = 0;
    this->head = NULL;
    this->last = NULL;
    this->append = list_append;
    this->remove = list_remove;
    return this;
}

void list_append(struct list *this, struct object *object) {
    assert(this != NULL);
    struct element *new = malloc(sizeof *this->last);
    new->object = object;
    new->garbage = 0;
    new->next = NULL;
    if (this->head == NULL) {
        this->head = this->last = new;
    } else {
        this->last->next = new;
        this->last = new;
    }
    ++this->length;
}

void list_remove(struct list *this, struct object *object) {
    assert(this != NULL);
    for (struct element *current = this->head;
            current != NULL;
            current = current->next) {
        if (current->object != object) {
            continue;
        }
        current->garbage = 1;
        --this->length;
        break;
    }
}

void foreach(struct list *list, void (*f)(struct object *)) {
    assert(list != NULL);
    for (struct element *current = list->head;
            current != NULL;
            current = current->next) {
        if (current->garbage) {
            continue;
        }
        f(current->object);
    }
}

struct object *find(struct list *list, int (*p)(struct object *)) {
    assert(list != NULL);
    for (struct element *current = list->head;
            current != NULL;
            current = current->next) {
        if (!current->garbage && p(current->object)) {
            return current->object;
        }
    }
    return NULL;
}
