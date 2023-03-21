#include <stdio.h>
#include <SDL2/SDL.h>

#include "objects.h"
#include "display.h"

#define MAP_W 35
#define MAP_H 20

Tile *map = NULL;
Character* player = NULL;
Character* enemies = NULL;

//TODO load from file
Tile* load_map(uint32_t width, uint32_t height) {
    Tile* result = (Tile*)malloc(sizeof(Tile) * (width * height));
    for (uint32_t i = 0; i < width; i++) {
        for (uint32_t j = 0; j < height; j++) {
            Tile *tile = (result + (i * height) + j);
            if (j == 2 || i == 2) {
                tile->traversable = 0;
            } else {
                tile->traversable = 1;
            }
        }
    }
    return result;
}

Character* spawn_player() {
    Character* result = (Character*)malloc(sizeof(Character));
    result->x = 10;
    result->y = 10;
    return result;
}

Character* spawn_enemies() {
    Character* result = (Character*)malloc(sizeof(Character) * 4);
    (result + 0)->x = 1;
    (result + 0)->y = 1;

    (result + 1)->x = MAP_W - 2;
    (result + 1)->y = 1;

    (result + 2)->x = 1;
    (result + 2)->y = MAP_H - 2;

    (result + 3)->x = MAP_W - 2;
    (result + 3)->y = MAP_H - 2;
    return result;
}

uint8_t initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL. %s\n", SDL_GetError());
        return 0;
    }
    if (!Display_initialize("Pacman")) {
        printf("Could not initialize display");
        return 0;
    }
    
    map = load_map(MAP_W, MAP_H);
    player = spawn_player();
    enemies = spawn_enemies();
    Display_register_map(map, MAP_W, MAP_H, 50);
    Display_register_player(player);
    Display_register_enemies(enemies, 4);
    return 1;
}

void cleanup() {
    Display_cleanup();
    free(map);
    free(player);
    free(enemies);
    map = NULL;
    player = NULL;
    enemies = NULL;
}

void main_loop() {
    SDL_Event e;
    uint8_t quit_game = 0;

    while (quit_game == 0) {
        /* Handle all queued events */
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit_game = 1;
                    break;
                case SDL_KEYDOWN:
                    break;
            }
        }

        /* Handle physics */

        /* Handle display */
        Display_draw_frame();

        /* Don't waste CPU cycles */
        SDL_Delay(1);
    }
}

int main(void) {
    if (!initialize()) {
        printf("Initialization failed. Exitting\n");
        cleanup();
        return -1;
    }
    main_loop();    
    cleanup();
    return 0;
}