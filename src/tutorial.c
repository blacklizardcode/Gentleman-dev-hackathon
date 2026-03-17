#include "raylib.h"
#include "tutorial.h"
#include <stdio.h>

static bool tutorialAktiv = true;
static int schritt = 0;
static float blinkTimer = 0.0f;

bool Tutorial_IsFinished(void) {
    return !tutorialAktiv;
}

void Tutorial_Init(void) {
    tutorialAktiv = true;
    schritt = 0;
    printf("Tutorial started\n");
}

void Tutorial_Skip(void) {
    tutorialAktiv = false;
    printf("Tutorial skipped\n");
}

void Tutorial_Update(void) {
    if (!tutorialAktiv) return;
    
    blinkTimer += GetFrameTime();
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        schritt++;
        if (schritt >= 4) {  
            tutorialAktiv = false;
            printf("Tutorial finished!\n");
        }
    }
}

void Tutorial_Draw(void) {
    if (!tutorialAktiv) return;
    
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    
    if (schritt == 0) {
        DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 180});
    }
    
    int blasenX = 200;
    int blasenY = 300;
    int breite = 400;
    int hoehe = 150;
    
    Rectangle blase = {blasenX, blasenY, breite, hoehe};
    
    DrawRectangleRounded(blase, 0.3f, 8, WHITE);
    
    DrawRectangleRoundedLines(blase, 0.3f, 8, BLACK);
    
    DrawTriangle(
        (Vector2){blasenX + 50, blasenY + hoehe},
        (Vector2){blasenX + 70, blasenY + hoehe + 20},
        (Vector2){blasenX + 90, blasenY + hoehe},
        WHITE
    );
    
    DrawTriangleLines(
        (Vector2){blasenX + 50, blasenY + hoehe},
        (Vector2){blasenX + 70, blasenY + hoehe + 20},
        (Vector2){blasenX + 90, blasenY + hoehe},
        BLACK
    );
    
    const char* text = "";
    //const char* hinweis = "Click to continue";
    
    switch(schritt) {
        case 0:
            text = "Hi! You're the new manager \nin my summer Hotel.\nBuy floors to earn money!";
            break;
        case 1:
            text = "Floors generate money.\nMore floors = more money!";
            // Markiere "BuyNewRoom" Bereich
            //DrawRectangleLines(sw - 200, 100, 180, 60, (Color){255, 255, 0, 200});
            break;
        case 2:
            text = "Money gives you XP and levels.\nNew levels unlock better rooms!";
            break;
        case 3:
            text = "Good luck! You're ready to start.";
            //hinweis = "Click to begin";
            break;
    }
    
    DrawText(text, blasenX + 20, blasenY + 30, 20, BLACK);
    
    float alpha = 0.5f + 0.5f * sinf(blinkTimer * 5);
    //DrawText(hinweis, blasenX + 20, blasenY + hoehe - 30, 16, 
             //(Color){100, 100, 100, (unsigned char)(255 * alpha)});
}