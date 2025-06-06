cmake_minimum_required(VERSION 4.0)
project(Bocce C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(src)

target_include_directories(Bocce PUBLIC inc)

# target_compile_options(Bocce PRIVATE
#     $<$<CONFIG:Debug>:
#         -fsanitize=address,leak,undefined 
#         -pedantic 
#         -Wall 
#         -Wextra 
#         -Wconversion 
#         -Wundef 
#         -Werror  
#         -Wno-unused-parameter
#         -Wno-unused-variable
#     >
# )

# target_link_options(Bocce PRIVATE
#     $<$<CONFIG:Debug>:
#         -fsanitize=address,leak,undefined
#     >
# )

# add_library(debug INTERFACE)
# target_include_directories(debug INTERFACE inc)
# target_link_libraries(Bocce PUBLIC debug)

# include_directories(inc)
