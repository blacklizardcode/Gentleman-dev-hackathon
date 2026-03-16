#include "raylib.h"
#include <stddef.h>
#include <string.h>
#include "loadTexture.h"
#include "Ui.h"
#include "moove.h"
#include "mouse.h"
#include "globals.h"
#include "BuyRoom.h"

// -----------------------------------------------------------
//  Texture cache: one slot per unique room-type name
// -----------------------------------------------------------

typedef struct {
    const char *name;
    Texture2D   texture;
    bool        loaded;      // true  = GPU texture ready
    bool        attempted;   // true  = lookup tried at least once (even if failed)
} RoomTexCache;

#define MAX_TEX_CACHE 64
static RoomTexCache texCache[MAX_TEX_CACHE];
static int          texCacheCount = 0;

static RoomTexCache *FindCacheSlot(const char *name) {
    for (int i = 0; i < texCacheCount; i++) {
        if (strcmp(texCache[i].name, name) == 0)
            return &texCache[i];
    }
    return NULL;
}

static Texture2D *GetCachedTexture(const char *name) {
    RoomTexCache *slot = FindCacheSlot(name);
    if (slot && slot->loaded) return &slot->texture;
    return NULL;
}

static Texture2D *LoadAndCache(const char *name) {
    RoomTexCache *slot = FindCacheSlot(name);

    // Already tried and failed -- do not call GetTexture every frame.
    if (slot && slot->attempted && !slot->loaded) return NULL;

    // Already loaded successfully.
    if (slot && slot->loaded) return &slot->texture;

    // New entry -- register the slot first so we mark it as attempted.
    if (texCacheCount >= MAX_TEX_CACHE) return NULL;
    slot = &texCache[texCacheCount++];
    slot->name      = name;
    slot->loaded    = false;
    slot->attempted = true;

    Image *img = GetTexture(name);
    if (img == NULL || img->data == NULL) return NULL;

    slot->texture = LoadTextureFromImage(*img);
    slot->loaded  = true;
    return &slot->texture;
}

// -----------------------------------------------------------
//  Helper functions
// -----------------------------------------------------------

static int GetRoomH(const char *name) {
    Texture2D *tex = GetCachedTexture(name);
    if (tex && tex->width > 0) {
        float aspect = (float)tex->height / (float)tex->width;
        return (int)((float)GetScreenWidth() * aspect);
    }
    return 800; // fallback
}

static int GetTotalWorldHeight(void) {
    int total = 0;
    for (int i = 0; i < roomListCount; i++) {
        const char *name = roomList[i].name;
        if (name == NULL) continue;
        if (name[0] == '\0') { total += 5; continue; } // spacer entry
        total += GetRoomH(name);
    }
    return total;
}

static void EnsureAllTexturesLoaded(void) {
    for (int i = 0; i < roomListCount; i++) {
        const char *name = roomList[i].name;
        if (name == NULL || name[0] == '\0') continue;
        LoadAndCache(name);
    }
}

// -----------------------------------------------------------
//  DrawRoom
// -----------------------------------------------------------

void DrawRoom(void) {
    EnsureAllTexturesLoaded();

    int windowWidth  = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    int worldHeight  = GetTotalWorldHeight();

    Camera_Update(worldHeight, windowHeight, GetFrameTime());
    float cameraY = Camera_GetY();

    int yOffset = 0;
    for (int i = 0; i < roomListCount; i++) {
        const char *name = roomList[i].name;
        if (name == NULL) continue;

        // Spacer
        if (name[0] == '\0') { yOffset += 5; continue; }

        int roomH       = GetRoomH(name);
        int roomScreenY = yOffset - (int)cameraY;

        // Culling
        if (roomScreenY + roomH < 0 || roomScreenY > windowHeight) {
            yOffset += roomH;
            continue;
        }

        Rectangle destRec = {0, (float)roomScreenY, (float)windowWidth, (float)roomH};
        Texture2D *tex    = GetCachedTexture(name);

        bool hover = IsRoomHovered(destRec);

        if (tex && tex->id != 0) {
            Rectangle srcRec = {0, 0, (float)tex->width, (float)tex->height};
            DrawTexturePro(*tex, srcRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        } else {
            DrawRectangleRec(destRec, LIGHTGRAY);
            DrawText(TextFormat("Missing: %s", name), 20, roomScreenY + 20, 20, DARKGRAY);
        }

        if (hover) {
            DrawRectangleLinesEx(destRec, 4, WHITE);

            // clicking "BuyNewRoom" opens the buy menu
            if (strcmp(name, "BuyNewRoom") == 0 && IsRoomClicked(destRec)) {
                RoomSelect = true;
            }
        }

        yOffset += roomH;
    }

    // fill background below the world
    int worldBottomOnScreen = worldHeight - (int)cameraY;
    if (worldBottomOnScreen < windowHeight) {
        DrawRectangle(0, worldBottomOnScreen, windowWidth,
                      windowHeight - worldBottomOnScreen, DARKGRAY);
    }
}

// -----------------------------------------------------------
//  Cleanup / unload
// -----------------------------------------------------------

void UnloadRoomTextures(void) {
    for (int i = 0; i < texCacheCount; i++) {
        if (texCache[i].loaded) {
            UnloadTexture(texCache[i].texture);
            texCache[i].loaded = false;
        }
    }
    texCacheCount = 0;
}