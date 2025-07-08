#ifndef __SERIALIZE_H
#define __SERIALIZE_H

#include <stdint.h>
#include "debug.h"
#include "error_codes.h"
#include "types.h"

typedef struct SerializedGameState {
    byte data[sizeof(GameState)];
} SerializedGameState;

typedef struct SerializedBocceBallState {
    byte data[sizeof(BocceBallState)];
} SerializedBocceBallState;

typedef struct SerializedPlayerState {
    byte data[sizeof(PlayerState)];
} SerializedPlayerState;

ErrorCode serialize_game_state(SerializedGameState*, const GameState);
ErrorCode serialize_bocce_ball_state(SerializedBocceBallState*, const BocceBallState);
ErrorCode serialize_player_state(SerializedPlayerState*, const PlayerState);


#endif