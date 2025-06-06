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

#define __DEBUG_MESSAGE_BUFFER_SIZE 512

#define B_INFO(X, ...) __info__(X, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#define B_WARNING(X, ...) __warning__(X, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#define B_ERROR(X, ...) __error__(X, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

void __error__(const char* message, const char* func, const uint64_t line, ...);
void __warning__(const char* message, const char* func, const uint64_t line, ...);
void __info__(const char* message, const char* func, const uint64_t line, ...);

#endif