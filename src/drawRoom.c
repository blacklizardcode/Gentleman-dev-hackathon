#include "raylib.h"
#include "loadTexture.h"
#include "Ui.h"

static float cameraY = 0.0f;

typedef struct RoomDef {
    const char* name;
    int height;
    Texture2D texture;
    bool textureLoaded;
} RoomDef;

static RoomDef rooms[] = {
    {"BuyNewRoom", 100, {0}, false},
    {"", 5, {0}, false}, // spacer
    {"basicRoom", 800, {0}, false},
    {"", 5, {0}, false}, // spacer
    {"basicRoom", 800, {0}, false}, 
};

static int GetTotalWorldHeight(void) {
    int total = 0;
    for (int i = 0; i < (int)(sizeof(rooms) / sizeof(rooms[0])); i++) {
        total += rooms[i].height;
    }
    return total;
}

static void EnsureRoomTexturesLoaded(void) {
    int roomCount = (int)(sizeof(rooms) / sizeof(rooms[0]));
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].textureLoaded) continue;
        if (rooms[i].name == NULL || rooms[i].name[0] == '\0') continue;
        Image* img = GetTexture(rooms[i].name);
        if (img == NULL || img->data == NULL) continue;
        rooms[i].texture = LoadTextureFromImage(*img);
        rooms[i].textureLoaded = true;
    }
}

static void UpdateRoomCamera(float deltaTime) {
    const float speed = 400.0f;

    if (IsKeyDown(KEY_W)) cameraY -= speed * deltaTime;
    if (IsKeyDown(KEY_S)) cameraY += speed * deltaTime;

    int windowHeight = GetScreenHeight();
    int worldHeight  = GetTotalWorldHeight();

    if (windowHeight >= worldHeight) {
        cameraY = 0.0f;
        return;
    }

    if (cameraY < 0.0f) cameraY = 0.0f;
    if (cameraY > (float)(worldHeight - windowHeight))
        cameraY = (float)(worldHeight - windowHeight);
}

void DrawRoom(void) {
    EnsureRoomTexturesLoaded();

    int windowWidth  = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    int worldHeight  = GetTotalWorldHeight();

    UpdateRoomCamera(GetFrameTime());

    int yOffset = 0;
    for (int i = 0; i < (int)(sizeof(rooms) / sizeof(rooms[0])); i++) {
        RoomDef* room = &rooms[i];

        int roomScreenY = yOffset - (int)cameraY;

        if (roomScreenY + room->height < 0 || roomScreenY > windowHeight) {
            yOffset += room->height;
            continue;
        }

        if (room->textureLoaded) {
            Rectangle sourceRec = {0, 0, (float)room->texture.width, (float)room->texture.height};
            Rectangle destRec   = {0, (float)roomScreenY, (float)windowWidth, (float)room->height};
            DrawTexturePro(room->texture, sourceRec, destRec, (Vector2){0,0}, 0.0f, WHITE);

        } else if (rooms[i].name != NULL && rooms[i].name[0] != '\0') {
            DrawRectangle(0, roomScreenY, windowWidth, room->height, LIGHTGRAY);
            DrawText(TextFormat("Missing: %s", room->name), 20, roomScreenY + 20, 20, DARKGRAY);
        }

        yOffset += room->height;
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