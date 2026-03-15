#include "raylib.h"
#include <stddef.h>
#include <string.h>
#include "loadTexture.h"
#include "Ui.h"
#include "moove.h"
#include "mouse.h"
#include "globals.h"


typedef struct RoomDef {
    const char* name;
    int height;
    Texture2D texture;
    bool textureLoaded;
} RoomDef;

static bool ShouldShowRoom(const RoomDef *room) {
    // Always show the buy button or spacers.
    if (room->name == NULL || room->name[0] == '\0') return true;
    if (strcmp(room->name, "BuyNewRoom") == 0) return true;
    return IsRoomOwned(room->name);
}

static RoomDef rooms[] = {
    {"BuyNewRoom", 100, {0}, false},
    //Rume list
    {"", 5, {0}, false}, // spacer
    {"room", 800, {0}, false},
    {"basicRoom", 800, {0}, false}, 
    {"kitchen", 800, {0}, false},
    {"FrontDesk", 800, {0}, false},
    
};

static int GetRoomHeight(RoomDef* room) {
    if (room->textureLoaded && room->texture.width > 0) {
        float aspect = (float)room->texture.height / (float)room->texture.width;
        return (int)((float)GetScreenWidth() * aspect);
    }
    return room->height; 
}

static int GetTotalWorldHeight(void) {
    int total = 0;
    for (int i = 0; i < (int)(sizeof(rooms) / sizeof(rooms[0])); i++) {
        RoomDef *room = &rooms[i];
        if (!ShouldShowRoom(room)) continue;
        total += GetRoomHeight(room);
    }
    return total;
}

static void EnsureRoomTexturesLoaded(void) {
    int roomCount = (int)(sizeof(rooms) / sizeof(rooms[0]));
    for (int i = 0; i < roomCount; i++) {
        RoomDef *room = &rooms[i];
        if (!ShouldShowRoom(room)) continue;
        if (room->textureLoaded) continue;
        if (room->name == NULL || room->name[0] == '\0') continue;
        Image* img = GetTexture(room->name);
        if (img == NULL || img->data == NULL) continue;
        room->texture = LoadTextureFromImage(*img);
        room->textureLoaded = true;
    }
}

void DrawRoom(void) {
    EnsureRoomTexturesLoaded();

    int windowWidth  = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    int worldHeight  = GetTotalWorldHeight();

    Camera_Update(worldHeight, windowHeight, GetFrameTime());
    float cameraY = Camera_GetY();

    int yOffset = 0;
    for (int i = 0; i < (int)(sizeof(rooms) / sizeof(rooms[0])); i++) {
        RoomDef* room = &rooms[i];
        if (!ShouldShowRoom(room)) continue;

        int roomH = GetRoomHeight(room); 
        int roomScreenY = yOffset - (int)cameraY;

        if (roomScreenY + roomH < 0 || roomScreenY > windowHeight) {
            yOffset += roomH;
            continue;
        }

        Rectangle destRec = {0, (float)roomScreenY, (float)windowWidth, (float)roomH};
        bool hover = (room->name != NULL && room->name[0] != '\0') && IsMouseOverRect(destRec);

        if (room->textureLoaded) {
            Rectangle sourceRec = {0, 0, (float)room->texture.width, (float)room->texture.height};
            DrawTexturePro(room->texture, sourceRec, destRec, (Vector2){0,0}, 0.0f, WHITE);

        } else if (room->name != NULL && room->name[0] != '\0') {
            DrawRectangleRec(destRec, LIGHTGRAY);
            DrawText(TextFormat("Missing: %s", room->name), 20, roomScreenY + 20, 20, DARKGRAY);
        }

        if (hover) {
            DrawRectangleLinesEx(destRec, 4, WHITE);
        }

        yOffset += roomH;
    }

    int worldBottomOnScreen = worldHeight - (int)cameraY;
    if (worldBottomOnScreen < windowHeight) {
        DrawRectangle(0, worldBottomOnScreen, windowWidth, windowHeight - worldBottomOnScreen, DARKGRAY);
    }
}

void UnloadRoomTextures(void) {
    int roomCount = (int)(sizeof(rooms) / sizeof(rooms[0]));
    for (int i = 0; i < roomCount; i++) {
        if (!rooms[i].textureLoaded) continue;
        UnloadTexture(rooms[i].texture);
        rooms[i].textureLoaded = false;
        rooms[i].texture = (Texture2D){0};
    }
}