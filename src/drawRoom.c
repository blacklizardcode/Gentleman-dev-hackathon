#include "raylib.h"
#include "loadTexture.h"
#include "Ui.h"

void DrawRoom() {
    Image* roomImage = GetTexture("basicRoom"); // 800 * 300
    
    if (roomImage != NULL && roomImage->data != NULL) {
        static Texture2D roomTexture = {0};
        static bool textureLoaded = false;
        
        if (!textureLoaded) {
            roomTexture = LoadTextureFromImage(*roomImage);
            textureLoaded = true;
        }

        int windowWidth = GetScreenWidth();
        int windowHeight = GetScreenHeight();

        int basicRoomX = 0;
        int basicRoomY = windowHeight - roomTexture.height;

        float scaleX = (float)windowWidth / roomTexture.width;
        Rectangle sourceRec = {0, 0, (float)roomTexture.width, (float)roomTexture.height};
        Rectangle destRec = {0, (float)basicRoomY, (float)windowWidth, (float)roomTexture.height};
        Vector2 origin = {0, 0};
        
        DrawTexturePro(roomTexture, sourceRec, destRec, origin, 0.0f, WHITE);
    } else {
        DrawRectangle(0, GetScreenHeight() - 150, GetScreenWidth(), 150, LIGHTGRAY);
        DrawText("Room Texture", 120, GetScreenHeight() - 80, 20, DARKGRAY);
    }
}