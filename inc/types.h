#ifndef __TYPES_H
#define __TYPES_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "raylib.h"
#include "ball.h"

#define MAX_PLAYERS (16) // NOTE: This is independent of the config.toml

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
    Ball* balls;
} GameState;

#endif