#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#include "objects.h"

uint8_t Display_initialize(const char* title);
void Display_cleanup();
void Display_register_map(Tile *map);
void Display_register_player(Character* player);
void Display_register_enemies(Character* enemies, uint32_t n);
void Display_draw_frame();

#endif