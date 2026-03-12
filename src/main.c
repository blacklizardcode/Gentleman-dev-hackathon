#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "gentleman-dev-hackathon");
    InitWindow(screenWidth, screenHeight, "gentleman-dev-hackathon");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("raylib is working", 260, 210, 30, DARKGRAY);
        DrawText("raylib is working", 260, 210, 30, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}