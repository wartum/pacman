#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#include "objects.h"

uint8_t Display_initialize(const char* title);
void Display_cleanup();
void Display_register_map(Tile *map, uint32_t width, uint32_t height, uint32_t scale);
void Display_draw_frame();

#endif