#include "debug.h"

void __error__(const char* message, const char* file, const char* func, const uint64_t line, ...) {
#ifdef __ENABLE_ERROR
	va_list args;
	va_start(args, line);

	char buffer[__DEBUG_MESSAGE_BUFFER_SIZE] = { 0 };
	int res = snprintf(buffer, __DEBUG_MESSAGE_BUFFER_SIZE, "[ERROR] %s:%s():%" PRIu64 ": \"%s\"\n", file, func, line, message);
	if (res > __DEBUG_MESSAGE_BUFFER_SIZE) {
		printf("META: The following error message was longer than %d characters and has been truncated.\n", __DEBUG_MESSAGE_BUFFER_SIZE);
	}
	else if (res == -1) {
		printf("META: The following error message contains an encoding error.\n");
	}
	vprintf(buffer, args);

	va_end(args);

#endif
}

void __warning__(const char* message, const char* file, const char* func, const uint64_t line, ...) {
#ifdef __ENABLE_WARNING
	va_list args;
	va_start(args, line);

	char buffer[__DEBUG_MESSAGE_BUFFER_SIZE] = { 0 };
	int res = snprintf(buffer, __DEBUG_MESSAGE_BUFFER_SIZE, "[WARNING] %s:%s():%" PRIu64 ": \"%s\"\n", file, func, line, message);
	if (res > __DEBUG_MESSAGE_BUFFER_SIZE) {
		printf("META: The following warning message was longer than %d characters and has been truncated.\n", __DEBUG_MESSAGE_BUFFER_SIZE);
	}
	else if (res == -1) {
		printf("META: The following warning message contains an encoding error.\n");
	}
	vprintf(buffer, args);

	va_end(args);
#endif
}

void __info__(const char* message, const char* file, const char* func, const uint64_t line, ...) {
#ifdef __ENABLE_INFO
	va_list args;
	va_start(args, line);

	char buffer[__DEBUG_MESSAGE_BUFFER_SIZE] = { 0 };
	int res = snprintf(buffer, __DEBUG_MESSAGE_BUFFER_SIZE, "[INFO] %s:%s():%" PRIu64 ": \"%s\"\n", file, func, line, message);
	if (res > __DEBUG_MESSAGE_BUFFER_SIZE) {
		printf("META: The following info message was longer than %d characters and has been truncated.\n", __DEBUG_MESSAGE_BUFFER_SIZE);
	}
	else if (res == -1) {
		printf("META: The following info message contains an encoding error.\n");
	}
	vprintf(buffer, args);

	va_end(args);
#endif
}