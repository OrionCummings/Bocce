#ifndef __UPDATE_H
#define __UPDATE_H

#include "types.h"
#include "error_codes.h"
#include "debug.h"
#include "networking.h"
#include "raylib.h"
#include "chat.h"
#include "physac.h"
#include "scene.h"

ErrorCode update(ApplicationSettings*, Server*, Client*, GameState*, Chat*, SceneID*);
ErrorCode update_game_state(GameState*);
ErrorCode update_server(Server*);
ErrorCode update_client(Client*, SceneID*);
ErrorCode update_chat(Chat*);

// Misc
Ball generate_ball(Vector2 mouse_position);
Color get_random_color(void);

#endif