#include "raylib.h"
#include "Ui.h"
#include "loadTexture.h"
#include "globals.h"
#include "drawRoom.h"
#include "BuyRoom.h"

// Texture list
TextureEntry myTextures[] = {
    {"basicRoom",           "surse/Images/Rooms/basicRoom.png"},
    {"BuyNewRoom",          "surse/Images/BuyNewRoom.png"},
    {"FrontDesk",           "surse/Images/Rooms/FrontDesk.png"},
    {"kitchen",             "surse/Images/Rooms/kitchen.png"},
    {"room",                "surse/Images/Rooms/room.png"},
    // Card images
    {"kitchenCard",         "surse/Images/Cards/kitchenCard.png"},
    {"PresidentialSuiteCard","surse/Images/Cards/PresidentialSuiteCard.png"},
    {"RoomCard",            "surse/Images/Cards/RoomCard.png"},
    {"SuiteCard",           "surse/Images/Cards/SuiteCard.png"},
};

int textureCount = sizeof(myTextures) / sizeof(myTextures[0]);

int main(void)
{
    int screenWidth  = 800;
    int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "TrainBuilder");
    SetTargetFPS(60);

    // Load all textures once
    LoadAllTextures(myTextures, textureCount);

    // Generate the initial room selection right at startup
    GenerateRoomSelection();

    while (!WindowShouldClose()) {

        // --- ESC closes the buy menu ---
        if (RoomSelect && IsKeyPressed(KEY_ESCAPE)) {
            RoomSelect = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRoom();
        DrawUi();

        if (RoomSelect) {
            DrawBuyRoomUi();
        }

        EndDrawing();
    }

    UnloadRoomTextures();
    UnLoadTextures();
    CloseWindow();
    return 0;
}