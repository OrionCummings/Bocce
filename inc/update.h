#ifndef __UPDATE_H
#define __UPDATE_H

#include "types.h"
#include "error_codes.h"
#include "debug.h"
#include "networking.h"

ErrorCode update(ApplicationSettings*, Server*, Client*, GameState*);
ErrorCode update_game_state(GameState*);
ErrorCode update_server(Server*);
ErrorCode update_client(Client*);

// Misc
BocceBall generate_ball(V2i mouse_position);
Color get_random_color(void);

#endif