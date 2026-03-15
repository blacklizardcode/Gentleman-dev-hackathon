#include "mouse.h"
#include "globals.h"   // RoomSelect

// -------------------------------------------------------
//  Click-consumption guard
//  Ensures a single mouse-press cannot trigger two actions
//  in the same frame (e.g. opening AND closing the menu).
// -------------------------------------------------------

static bool clickConsumed = false;

// Call once at the start of every frame to reset the guard.
void Mouse_NewFrame(void) {
    clickConsumed = false;
}

// -------------------------------------------------------
//  Basic helpers
// -------------------------------------------------------

bool IsMouseOverRect(Rectangle rect) {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, rect);
}

// -------------------------------------------------------
//  Room-world interaction  (suppressed while buy-menu is open)
// -------------------------------------------------------

bool IsRoomHovered(Rectangle rect) {
    if (RoomSelect) return false;
    return IsMouseOverRect(rect);
}

bool IsRoomClicked(Rectangle rect) {
    if (RoomSelect) return false;
    if (clickConsumed) return false;
    if (IsMouseOverRect(rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        clickConsumed = true;
        return true;
    }
    return false;
}

// -------------------------------------------------------
//  Card interaction  (only active while buy-menu is open)
// -------------------------------------------------------

bool IsCardHovered(Rectangle rect) {
    if (!RoomSelect) return false;
    return IsMouseOverRect(rect);
}

bool IsCardClicked(Rectangle rect) {
    if (!RoomSelect) return false;
    if (clickConsumed) return false;
    if (IsMouseOverRect(rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        clickConsumed = true;
        return true;
    }
    return false;
}