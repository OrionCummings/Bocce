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
    timer.c
    file_system.c
    )

target_include_directories(Bocce PRIVATE "../inc")

target_compile_options(Bocce PRIVATE
    -pedantic 
    -Wall 
    -Wextra 
    -Werror  
    -Wconversion 
    -Wundef 
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-type-limits
    -g
)
