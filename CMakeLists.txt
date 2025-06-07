cmake_minimum_required(VERSION 4.0)
project(Bocce C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(src)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

# The main library for graphics, vector math, audio, and rendering
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

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

FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(tinycsocket)
FetchContent_MakeAvailable(cwalk)