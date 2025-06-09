#include "networking.h"

ErrorCode send(TcsSocket client_socket, const uint8_t* buffer, size_t buffer_size) {

    size_t num_bytes_sent = 0;
    if (tcs_send(client_socket, buffer, buffer_size, TCS_MSG_SENDALL, &num_bytes_sent) != TCS_SUCCESS) {
        B_ERROR("Client failed to send data to the server");
        return EC_TCS_CLIENT_SEND_FAILURE;
    }

    printf("Sent %lld bytes to server\n", num_bytes_sent);

    return EC_OK;
}

ErrorCode receive() {
    
    return EC_OK;
}