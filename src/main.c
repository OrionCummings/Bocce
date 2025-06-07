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
#include "raylib.h"

#define WINDOW_WIDTH (1920)
#define WINDOW_HEIGHT (1080)
#define COLOR_BACKGROUND ((Color){127, 127, 150, 255})
#define DELTA_T (0.01)

static double dt = (double)0.0;

Color dim_color(const Color C, float Factor){
    if (Factor < 0.0f || Factor > 1.0f) return C;
    Color R = C;
    R.a = (unsigned char)((float)C.a * Factor);
    return R;
}

void draw_background() {
    ClearBackground(COLOR_BACKGROUND);
}

void draw_circle_outline(int X, int Y, float R, Color C) {
    DrawCircle(X, Y, R, dim_color(C, 0.3f));
    DrawCircleLines(X, Y, R, C);
    DrawCircleLines(X, Y, R * 0.99f, C);
}

int setup(void){

    // Initialize the window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bocce");

    // Set window properties
    SetTraceLogLevel(LOG_ERROR);
    SetTargetFPS(120);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    return 0;
}

int draw(void){

    // Draw the background
    draw_background();

    draw_circle_outline(0, 0, 40, (Color){255, 0, 255, 70});

    return 0;
}

int update(void){
    dt += DELTA_T;
    return 0;
}

int main(int argc, char** argv) {

    B_INFO("Starting Bocce");
    B_INFO("C Version: %d", __STDC_VERSION__);

    if (setup()) {
        B_ERROR("Setup failed!");
    }

    while (!WindowShouldClose())
    {
        if (update()) B_ERROR("Update failed");

        BeginDrawing();

        if (draw()) B_ERROR("Draw failed");

        EndDrawing();
    }

    CloseWindow();

    B_INFO("Exiting program\n");

    return 0;
}