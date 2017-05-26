#include <object.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static struct object *create_object(int x, int y, enum object_type type);
static void print_player(struct object *this);
static void list_append(struct list *this, struct object *object);
static void list_remove(struct list *this, struct object *object);

struct object *create_player(int x, int y, int client_id) {
    struct object *this = create_object(x, y, OT_PLAYER);
    this->obj.player.client_id = client_id;
    this->obj.player.health = 100;
    this->obj.player.weapon = WT_SWORD;
    this->obj.player.direction = D_DOWN;
    this->obj.player.moving = 0;
    this->obj.player.cspeed = 100;
    this->print = print_player;
    return this;
}

struct object *create_object(int x, int y, enum object_type type) {
    struct object *this = malloc(sizeof *this);
    this->x = x;
    this->y = y;
    this->type = type;
    this->print = NULL;
    return this;
}

void print_player(struct object *this) {
    printf("<player object at %p>\n", this);
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
