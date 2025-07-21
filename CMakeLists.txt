cmake_minimum_required(VERSION 4.0)
project(Bocce C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

# Provides cross-platform sockets
FetchContent_Declare(
    tinycsocket
    GIT_REPOSITORY "https://github.com/dosshell/tinycsocket.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)
    
# Provides cross-platform file path manipulation
FetchContent_Declare(
    cwalk
    GIT_REPOSITORY "https://github.com/likle/cwalk.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

# Provides TOML parsing
FetchContent_Declare(
    libtoml
    GIT_REPOSITORY "https://github.com/brglng/libtoml.git"
    GIT_SHALLOW ON
)

# Provides TOML parsing
# FetchContent_Declare(
#     safestringlib
#     GIT_REPOSITORY "https://github.com/intel/safestringlib.git"
#     GIT_SHALLOW ON
# )

FetchContent_MakeAvailable(tinycsocket cwalk libtoml)

find_package(SQLite3 REQUIRED)

add_subdirectory(src)

target_link_libraries(Bocce toml tinycsocket sqlite3 cwalk safestring_shared)