#include <stdio.h>

#include "display.h"

#define SCREEN_W 1920
#define SCREEN_H 1080
#define FPS_CAP 32

typedef struct {
    uint8_t fps_counter;
    SDL_Window *window;
    Tile *map;
    uint32_t map_w, map_h;
} Display;

Display display;

uint8_t Display_initialize(const char* title) {
    display.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (display.window == NULL) {
        printf("Could not create window. %s\n", SDL_GetError());
        return 0;
    }
    return 1;
}

void Display_cleanup() {
    SDL_DestroyWindow(display.window);
    display.window = NULL;
    display.map = NULL;
    display.map_w = 0;
    display.map_h = 0;
}

Tile* get_tile(uint16_t x, uint16_t y) {
    return display.map + (x * display.map_h) + y;
}

void Display_register_map(Tile *map, uint32_t width, uint32_t height, uint32_t tile_size) {
    display.map = map;
    display.map_w = width;
    display.map_h = height;
    int origin_x = (SCREEN_W / 2) - ((tile_size * display.map_w) / 2);
    int origin_y = (SCREEN_H / 2) - ((tile_size * display.map_h) / 2);
    for (uint32_t i = 0; i < display.map_w; i++) {
        for (uint32_t j = 0; j < display.map_h; j++) {
            Tile *tile = get_tile(i, j);
            tile->rectangle.w = tile_size - 5;
            tile->rectangle.h = tile_size - 5;
            tile->rectangle.x = (tile_size * i) + origin_x;
            tile->rectangle.y = (tile_size * j) + origin_y;
        }
    }
}

void draw_map() {
    static SDL_Rect clrscr = { 0, 0, SCREEN_W, SCREEN_H };
    SDL_Surface *surface = SDL_GetWindowSurface(display.window); 
    SDL_FillRect(surface, &clrscr, SDL_MapRGB(surface->format, 0, 0, 0));
    
    for (uint32_t i = 0; i < display.map_w; i++) {
        for (uint32_t j = 0; j < display.map_h; j++) {
            if (get_tile(i, j)->traversable) {
                    SDL_FillRect(surface, &get_tile(i, j)->rectangle, SDL_MapRGB(surface->format, 50, 160, 0));
            } else {
                    SDL_FillRect(surface, &get_tile(i, j)->rectangle, SDL_MapRGB(surface->format, 80, 80, 220));
            }
        }
    }
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
