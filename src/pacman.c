#include <stdio.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "objects.h"
#include "collision.h"
#include "display.h"

Tile *map = NULL;
Character* player = NULL;
Character* enemies = NULL;
uint32_t turn_countdown = 0;

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

void handle_keyboard_input(SDL_Keycode keycode) {
    switch(keycode) {
    case SDLK_RIGHT:
        player->next_direction = RIGHT;
        break;
    case SDLK_LEFT:
        player->next_direction = LEFT;
        break;
    case SDLK_UP:
        player->next_direction = UP;
        break;
    case SDLK_DOWN:
        player->next_direction = DOWN;
        break;
    default:
        break;
    }
}

Direction chase(Character* chaser, Character* chased) {
    if (chaser->x > chased->x + 100 ) {
        return LEFT;
    }

    if (chaser->x < chased->x - 100 ) {
        return RIGHT;
    }

    if (chaser->y > chased->y + 100 ) {
        return UP;
    }

    if (chaser->y > chased->y - 100 ) {
        return DOWN;
    }
}

void handle_movement() {
    if (turn_countdown <= SDL_GetTicks()) {
        turn_countdown = SDL_GetTicks() + 20;
        
        // Player
        if (can_switch_direction(player)) {
            player->current_direction = player->next_direction;
        }

        switch(player->current_direction) {
            case RIGHT:
                player->x += !collide(player) * PIXEL_PER_TURN;
                break;
            case LEFT:
                player->x -= !collide(player) * PIXEL_PER_TURN;
                break;
            case UP:
                player->y -= !collide(player) * PIXEL_PER_TURN;
                break;
            case DOWN:
                player->y += !collide(player) * PIXEL_PER_TURN;
                break;
        }

        // Enemies
        for (uint32_t i = 0; i < 4; i++) {
            Character* enemy = (enemies + i);
            enemy->next_direction = chase(enemy, player);

            if (can_switch_direction(enemy)) {
                enemy->current_direction = enemy->next_direction;
            }

            switch(enemy->current_direction) {
                case RIGHT:
                    enemy->x += !collide(enemy) * PIXEL_PER_TURN;
                    break;
                case LEFT:
                    enemy->x -= !collide(enemy) * PIXEL_PER_TURN;
                    break;
                case UP:
                    enemy->y -= !collide(enemy) * PIXEL_PER_TURN;
                    break;
                case DOWN:
                    enemy->y += !collide(enemy) * PIXEL_PER_TURN;
                    break;
            }
        }
    }
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
    Display_register_map(map);
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
                    handle_keyboard_input(e.key.keysym.sym);
                    break;
            }
        }

        /* Handle physics */
        handle_movement();

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