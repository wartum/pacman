#include "constants.h"
#include "collision.h"

static int origin_x = (SCREEN_W / 2) - ((TILE_SIZE * MAP_W) / 2);
static int origin_y = (SCREEN_H / 2) - ((TILE_SIZE * MAP_H) / 2);

Tile* tile_at(int x, int y, Tile* map) {
    int xp = (x - origin_x) / TILE_SIZE;
    int yp = (y - origin_y) / TILE_SIZE;

    return map + (xp * MAP_H) + yp;
}

Tile* neighbour_tile(Character* character, Tile* map, Direction neighbour) {
    int x = character->x;
    int y = character->y;
    if (neighbour == RIGHT) {
        x += TILE_SIZE + 1;
    } else if (neighbour == LEFT) {
        x -= 1;
    } else if (neighbour == UP) {
        y -= 1;
    } else if (neighbour == DOWN) {
        y += TILE_SIZE + 1;
    }
    Tile* ret = tile_at(x, y, map);
    //ret->traversable = 1;
    return ret;
}

uint8_t can_switch_direction(Character* character) {
    return ((character->x - origin_x) % (TILE_SIZE / 2) == 0 ) &&
           ((character->y - origin_y) % (TILE_SIZE / 2) == 0 );
}

uint8_t collide(Character* character, Tile* map) {
    int ret = 0;
    ret += character->current_direction == LEFT && character->x <= origin_x;
    ret += character->current_direction == RIGHT && character->x >= (origin_x + TILE_SIZE * (MAP_W - 1));
    ret += character->current_direction == UP && character->y <= origin_y;
    ret += character->current_direction == DOWN && character->y >= (origin_y + TILE_SIZE * (MAP_H - 1));
    ret += neighbour_tile(character, map, character->current_direction)->traversable == 0;
    return ret;
}