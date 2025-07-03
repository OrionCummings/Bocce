#include "update.h"

ErrorCode update(ApplicationSettings* settings, Server* server, Client* client, GameState* state, Chat* chat, SceneID* scene_current){

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
        ErrorCode ec_update_client = update_client(client, scene_current);
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

ErrorCode update_client(Client* client, SceneID* scene_current){

    // B_INFO("client time: %f", GetTime());

    SceneID current_scene_id = *scene_current;

    // Update the current scene
    if (IsKeyPressed(KEY_RIGHT)) {
        *scene_current = (SceneID)((current_scene_id + 1) % 3);
    }
    
    if (IsKeyPressed(KEY_LEFT)) {
        *scene_current = (SceneID)((current_scene_id + 2) % 3);
    
    }


    ///////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Physics stuff: temp location
    // Create floor rectangle physics body
    // PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ 0.0f, 0.0f }, (float)GetScreenWidth(), (float)GetScreenHeight(), 10.0f);
    // floor->enabled = false; // Disable body state to convert it to static (no dynamics, but collisions)

    // PhysicsBody wall = CreatePhysicsBodyRectangle((Vector2){ 0.0f, 0.0f }, 500.0f, 500.0f, 10.0f);
    // wall->enabled = false; // Disable body state to convert it to static (no dynamics, but collisions)

    // // Create dynamic physics bodies
    // PhysicsBody ball = CreatePhysicsBodyCircle((Vector2){ 100.0f, 500.0f }, 100.0f, 10.0f);
    // ball->staticFriction = 0.1f;
    // ball->dynamicFriction = 0.1f;



    return EC_OK;
}

ErrorCode update_chat(Chat* chat) {

    int key = GetCharPressed();

    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (chat->active_message.text_size < MAX_CHAT_LENGTH)) { // TODO: Magic number!
            chat->active_message.text[chat->active_message.text_size] = (char)key;
            chat->active_message.text[chat->active_message.text_size + 1] = '\0'; // Add null terminator at the end of the string.
            chat->active_message.text_size++;
            B_INFO("[%d]: '%s'", chat->active_message.text_size, chat->active_message.text);
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
        if (chat->active_message.text_size >= 1) {
            chat->active_message.text_size--;
        }
        chat->active_message.text[chat->active_message.text_size] = '\0';
        B_INFO("[%d]: '%s'", chat->active_message.text_size, chat->active_message.text);
    }

    if (IsKeyPressed(KEY_ENTER) && has_active_text(chat)) {

        B_INFO("Pushing active message to chat history");
        ErrorCode ec_push_active_message = push_active_message(chat);
        if (ec_push_active_message) {
            B_ERROR("Failed to push active message to chat history");
        }

        B_INFO("Clearing active message");
        ErrorCode ec_clear_active_message = clear_active_message(chat);
        if (ec_clear_active_message) {
            B_ERROR("Failed to clear active message");
        }
    }

    return EC_OK;
}






