#ifndef __NETWORKING_H
#define __NETWORKING_H

#include "tinycsocket.h"
#include "debug.h"
#include "types.h"
#include "error_codes.h"

ErrorCode send(TcsSocket client_socket, const uint8_t* buffer, size_t buffer_size);
ErrorCode receive();

#endif