#include "raylib.h"
#include "Ui.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "gentleman-dev-hackathon");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawUi();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}