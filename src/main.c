#include "raylib.h"
#include "Ui.h"
#include "loadTexture.h"
#include "globals.h"
#include "drawRoom.h"
#include "BuyRoom.h"
#include "mouse.h"

// Texture list
// Room name (left) must exactly match the RoomType name in globals.c.
// File path (right) is relative to the executable directory.
TextureEntry myTextures[] = {
    // --- UI ---
    {"BuyNewRoom",           "surse/Images/BuyNewRoom.png"},

    // --- Rooms ---
    {"basicRoom",            "surse/Images/Rooms/basicRoom.png"},
    {"FrontDesk",            "surse/Images/Rooms/FrontDesk.png"},
    {"kitchen",              "surse/Images/Rooms/kitchen.png"},
    {"room",                 "surse/Images/Rooms/room.png"},
    {"Restaurant",           "surse/Images/Rooms/restauramt.png"},
    {"Sauna",                "surse/Images/Rooms/sauna.png"},
    {"MovieTheater",         "surse/Images/Rooms/Movie.png"},
    {"PresidentialSuite",    "surse/Images/Rooms/PresiSuite.png"},
    {"Suites",               "surse/Images/Rooms/suite.png"},

    // --- Cards ---
    {"kitchenCard",           "surse/Images/Cards/kitchenCard.png"},
    {"PresidentialSuiteCard", "surse/Images/Cards/PresidentialSuiteCard.png"},
    {"RoomCard",              "surse/Images/Cards/RoomCard.png"},
    {"SuiteCard",             "surse/Images/Cards/SuiteCard.png"},
    {"SaunaCard",             "surse/Images/Cards/SaunaCard.png"},
    {"MovieTheaterCard",      "surse/Images/Cards/MovieTheaterCard.png"},
};

int textureCount = sizeof(myTextures) / sizeof(myTextures[0]);

int main(void)
{
    int screenWidth  = 800;
    int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "TrainBuilder");
    SetTargetFPS(60);

    // load all textures
    LoadAllTextures(myTextures, textureCount);

    // initialise hotel room list: BuyNewRoom (top) + FrontDesk (bottom)
    RoomList_Init();

    // generate the first card selection
    GenerateRoomSelection();

    while (!WindowShouldClose()) {

        // reset click-consumption guard (must be first!)
        Mouse_NewFrame();

        // ESC closes the buy menu
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