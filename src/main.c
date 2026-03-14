#include "raylib.h"
#include "Ui.h"
#include "loadTexture.h"
#include "drawRoom.h"

//texture list
TextureEntry myTextures[] = {
    {"basicRoom", "surse/Images/Rooms/basicRoom.png"},
    {"BuyNewRoom", "surse/Images/BuyNewRoom.png"},
    {"FrontDesk", "surse/Images/Rooms/FrontDesk.png"}, 
    {"kitchen", "surse/Images/Rooms/kitchen.png"}, 
    {"room", "surse/Images/Rooms/room.png"}
};

int textureCount = sizeof(myTextures) / sizeof(myTextures[0]);

int main(void)
{
    int screenWidth = 800;
    int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "TrainBuilder");
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
    UnloadRoomTextures();

    UnLoadTextures();
    CloseWindow();
    return 0;
}