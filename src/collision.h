#ifndef COLLIDE_H
#define COLLIDE_H

#include <stdint.h>
#include "objects.h"

uint8_t can_switch_direction(Character* character);
uint8_t collide(Character* character);

#endif