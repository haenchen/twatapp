#include <graphics.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <game.h>
#include <object.h>
#include <util.h>

enum {
    WIDTH = 640,
    HEIGHT = 480,
};

enum {
#include<config/constants.h>
};

enum animation_type {
    AT_PLAYER,
    AT_BULLET,
};

struct template {
    const int speed;
    const int frames;
    const int width;
    const int height;
    const char *path;
    const int directions[4];
};

struct animation {
    int is_ticking;
    int tick;
    int frame;
    enum animation_type type;
};

static SDL_Window *window;
static SDL_Renderer *renderer;

static const struct template templates[] = {
#include <config/templates.h>
};

static struct animation *animations[C_MAX_OBJECTS_PER_PLAYER] = {};

static void clear_references(void);
static void load_texture(int index, const char *filename);

static void render_object(struct object *object);
static void render_bullet(struct object *object);
static void start_animation(struct object *object);
static void tick_animation(struct animation *animation);
static void stop_animation(struct animation *this);

SDL_Texture *texture_map[sizeof templates / sizeof templates[0]];

void init_graphics(void) {
    int status = SDL_Init(SDL_INIT_VIDEO);
    check_SDL(status == 0, "init_graphics: SDL_Init");
    atexit(SDL_Quit);
    atexit(clear_references);
    window = SDL_CreateWindow("twatapp", 0, 0,
                              WIDTH, HEIGHT, 0);
    check_SDL(window != NULL,
              "init_graphics: SDL_CreateWindow");
    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    check_SDL(renderer != NULL,
              "init_graphics: SDL_CreateRenderer");
    for (size_t i = 0; i < sizeof templates / sizeof templates[0]; ++i) {
        load_texture(i, templates[i].path);
    }
}

void render_game(const struct state *state) {
    SDL_SetRenderDrawColor(renderer, 0, 63, 0, 127);
    SDL_RenderClear(renderer);
    foreach(state->players, render_object);
    foreach(state->bullets, render_object);
    SDL_RenderPresent(renderer);

    /* tick _all_ animations */
}

void clear_references(void) {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    renderer = NULL;
    window = NULL;
    for (int i = 0; i < sizeof texture_map / sizeof texture_map[0]; ++i) {
        if (texture_map[i] != 0) {
            SDL_DestroyTexture(texture_map[i]);
        }
        texture_map[i] = NULL;
    }
}

void load_texture(int index, const char *filename) {
    SDL_Surface *surface = IMG_Load(filename);
    check_SDL(surface != NULL, "load_texture");
    Uint32 colorkey = SDL_MapRGB(surface->format, 255, 0, 255);
    SDL_SetColorKey(surface, SDL_TRUE, colorkey);
    texture_map[index] = SDL_CreateTextureFromSurface(renderer, surface);
    check_SDL(texture_map[index] != NULL, "create_texture");
    SDL_FreeSurface(surface);
}

void render_object(struct object *object) {
    if (animations[object->id] == NULL) {
        start_animation(object);
    }
    struct animation *animation = animations[object->id];
    enum direction direction = D_UP;
    if (object->type == OT_PLAYER) {
        direction = object->obj.player.direction;
        if (object->obj.player.moving) {
            tick_animation(animation);
        } else {
            stop_animation(animation);
        }
    } else {
        tick_animation(animation);
    }
    const struct template *template = templates + animation->type;
    int direction_offset = template->frames * template->width * template->directions[direction];
    int frame_offset = animation->frame * template->width;
    SDL_Rect render_position = {object->x,
                                object->y,
                                template->width,
                                template->height};
    SDL_Rect texture_position = {direction_offset + frame_offset,
                                 0,
                                 template->width,
                                 template->height};
    SDL_RenderCopy(renderer, texture_map[animation->type],
                   &texture_position, &render_position);
}

void start_animation(struct object *object) {
    animations[object->id] = malloc(sizeof *animations[object->id]);
    animations[object->id]->is_ticking = 0;
    animations[object->id]->tick = 0;
    animations[object->id]->frame = 0;
    if (object->type == OT_PLAYER) {
        animations[object->id]->type = AT_PLAYER;
    } else {
        animations[object->id]->type = AT_BULLET;
    }
}

void tick_animation(struct animation *this) {
    this->is_ticking = 1;
    this->tick++;
    if (this->tick >= templates[this->type].speed) {
        this->tick = 0;
        this->frame = (this->frame + 1) % templates[this->type].frames;
    }
}

void stop_animation(struct animation *this) {
    this->is_ticking = 0;
    this->tick = templates[this->type].speed - 1;
}
