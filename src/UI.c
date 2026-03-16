#include "raylib.h"
#include "Ui.h"
#include "loadTexture.h"
#include "globals.h"

// -------------------------------------------------------
//  UI-Icon Textures (lazy-loaded, never unloaded manually)
// -------------------------------------------------------

static Texture2D coinTex   = {0};
static Texture2D guestsTex = {0};
static Texture2D levlTex   = {0};

static void EnsureUiTextures(void) {
    if (coinTex.id == 0) {
        Image *img = GetTexture("coin");
        if (img && img->data) coinTex = LoadTextureFromImage(*img);
    }
    if (guestsTex.id == 0) {
        Image *img = GetTexture("guests");
        if (img && img->data) guestsTex = LoadTextureFromImage(*img);
    }
    if (levlTex.id == 0) {
        Image *img = GetTexture("levl");
        if (img && img->data) levlTex = LoadTextureFromImage(*img);
    }
}

// -------------------------------------------------------
//  DrawUi
// -------------------------------------------------------

void DrawUi(void) {
    EnsureUiTextures();

    int sw = GetScreenWidth();

    // Layout constants
    const int iconSize  = 32;   // square icon height/width
    const int padX      = 12;   // left margin
    const int padY      = 10;   // top margin
    const int spacing   = 110;  // distance between each icon+text group
    const int textOffX  = iconSize + 6;  // text starts this many px right of icon
    const int fontSize  = 20;

    // --- Background bar ---
    DrawRectangle(0, 0, sw, iconSize + padY * 2, (Color){0, 0, 0, 160});

    // --- Helper lambda (C-style): draw one icon + number ---
    // Group layout: [icon][value]
    // We draw three groups side by side.

    // 1) Coin / money
    {
        int gx = padX;
        int gy = padY;
        if (coinTex.id != 0) {
            Rectangle src  = {0, 0, (float)coinTex.width, (float)coinTex.height};
            Rectangle dest = {(float)gx, (float)gy, (float)iconSize, (float)iconSize};
            DrawTexturePro(coinTex, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        } else {
            DrawRectangle(gx, gy, iconSize, iconSize, GOLD);
        }
        DrawText(TextFormat("%d", monney), gx + textOffX, gy + (iconSize - fontSize) / 2, fontSize, WHITE);
    }

    // 2) Guests
    {
        int gx = padX + spacing;
        int gy = padY;
        if (guestsTex.id != 0) {
            Rectangle src  = {0, 0, (float)guestsTex.width, (float)guestsTex.height};
            Rectangle dest = {(float)gx, (float)gy, (float)iconSize, (float)iconSize};
            DrawTexturePro(guestsTex, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        } else {
            DrawRectangle(gx, gy, iconSize, iconSize, SKYBLUE);
        }
        DrawText(TextFormat("%d", guests), gx + textOffX, gy + (iconSize - fontSize) / 2, fontSize, WHITE);
    }

    // 3) Level
    {
        int gx = padX + spacing * 2;
        int gy = padY;
        if (levlTex.id != 0) {
            Rectangle src  = {0, 0, (float)levlTex.width, (float)levlTex.height};
            Rectangle dest = {(float)gx, (float)gy, (float)iconSize, (float)iconSize};
            DrawTexturePro(levlTex, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        } else {
            DrawRectangle(gx, gy, iconSize, iconSize, GREEN);
        }
        DrawText(TextFormat("Lv %d", levl), gx + textOffX, gy + (iconSize - fontSize) / 2, fontSize, WHITE);
    }
}

// -------------------------------------------------------
//  Cleanup
// -------------------------------------------------------

void UnloadUiTextures(void) {
    if (coinTex.id   != 0) { UnloadTexture(coinTex);   coinTex.id   = 0; }
    if (guestsTex.id != 0) { UnloadTexture(guestsTex); guestsTex.id = 0; }
    if (levlTex.id   != 0) { UnloadTexture(levlTex);   levlTex.id   = 0; }
}