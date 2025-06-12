#ifndef __CHAT_H
#define __CHAT_H

#include <stdio.h>
#include <inttypes.h>
#include "debug.h"
#include "error_codes.h"

typedef struct ChatMessage {
    char text[128];
    uint8_t text_index;
    const uint32_t userId;
} ChatMessage;

typedef struct ChatHistory {
    uint16_t num_messages;
    ChatMessage* messages;
} ChatHistory;

typedef struct Chat {
    ChatHistory history;
    ChatMessage pre_buffer;
    uint8_t pre_buffer_index;
} Chat;

ErrorCode add_char_to_pre_buffer(Chat*, const char);
// ChatMessage pre_buffer_to_chat_message(const char[static 128]);

// Misc
void format_chat_message(char*, ChatMessage);

#endif