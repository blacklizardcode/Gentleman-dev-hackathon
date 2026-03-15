#include "raylib.h"
#include "moove.h"

static float cameraY = 0.0f;
static bool dragging = false;
static int lastMouseY = 0;

void Camera_Update(int worldHeight, int windowHeight, float deltaTime) {
    const float keyboardSpeed = 400.0f;
    const float mouseDragSpeed = 1.0f;     // how fast dragging moves the camera
    const float mouseWheelSpeed = 120.0f;   // scroll wheel delta scaling

    if (windowHeight >= worldHeight) {
        cameraY = 0.0f;
        // ensure we stop dragging when there's nothing to scroll
        dragging = false;
        return;
    }

    // Keyboard (W/S)
    if (IsKeyDown(KEY_W)) cameraY -= keyboardSpeed * deltaTime;
    if (IsKeyDown(KEY_S)) cameraY += keyboardSpeed * deltaTime;

    // Mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        cameraY -= wheel * mouseWheelSpeed;
    }

    // Mouse drag (right button)
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        int mouseY = GetMouseY();
        if (!dragging) {
            dragging = true;
            lastMouseY = mouseY;
        } else {
            int deltaY = mouseY - lastMouseY;
            // Invert drag direction so dragging the mouse up scrolls up (and down scrolls down)
            cameraY -= deltaY * mouseDragSpeed;
            lastMouseY = mouseY;
        }
    } else {
        dragging = false;
    }

    // Clamp camera to world bounds.
    const float maxCameraY = (float)(worldHeight - windowHeight);
    if (cameraY < 0.0f) cameraY = 0.0f;
    if (cameraY > maxCameraY) cameraY = maxCameraY;
}

float Camera_GetY(void) {
    return cameraY;
}
