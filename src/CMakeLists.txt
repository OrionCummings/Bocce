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
    # graphics.c
    # physics.c
    # resources.c
    # data_structures.c
    # database.c
    # cli.c
    )

target_include_directories(Bocce PRIVATE ../inc)

target_compile_options(Bocce PRIVATE
    -pedantic 
    -Wall 
    -Wextra 
    -Wconversion 
    -Wundef 
    -Werror  
    -Wno-unused-parameter
    -Wno-unused-variable
    -g
)
