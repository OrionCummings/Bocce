#include "update.h"

ErrorCode update(ApplicationSettings* settings, Server* server, Client* client, GameState* state){

    // NOTE: These parameters can safely be deferenced here
    // because they are all null-checked prior to being passed.

    ErrorCode ec_update_game_state = update_game_state(state);
    if (ec_update_game_state) {
        B_ERROR("Failed to update game state");
        return ec_update_game_state;
    }

    if (is_server(*settings)){
        ErrorCode ec_update_server = update_server(server);
        if (ec_update_server) {
            B_ERROR("Failed to update server");
            return ec_update_server;
        }
    }

    if (is_client(*settings)){
        ErrorCode ec_update_client = update_client(client);
        if (ec_update_client) {
            B_ERROR("Failed to update client");
            return ec_update_client;
        }
    }

    return EC_OK;
}

ErrorCode update_game_state(GameState* state) {

    // Add balls on click
    // const uint16_t max_num_balls = 12;
    // if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    //     Ball new_ball = generate_ball(GetMousePosition());
    //     if (state->num_balls < max_num_balls) {
    //         state->balls[state->num_balls++] = new_ball;
    //         B_INFO("New ball created at (%d, %d) #%hd", (int)new_ball.position.x, (int)new_ball.position.y, state->num_balls);
    //     } else if (state->num_balls >= max_num_balls) {
    //         B_INFO("Failed to add ball at (%d, %d); maximum number of balls exist (%hd)", (int)new_ball.position.x, (int)new_ball.position.y, max_num_balls);
    //     }
    // }

    return EC_OK;
}

BocceBall generate_ball(V2i mouse_position) {
    BocceBall b = { 0 };
    b.color = get_random_color();
    b.position = mouse_position;
    b.size = 20.0;
    return b;
}

Color get_random_color(void) {
    unsigned char r = (unsigned char)((float)rand() / (float)(RAND_MAX / 201)) + 27;
    unsigned char g = (unsigned char)((float)rand() / (float)(RAND_MAX / 201)) + 27;
    unsigned char b = (unsigned char)((float)rand() / (float)(RAND_MAX / 201)) + 27;
    return (Color){ r, g, b, 255 };
}

ErrorCode update_server(Server* server){

    // Check for new connections


    // Update existing connections with new information
    for (uint8_t connection_id = 0; connection_id < MAX_CONNECTIONS; connection_id++) {

        // Get the socket
        // TcsSocket client_socket = server->player_sockets[connection_id];

        // if (client_socket) {

        // }

        // Send the updated game state to the client



    }

    return EC_OK;
}

ErrorCode update_client(Client* client){

    return EC_OK;
}


