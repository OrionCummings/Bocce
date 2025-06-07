// This removes a lot of bloat from windows header files
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "debug.h"

int main(int argc, char** argv) {

	B_INFO("C Version: %d", __STDC_VERSION__);

	return 0;
}