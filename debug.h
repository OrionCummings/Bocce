#pragma once
#ifndef __DEBUG_H
#define __DEBUG_H

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define __ENABLE_INFO
#define __ENABLE_WARNING
#define __ENABLE_ERROR

#define MESSAGE_BUFFER_SIZE 512

#define INFO(X, ...) \
	 __info__(X, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

#define WARNING(X, ...) \
	 __warning__(X, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

#define ERROR(X, ...) \
	 __error__(X, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

inline void __error__(const char* message, const char* func, const uint64_t line, ...) {
#if defined __ENABLE_ERROR
	va_list args;
	va_start(args, line);

	char buffer[MESSAGE_BUFFER_SIZE] = { 0 };
	int res = snprintf(buffer, MESSAGE_BUFFER_SIZE, "[ERROR] In function '%s' on line %" PRIu64 ": \"%s\"\n", func, line, message);
	if (res > MESSAGE_BUFFER_SIZE) {
		printf("[META] The following error message was longer than %d characters and has been truncated.\n", MESSAGE_BUFFER_SIZE);
	}
	else if (res == -1) {
		printf("[META] The following error message contains an encoding error.\n");
	}
	vprintf(buffer, args);

	va_end(args);

#endif
	exit(1);
}

inline void __warning__(const char* message, const char* func, const uint64_t line, ...) {
#if defined __ENABLE_WARNING
	va_list args;
	va_start(args, line);

	char buffer[MESSAGE_BUFFER_SIZE] = { 0 };
	int res = snprintf(buffer, MESSAGE_BUFFER_SIZE, "[WARNING] In function '%s' on line %" PRIu64 ": \"%s\"\n", func, line, message);
	if (res > MESSAGE_BUFFER_SIZE) {
		printf("[META] The following warning message was longer than %d characters and has been truncated.\n", MESSAGE_BUFFER_SIZE);
	}
	else if (res == -1) {
		printf("[META] The following warning message contains an encoding error.\n");
	}
	vprintf(buffer, args);

	va_end(args);
#endif
}

inline void __info__(const char* message, const char* func, const uint64_t line, ...) {
#if defined __ENABLE_INFO && !defined __ENABLE_TESTS
	va_list args;
	va_start(args, line);

	char buffer[MESSAGE_BUFFER_SIZE] = { 0 };
	int res = snprintf(buffer, MESSAGE_BUFFER_SIZE, "[INFO] In function '%s' on line %" PRIu64 ": \"%s\"\n", func, line, message);
	if (res > MESSAGE_BUFFER_SIZE) {
		printf("[META] The following info message was longer than %d characters and has been truncated.\n", MESSAGE_BUFFER_SIZE);
	}
	else if (res == -1) {
		printf("[META] The following info message contains an encoding error.\n");
	}
	vprintf(buffer, args);

	va_end(args);
#endif
}

inline void __test__(const char* file, const char* func, const uint64_t line) {
#ifdef __ENABLE_TESTS
	printf("[TEST] Testing '%s' on line %" PRIu64 " of %s\n", func, line, file);
#endif
}

inline void __header__(const char* title) {
#ifdef __ENABLE_TESTS

	/**
	* NOTE: The following code includes magic numbers. These number are the ASCII
	* character codes for a variety of special formatting characters. In my
	* opinion, defining variables to hold these character codes would not 
	* significantly enhance the readability and scalability of this code.
	* The __header__ function is not something to be altered or even looked
	* at often, so I find it acceptable to use some less-than-ideal programming
	* practices.
	*/
	if (title == NULL) {
		ERROR("Title pointer was NULL!");
	}
	else {
		size_t len = strlen(title);
		printf("%c", 201);
		for (uint16_t i = 0; i < len + 2; i++) {
			printf("%c", 205);
		}
		printf("%c\n%c %s %c\n%c", 187, 186, title, 186, 200);
		for (uint16_t i = 0; i < len + 2; i++) {
			printf("%c", 205);
		}
		printf("%c\n", 188);
	}

#endif
}

#endif