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
};

static SDL_Window *window;
static SDL_Renderer *renderer;

static void clear_references(void);
static void load_texture(int index, const char *filename);

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
}

void render_game(const struct state *state) {
    SDL_SetRenderDrawColor(renderer, 0, 127, 0, 255);
    SDL_RenderClear(renderer);
    for (struct element *current = state->players->head;
            current != NULL;
            current = current->next) {
        SDL_Rect render_position = {current->object->x,
                                    current->object->y,
                                    SPRITE_SIZE,
                                    SPRITE_SIZE};
        SDL_Rect texture_position = {2 * SPRITE_SIZE * current->object->obj.player.direction,
                                     0,
                                     SPRITE_SIZE,
                                     SPRITE_SIZE};
        SDL_RenderCopy(renderer, texture_map[current->object->type],
                       &texture_position, &render_position);
    }
    SDL_RenderPresent(renderer);
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
