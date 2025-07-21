#ifndef __DEBUG_H
#define __DEBUG_H

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#define __ENABLE_INFO
#define __ENABLE_WARNING
#define __ENABLE_ERROR

#define __DEBUG_MESSAGE_BUFFER_SIZE 512

#define B_INFO(X, ...) __info__(X, __FILE__, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#define B_WARNING(X, ...) __warning__(X, __FILE__, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#define B_ERROR(X, ...) __error__(X, __FILE__, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

void __error__(const char* message, const char* file, const char* func, const uint64_t line, ...);
void __warning__(const char* message, const char* file, const char* func, const uint64_t line, ...);
void __info__(const char* message, const char* file, const char* func, const uint64_t line, ...);

// Binary printing
static const char* bits[16] = {
    [0] = "0000",[1] = "0001",[2] = "0010",[3] = "0011",
    [4] = "0100",[5] = "0101",[6] = "0110",[7] = "0111",
    [8] = "1000",[9] = "1001",[10] = "1010",[11] = "1011",
    [12] = "1100",[13] = "1101",[14] = "1110",[15] = "1111",
};

void print_byte(uint8_t);
void print_bytes(uint8_t*, size_t);
void print_word(uint16_t);
void print_words(uint16_t*, size_t);
void print_dword(uint32_t);
void print_dwords(uint32_t*, size_t);
void print_qword(uint64_t);
void print_qwords(uint64_t*, size_t);

#endif