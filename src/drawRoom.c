#include "raylib.h"
#include "Ui.h"

void DrawRoom() {
    Image* roomImage = GetTexture("basicRoom");
    
    if (roomImage != NULL && roomImage->data != NULL) {
        static Texture2D roomTexture = {0};
        static bool textureLoaded = false;
        
        if (!textureLoaded) {
            roomTexture = LoadTextureFromImage(*roomImage);
            textureLoaded = true;
        }
        
        DrawTexture(roomTexture, 100, 100, WHITE);
    } else {
        DrawRectangle(100, 100, 200, 150, LIGHTGRAY);
        DrawText("Room Texture", 120, 170, 20, DARKGRAY);
    }
}