#ifndef __FILE_SYSTEM_H
#define __FILE_SYSTEM_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// To support getcwd()
#if defined(_MSC_VER)
#include <direct.h>
#define getcwd _getcwd
#elif defined(__GNUC__)
#include <unistd.h>
#endif

#include "safe_lib.h"
#include "cwalk.h"
#include "debug.h"
#include "error_codes.h"

ErrorCode get_project_dir(char**);

#endif