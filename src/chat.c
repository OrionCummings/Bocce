#include "chat.h"

ErrorCode add_char_to_pre_buffer(Chat* chat, const char c) {

    if (chat->pre_buffer.text_index < 128) { // TODO: Magic number!
        chat->pre_buffer.text[chat->pre_buffer.text_index++] = c;
    } else {
        B_ERROR("Failed to add new character '%c' to prebuffer", c);
    }

    return EC_OK;
}

void format_chat_message(char* buffer, ChatMessage message) {

    const size_t n = 128;
    // char copy[n]; // TODO: Magic number!
    // memcpy_s(copy, n, buffer, n);

    // TODO: Check return value!
    int num_chars = snprintf(buffer, n, "[%d]: %s", message.userId, message.text);
    
    // B_ERROR("Failed to format chat message");
}

