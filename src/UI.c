#include "raylib.h"
#include "Ui.h"
#include "loadTexture.h"
#include "globals.h"
#include "gamelogic.h"
#include <math.h>

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
//  Level-Up animation state
// -------------------------------------------------------

// Total duration of the animation in seconds.
// Phase 0: pop in  (0 .. ANIM_POP_END)
// Phase 1: hold    (ANIM_POP_END .. ANIM_HOLD_END)
// Phase 2: shrink  (ANIM_HOLD_END .. ANIM_TOTAL)
#define ANIM_TOTAL     2.2f
#define ANIM_POP_END   0.25f
#define ANIM_HOLD_END  1.6f

static float animTimer    = 0.0f;   // counts up from 0 while animating
static bool  animRunning  = false;
static int   animLevel    = 0;      // level number to display during animation

// Called by DrawUi every frame to advance the animation clock.
static void TickLevelUpAnim(float dt) {
    if (!animRunning) {
        // Check if a new level-up just happened
        if (GameLogic_JustLeveledUp()) {
            animRunning = true;
            animTimer   = 0.0f;
            animLevel   = levl;
        }
        return;
    }
    animTimer += dt;
    if (animTimer >= ANIM_TOTAL) {
        animRunning = false;
        animTimer   = 0.0f;
    }
}

// Easing: smoothstep for a natural feel
static float SmoothStep(float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return t * t * (3.0f - 2.0f * t);
}

// Draws the level-up pop animation.
static void DrawLevelUpAnim(int sw, int sh) {
    if (!animRunning) return;

    float t = animTimer;
    float scale = 0.0f;
    float alpha = 1.0f;

    if (t < ANIM_POP_END) {
        // Pop in with a slight overshoot (scale goes 0 -> 1.15 -> 1.0)
        float p = t / ANIM_POP_END;           // 0..1
        float s = SmoothStep(p);
        // overshoot: peaks at ~1.15 halfway through pop
        scale = s * (1.0f + 0.15f * sinf(s * 3.14159f));
    } else if (t < ANIM_HOLD_END) {
        // Hold at full size
        scale = 1.0f;
    } else {
        // Shrink out
        float p = (t - ANIM_HOLD_END) / (ANIM_TOTAL - ANIM_HOLD_END);  // 0..1
        scale = 1.0f - SmoothStep(p);
        alpha = 1.0f - SmoothStep(p);
    }

    if (scale <= 0.0f) return;

    // --- Sizes ---
    float iconSize  = 120.0f * scale;
    float fontSize  = 44.0f * scale;
    if (fontSize < 1.0f) fontSize = 1.0f;
    int   fontSizeI = (int)fontSize;

    // --- Position: centred on screen ---
    float cx = sw / 2.0f;
    float cy = sh / 2.0f - 20.0f;

    // Dim backdrop
    unsigned char a = (unsigned char)(160.0f * alpha);
    float backdropW = iconSize + 200.0f * scale;
    float backdropH = iconSize + fontSizeI + 30.0f * scale;
    DrawRectangle(
        (int)(cx - backdropW / 2.0f),
        (int)(cy - iconSize / 2.0f - 10.0f * scale),
        (int)backdropW,
        (int)backdropH,
        (Color){0, 0, 0, a}
    );

    // Level icon (levlTex), centred above text
    if (levlTex.id != 0) {
        Rectangle src  = {0, 0, (float)levlTex.width, (float)levlTex.height};
        Rectangle dest = {
            cx - iconSize / 2.0f,
            cy - iconSize / 2.0f,
            iconSize,
            iconSize
        };
        unsigned char ca = (unsigned char)(255.0f * alpha);
        DrawTexturePro(levlTex, src, dest, (Vector2){0, 0}, 0.0f, (Color){ca, ca, ca, ca});
    } else {
        // Fallback: coloured square
        unsigned char ca = (unsigned char)(255.0f * alpha);
        DrawRectangle(
            (int)(cx - iconSize / 2.0f),
            (int)(cy - iconSize / 2.0f),
            (int)iconSize, (int)iconSize,
            (Color){80, 220, 80, ca}
        );
    }

    // "LEVEL UP!" text below icon
    const char *line1 = "LEVEL UP!";
    const char *line2 = TextFormat("Now Level %d", animLevel);

    unsigned char ta = (unsigned char)(255.0f * alpha);
    Color goldA  = {255, 200,  40, ta};
    Color whiteA = {255, 255, 255, ta};

    int w1 = MeasureText(line1, fontSizeI);
    int w2 = MeasureText(line2, (int)(fontSizeI * 0.7f));

    int y1 = (int)(cy + iconSize / 2.0f + 6.0f * scale);
    int y2 = y1 + fontSizeI + 2;

    // Gold shadow + white
    DrawText(line1, (int)(cx - w1 / 2.0f) + 2, y1 + 2, fontSizeI, goldA);
    DrawText(line1, (int)(cx - w1 / 2.0f),     y1,     fontSizeI, whiteA);

    int fs2 = (int)(fontSizeI * 0.7f);
    DrawText(line2, (int)(cx - w2 / 2.0f), y2, fs2, whiteA);
}

// -------------------------------------------------------
//  DrawUi
// -------------------------------------------------------

void DrawUi(void) {
    EnsureUiTextures();

    float dt = GetFrameTime();
    TickLevelUpAnim(dt);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Layout constants
    const int iconSize  = 32;
    const int padX      = 12;
    const int padY      = 10;
    const int spacing   = 110;
    const int textOffX  = iconSize + 6;
    const int fontSize  = 20;
    const int barH      = 6;

    int topBarH = iconSize + padY * 2;

    // --- Background bar ---
    DrawRectangle(0, 0, sw, topBarH, (Color){0, 0, 0, 160});

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

    // -------------------------------------------------------
    //  XP bar — sits right below the top bar, full width
    // -------------------------------------------------------
    {
        int barY = topBarH;
        DrawRectangle(0, barY, sw, barH, (Color){40, 40, 40, 220});

        float ratio = (xpForNextLevel > 0)
                      ? (float)xp / (float)xpForNextLevel
                      : 0.0f;
        if (ratio > 1.0f) ratio = 1.0f;
        int filledW = (int)(sw * ratio);
        DrawRectangle(0, barY, filledW, barH, (Color){80, 220, 80, 255});

    }

    // -------------------------------------------------------
    //  Level-Up pop animation (drawn on top of everything)
    // -------------------------------------------------------
    DrawLevelUpAnim(sw, sh);
}

// -------------------------------------------------------
//  Cleanup
// -------------------------------------------------------

void UnloadUiTextures(void) {
    if (coinTex.id   != 0) { UnloadTexture(coinTex);   coinTex.id   = 0; }
    if (guestsTex.id != 0) { UnloadTexture(guestsTex); guestsTex.id = 0; }
    if (levlTex.id   != 0) { UnloadTexture(levlTex);   levlTex.id   = 0; }
}