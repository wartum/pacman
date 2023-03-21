#include <stdio.h>

#include "display.h"

#define SCREEN_W 1920
#define SCREEN_H 1080
#define FPS_CAP 32

typedef struct {
    uint8_t fps_counter;
    SDL_Window *window;
    Tile *map;
    uint32_t map_w, map_h, tile_size;
    Character* player;
    Character* enemies;
    uint32_t enemies_n;
} Display;

Display display;
SDL_Rect buff_rect;

uint8_t Display_initialize(const char* title) {
    display.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (display.window == NULL) {
        printf("Could not create window. %s\n", SDL_GetError());
        return 0;
    }
    display.fps_counter = 0;
    return 1;
}

void Display_cleanup() {
    SDL_DestroyWindow(display.window);
    display.window = NULL;
    display.fps_counter = 0;
    display.map = NULL;
    display.map_w = 0;
    display.map_h = 0;
}

Tile* get_tile(uint16_t x, uint16_t y) {
    return display.map + (x * display.map_h) + y;
}

void coord_to_rect(int x, int y) {
    int origin_x = (SCREEN_W / 2) - ((display.tile_size * display.map_w) / 2);
    int origin_y = (SCREEN_H / 2) - ((display.tile_size * display.map_h) / 2);
    buff_rect.x = ((x * display.tile_size) + origin_x);
    buff_rect.y = ((y * display.tile_size) + origin_y);
}

void Display_register_map(Tile *map, uint32_t width, uint32_t height, uint32_t tile_size) {
    display.map = map;
    display.map_w = width;
    display.map_h = height;
    display.tile_size = tile_size;
    int origin_x = (SCREEN_W / 2) - ((tile_size * display.map_w) / 2);
    int origin_y = (SCREEN_H / 2) - ((tile_size * display.map_h) / 2);
    Tile *tile = display.map;
    for (uint32_t i = 0; i < display.map_w; i++) {
        for (uint32_t j = 0; j < display.map_h; j++) {
            tile += 1;
        }
    }
    buff_rect.w = tile_size - 5;
    buff_rect.h = tile_size - 5;
}

void Display_register_player(Character* player) {
    display.player = player;
    coord_to_rect(player->x, player->y);
    player->x = buff_rect.x;
    player->y = buff_rect.y;
}

void Display_register_enemies(Character* enemies, uint32_t n) {
    display.enemies = enemies;
    display.enemies_n = n;
    for (uint32_t i = 0; i < n; i++) {
        coord_to_rect((enemies + i)->x, (enemies + i)->y);
        (enemies + i)->x = buff_rect.x;
        (enemies + i)->y = buff_rect.y;
    }
}

void draw_map() {
    static SDL_Rect clrscr = { 0, 0, SCREEN_W, SCREEN_H };
    SDL_Surface *surface = SDL_GetWindowSurface(display.window); 
    SDL_FillRect(surface, &clrscr, SDL_MapRGB(surface->format, 0, 0, 0));
    
    for (uint32_t i = 0; i < display.map_w; i++) {
        for (uint32_t j = 0; j < display.map_h; j++) {
            coord_to_rect(i, j);
            if (get_tile(i, j)->traversable) {
                SDL_FillRect(surface, &buff_rect, SDL_MapRGB(surface->format, 50, 160, 0));
            } else {
                SDL_FillRect(surface, &buff_rect, SDL_MapRGB(surface->format, 80, 80, 220));
            }
        }
    }

    for (uint32_t i = 0; i < display.enemies_n; i++) {
        buff_rect.x = (display.enemies + i)->x;
        buff_rect.y = (display.enemies + i)->y;
        SDL_FillRect(surface, &buff_rect, SDL_MapRGB(surface->format, 255, 0, 255));
    }

    buff_rect.x = display.player->x;
    buff_rect.y = display.player->y;
    SDL_FillRect(surface, &buff_rect, SDL_MapRGB(surface->format, 255, 255, 0));

}

void calculate_fps() {
    static uint32_t fps_calculation_countdown = 0;
    if (fps_calculation_countdown <= SDL_GetTicks()) {
        printf("Current fps:%d\n", display.fps_counter);
        fps_calculation_countdown = SDL_GetTicks() + 1000;
        display.fps_counter = 0;
    }
}

void Display_draw_frame() {
    static uint32_t next_frame_countdown = 0;

    if (next_frame_countdown <= SDL_GetTicks()) {
        next_frame_countdown = SDL_GetTicks() + (1000 / FPS_CAP);

        /* Draw staff on surface */
        draw_map(display);

        /* Display surface */
        if (SDL_UpdateWindowSurface(display.window) < 0) {
            printf("Updating window surface failed. %s\n", SDL_GetError());
        }
        display.fps_counter += 1;
    }
    calculate_fps();
}
