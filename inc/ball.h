#ifndef __BALL_H
#define __BALL_H

#include <stdint.h>
#include "colors.h"
#include "bmath.h"

typedef struct BocceBall {
    uint32_t mass;
    V2i position;
    V2i velocity;
    V2i acceleration;
    uint16_t size;
    Color color;
} BocceBall;

#endif