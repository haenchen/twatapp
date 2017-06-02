#include <graphics.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <game.h>
#include <object.h>
#include <util.h>

enum {
    WIDTH = 640,
    HEIGHT = 480,
    SPRITE_SIZE = 32,
    BULLET_SIZE = 13,
    ANIMATION_SPEED = 8,
};

enum animation_type {
    AT_PLAYER,
    AT_BULLET,
};

struct animation_template {
    int speed;
    int frames;
};

struct animation {
    int is_ticking;
    int tick;
    int frame;
    enum animation_type type;
};


static SDL_Window *window;
static SDL_Renderer *renderer;
static struct animation_template templates[2] = {
    {.speed = 8, .frames = 2},
    {.speed = 8, .frames = 2}
};
static struct animation player_animation = {.is_ticking = 0, .tick = 0, .frame = 0, .type = AT_PLAYER};

static void clear_references(void);
static void load_texture(int index, const char *filename);

static void render_player(struct object *object);
static void render_bullet(struct object *object);
static void tick_animation(struct animation *animation);
static void stop_animation(struct animation *this);

SDL_Texture *texture_map[5];

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
    load_texture(OT_PLAYER, "res/player_sprite.bmp");
    load_texture(OT_BULLET, "res/bullet.png");
}

void render_game(const struct state *state) {
    SDL_SetRenderDrawColor(renderer, 0, 127, 0, 255);
    SDL_RenderClear(renderer);
    foreach(state->players, render_player);
    foreach(state->bullets, render_bullet);
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

void render_bullet(struct object *object) {
    SDL_Rect render_position = {object->x,
                                object->y,
                                BULLET_SIZE,
                                BULLET_SIZE};
    SDL_Rect texture_position = {0,
                                 0,
                                 BULLET_SIZE,
                                 BULLET_SIZE};
    SDL_RenderCopy(renderer, texture_map[object->type],
                   &texture_position, &render_position);
}
void render_player(struct object *object) {
    int x = player_animation.frame;
    if (object->obj.player.moving) {
        tick_animation(&player_animation);
    } else if (player_animation.is_ticking){
        stop_animation(&player_animation);
    }
    SDL_Rect render_position = {object->x,
                                object->y,
                                SPRITE_SIZE,
                                SPRITE_SIZE};
    SDL_Rect texture_position = {2 * SPRITE_SIZE * object->obj.player.direction + x * SPRITE_SIZE,
                                 0,
                                 SPRITE_SIZE,
                                 SPRITE_SIZE};
    SDL_RenderCopy(renderer, texture_map[object->type],
                   &texture_position, &render_position);
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
