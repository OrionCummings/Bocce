add_executable(Bocce
    main.c
    init.c
    debug.c
    drawing.c
    config.c
    types.c
    networking.c
    ball.c
    update.c
    chat.c
    # graphics.c
    # physics.c
    # resources.c
    # data_structures.c
    # database.c
    # cli.c
    )

target_include_directories(Bocce PRIVATE "../inc")
# target_include_directories(Bocce PRIVATE ../lib/raylib)
# include_directories(SYSTEM "../lib/raylib")

target_compile_options(Bocce PRIVATE
    -pedantic 
    -Wall 
    -Wextra 
    -Wconversion 
    -Wundef 
    -Werror  
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-type-limits
    -g
)
