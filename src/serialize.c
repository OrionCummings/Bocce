#include "serialize.h"

ErrorCode serialize_game_state(SerializedGameState* serial_state, const GameState state) {

    ErrorCode ec_serialize_bocce_ball_state = serialize_bocce_ball_state(&serial_state, state.bocce_ball_state);
    if (ec_serialize_bocce_ball_state) {
        B_ERROR("Failed to serialize the bocce ball state");
        return ec_serialize_bocce_ball_state;
    }

    return EC_OK;
}

ErrorCode serialize_bocce_ball_state(SerializedBocceBallState* serial_state, const BocceBallState state) {

    // Convert the list of balls to bytes
    size_t size = sizeof(*state.balls) * MAX_BOCCE_BALLS;
    int x = memcpy_s(serial_state->data, size, (byte*)state.balls, size);

    // return EC_SERIALIZE_FAILURE_BOCCE_BALL_STATE;

    return EC_OK;
}