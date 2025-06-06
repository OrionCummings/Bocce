// This removes a lot of bloat from windows header files
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "debug.h"
#include "networking.h"

int main(int argc, char** argv) {

	init_winsock();
	init_client("127.0.0.1", "37849");
	init_server("37849");
	

	



	return 0;
}