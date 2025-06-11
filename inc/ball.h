#ifndef __BALL_H
#define __BALL_H

#include <stdint.h>
#include "raylib.h"

typedef struct Ball {
    uint32_t mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    uint16_t size;
    Color color;
} Ball;

#endif