#ifndef __UPDATE_H
#define __UPDATE_H

#include "types.h"
#include "error_codes.h"
#include "debug.h"
#include "networking.h"

ErrorCode update(ApplicationSettings* settings, Server* server, Client* client);
ErrorCode update_server(Server* server);
ErrorCode update_client(Client* client);

#endif