#ifndef __TYPES_H
#define __TYPES_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "raylib.h"
#include "ball.h"

#define MAX_PLAYERS (16) // NOTE: This is independent of the config.toml

// A Ceritifed C Moment:
// It's 2025 and the C standard committee has 
// FINALLY decided to elevate the Boolean literals
// 'true' and 'false' to reserved keywords; however,
// the implementation of this change has not yet
// been made. It ""should"" be coming as part of 
// C23 (which this project is built with), but
// who even knows at this point. Sigh.
// ...
// Oh yea. These macros do the thing. They add bools.
// *thumbs up*
#define false ((bool) + 0)
#define  true ((bool) + 1)

typedef enum ApplicationMode {
    AM_UNKNOWN = 0,
    AM_CLIENT = 1,
    AM_SERVER = 2,
    AM_DUAL = 3,
} ApplicationMode;

typedef struct WindowSettings {
    int window_width;
    int window_height;
    bool fullscreen;
    char window_title[256];
    int log_level;
    int target_fps;
    unsigned int config_flags;
} WindowSettings;

typedef struct ApplicationSettings {
    ApplicationMode application_mode;
    WindowSettings window_settings;
} ApplicationSettings;

typedef struct GameState {
    uint16_t num_balls;
    Ball balls[12];
} GameState;

#endif