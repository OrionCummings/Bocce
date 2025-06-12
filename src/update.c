#include "update.h"

ErrorCode update(ApplicationSettings* settings, Server* server, Client* client, GameState* state, Chat* chat){

    // NOTE: These parameters can safely be deferenced here
    // because they are all null-checked prior to being passed.

    ErrorCode ec_update_game_state = update_game_state(state);
    if (ec_update_game_state) {
        B_ERROR("Failed to update game state");
        return ec_update_game_state;
    }

    // Update the chat window
    ErrorCode ec_update_chat = update_chat(chat);
    if (ec_update_chat) {
        B_ERROR("Failed to update chat");
        return ec_update_chat;
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

Ball generate_ball(Vector2 mouse_position) {
    Ball b = { 0 };
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

    return EC_OK;
}

ErrorCode update_client(Client* client){

    return EC_OK;
}

ErrorCode update_chat(Chat* chat) {

    int key = GetCharPressed();

    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (chat->pre_buffer_index < 128)) { // TODO: Magic number!
            chat->pre_buffer.text[chat->pre_buffer_index] = (char)key;
            chat->pre_buffer.text[chat->pre_buffer_index + 1] = '\0'; // Add null terminator at the end of the string.
            chat->pre_buffer_index++;
            B_INFO("[%d]: '%s'", chat->pre_buffer_index, chat->pre_buffer.text);
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
        if (chat->pre_buffer_index >= 1) {
            chat->pre_buffer_index--;
        }
        chat->pre_buffer.text[chat->pre_buffer_index] = '\0';
        B_INFO("[%d]: '%s'", chat->pre_buffer_index, chat->pre_buffer.text);
    }

    return EC_OK;
}






