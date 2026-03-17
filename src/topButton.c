#include "raylib.h"
#include "moove.h"
#include "globals.h"
#include "drawRoom.h"
#include "loadTexture.h"

// -------------------------------------------------------
//  Go to Top Button
//  Appears when not at the top of the world
//  Click to instantly scroll to the top
// -------------------------------------------------------

static Texture2D goUpTex = {0};
static bool textureLoaded = false;

// Load the goUp texture (lazy-loaded)
static void LoadGoUpTexture(void) {
    if (!textureLoaded) {
        Image *img = GetTexture("goUp");
        if (img && img->data) {
            goUpTex = LoadTextureFromImage(*img);
            textureLoaded = true;
        }
    }
}

// Unload the texture (call during cleanup)
void UnloadGoUpTexture(void) {
    if (textureLoaded && goUpTex.id != 0) {
        UnloadTexture(goUpTex);
        goUpTex.id = 0;
        textureLoaded = false;
    }
}

// Draw the go-to-top button if camera is not at the top
void DrawGoUpButton(void) {
    // Don't show button if buy menu is open
    if (RoomSelect) return;
    
    LoadGoUpTexture();
    
    float cameraY = Camera_GetY();
    
    // Only show button if we're not at the top (cameraY > 0)
    if (cameraY <= 0.0f) return;
    
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    
    // Button size - responsive
    int buttonSize = 60;
    int padding = 20;
    
    // Position: bottom right corner
    int buttonX = sw - buttonSize - padding;
    int buttonY = sh - buttonSize - padding;
    
    Rectangle buttonRect = {
        (float)buttonX,
        (float)buttonY,
        (float)buttonSize,
        (float)buttonSize
    };
    
    // Draw button background (semi-transparent)
    Color bgColor = {0, 0, 0, 180};
    DrawRectangleRounded(buttonRect, 0.3f, 8, bgColor);
    
    // Draw icon if available, otherwise draw arrow
    if (textureLoaded && goUpTex.id != 0) {
        // Draw the goUp texture
        Rectangle src = {0, 0, (float)goUpTex.width, (float)goUpTex.height};
        Rectangle dest = {
            buttonRect.x + 10,
            buttonRect.y + 10,
            buttonSize - 20,
            buttonSize - 20
        };
        DrawTexturePro(goUpTex, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        // Fallback: draw a simple arrow
        int arrowSize = 20;
        int centerX = buttonX + buttonSize / 2;
        int centerY = buttonY + buttonSize / 2;
        
        // Draw upward pointing triangle
        DrawTriangle(
            (Vector2){centerX, (float)(centerY - arrowSize/2)},
            (Vector2){centerX - arrowSize/2, (float)(centerY + arrowSize/2)},
            (Vector2){centerX + arrowSize/2, (float)(centerY + arrowSize/2)},
            WHITE
        );
    }
    
    // Hover effect
    Vector2 mousePos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mousePos, buttonRect);
    
    if (hover) {
        // Draw a brighter border on hover - KORRIGIERTE VERSION
        DrawRectangleRoundedLines(buttonRect, 0.3f, 8, WHITE);  // Ohne thickness Parameter
        
        // Click detection
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Scroll instantly to top
            Camera_ScrollToTop();
        }
    }
}