#ifndef OBJECTS_H
#define OBJECTS_H

typedef struct {
    uint8_t traversable;
} Tile;

typedef enum {
    RIGHT,
    LEFT,
    UP,
    DOWN
} Direction;

typedef struct {
    int x;
    int y;
    Direction current_direction;
    Direction next_direction;

} Character;

#endif