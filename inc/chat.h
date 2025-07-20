#ifndef __CHAT_H
#define __CHAT_H

#include <stdio.h>
#include <safe_lib.h>
#include <inttypes.h>
#include "debug.h"
#include "types.h"
#include "error_codes.h"

#define MAX_CHAT_LENGTH (256)
#define MAX_CHAT_HISTORY_DEPTH (512)

typedef struct ChatMessage {
    char text[MAX_CHAT_LENGTH];
    uint8_t text_size;
    const uint32_t user_id;
} ChatMessage;

typedef struct ChatHistory {
    uint16_t num_messages;
    ChatMessage messages[MAX_CHAT_HISTORY_DEPTH];
} ChatHistory;

typedef struct Chat {
    ChatHistory history;
    ChatMessage active_message;
} Chat;

ChatMessage get_message(const Chat*, const uint16_t);
ErrorCode add_char_to_active_message(Chat*, const char);
ErrorCode push_active_message(Chat*);
ErrorCode clear_active_message(Chat*);
bool has_active_text(Chat*);

ErrorCode serialize(const Chat*, byte*, size_t);

// Misc
void format_chat_message(char*, ChatMessage);

#endif