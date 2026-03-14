#include "raylib.h"
#include "Ui.h"
#include "loadTexture.h"

//texture list
TextureEntry myTextures[] = {
    //example
    {"basicRoom", "surse/Images/basicRoom.png"}
};


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Summer game");
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