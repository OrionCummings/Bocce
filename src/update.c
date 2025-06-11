#include "update.h"

ErrorCode update(ApplicationSettings* settings, Server* server, Client* client){

    ErrorCode ec_update_server = update_server(server);
    if (ec_update_server) {
        B_ERROR("Failed to update server");
        return ec_update_server;
    }

    ErrorCode ec_update_client = update_client(client);
    if (ec_update_client) {
        B_ERROR("Failed to update client");
        return ec_update_client;
    }
    
    return EC_OK;
}

ErrorCode update_server(Server* server){

    return EC_OK;
}

ErrorCode update_client(Client* client){

    return EC_OK;
}


