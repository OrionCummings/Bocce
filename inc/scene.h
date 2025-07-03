#ifndef __SCENE_H
#define __SCENE_H

#include <stdint.h>
#include "types.h"
#include "error_codes.h"

#define NUM_SCENES (3)

typedef uint8_t SceneID;

typedef enum Scenes {
    SCENE_MAIN = 0,
    SCENE_LOBBY = 1,
    SCENE_GAME = 2,
} Scenes;

static inline const char* get_scene_name(SceneID id) {
    switch (id) {
        case (SCENE_MAIN): return "Main";
        case (SCENE_LOBBY): return "Lobby";
        case (SCENE_GAME): return "Game";
        default: return "Unknown Scene";
    }
}

static const bool SCENE_TRANSITIONS[NUM_SCENES * NUM_SCENES] = {
    //  to      Main    Lobby   Game <- from
    //  Main      X       X       X
    //  Lobby     X       X       X
    //  Game              X       X

    //  to        0       1       2 <- from
    //  0        [0]     [1]     [2]
    //  1        [3]     [4]     [5]
    //  2                [7]     [8]
    true,  true,  true,
    true,  true,  true,
    false, true,  true,
};

bool is_valid_transition(SceneID, SceneID);

#endif