#include "chat.h"

ChatMessage get_message(const Chat* chat, const uint16_t index) {
    if (chat == NULL) {
        B_ERROR("Null parameter passed!");
    }

    return chat->history.messages[index];
}

ErrorCode add_char_to_active_message(Chat* chat, const char c) {

    if (chat->active_message.text_size < MAX_CHAT_LENGTH) { // TODO: Magic number!
        chat->active_message.text[chat->active_message.text_size++ % MAX_CHAT_LENGTH] = c;
    } else {
        B_ERROR("Failed to add new character '%c' to active text buffer", c);
    }

    return EC_OK;
}

ErrorCode push_active_message(Chat* chat) {
    if (chat->history.num_messages >= MAX_CHAT_HISTORY_DEPTH) {
        B_ERROR("Chat history is full");
        return EC_CHAT_MAX_CHAT_DEPTH_EXCEEDED;
    }
    strcpy_s(chat->history.messages[chat->history.num_messages++].text, sizeof(*chat->active_message.text) * MAX_CHAT_LENGTH, chat->active_message.text);
    
    return EC_OK;
}

ErrorCode clear_active_message(Chat* chat) {
    memset(chat->active_message.text, 0, MAX_CHAT_LENGTH);
    chat->active_message.text_size = 0;
    return EC_OK;
}

ErrorCode serialize(const Chat* chat, byte* bytes, size_t n) {

    // TODO: Implement

    return EC_OK;
}

bool has_active_text(Chat* chat) {
    return chat->active_message.text_size > 0;
}

void format_chat_message(char* buffer, ChatMessage message) {
    const size_t n = 128;
    int num_chars = snprintf(buffer, n, "[%d]: %s", message.user_id, message.text);
}

