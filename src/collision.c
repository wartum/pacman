#include "constants.h"
#include "collision.h"

static int origin_x = (SCREEN_W / 2) - ((TILE_SIZE * MAP_W) / 2);
static int origin_y = (SCREEN_H / 2) - ((TILE_SIZE * MAP_H) / 2);

uint8_t can_switch_direction(Character* character) {
    return ((character->x - origin_x) % (TILE_SIZE / 2) == 0 ) &&
           ((character->y - origin_y) % (TILE_SIZE / 2) == 0 );
}

uint8_t collide(Character* character) {
    int ret = 0;
    ret += character->current_direction == LEFT && character->x <= origin_x;
    ret += character->current_direction == RIGHT && character->x >= (origin_x + TILE_SIZE * (MAP_W - 1));
    ret += character->current_direction == UP && character->y <= origin_y;
    ret += character->current_direction == DOWN && character->y >= (origin_y + TILE_SIZE * (MAP_H - 1));
    return ret;
}