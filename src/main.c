#include "raylib.h"
#include "Ui.h"
#include "loadTexture.h"
#include "drawRoom.h"

//texture list
TextureEntry myTextures[] = {
    {"basicRoom", "surse/Images/basicRoom.png"}
};

int textureCount = sizeof(myTextures) / sizeof(myTextures[0]);

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Summer game");
    SetTargetFPS(60);

    //load Textures ones
    LoadAllTextures(myTextures, textureCount);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // begin drawing
        DrawRoom();

        DrawUi();
        EndDrawing();
    }
    //unload images
    UnLoadTextures();
    CloseWindow();
    return 0;
}