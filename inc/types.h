#ifndef __TYPES_H
#define __TYPES_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "raylib.h"
#include "ball.h" // TODO: Why is this seperate?

#define false (0)
#define true (1)

#define MAX_CONNECTIONS (16) // NOTE: This is independent of the config.toml
#define MAX_PLAYERS (MAX_CONNECTIONS) // NOTE: This is independent of the config.toml
#define MAX_BOCCE_BALLS (16)
#define MAX_MESSAGE_SIZE_BYTES (256)

typedef uint8_t byte;

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

typedef struct Player {
    uint8_t id;
    bool connected;
    bool active;
} Player;

typedef struct ScoreState {
    uint8_t score_red;
    uint8_t score_black;
} ScoreState;

typedef struct PlayerState {
    Player players[MAX_PLAYERS];
} PlayerState;

typedef struct BocceBallState {
    BocceBall balls[MAX_BOCCE_BALLS];
} BocceBallState;

/// @brief Contains the state of the game. This data is to be synced between the server and all connected clients
typedef struct GameState {
    ScoreState score_state;
    BocceBallState bocce_ball_state;
    PlayerState player_state;
} GameState;


#endif