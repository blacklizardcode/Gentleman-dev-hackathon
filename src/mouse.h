#ifndef MOUSE_H
#define MOUSE_H

#include "raylib.h"
#include <stdbool.h>

// -------------------------------------------------------
//  Frame reset
// -------------------------------------------------------

// Resets the click-consumption guard.
// Call ONCE at the very start of the game loop each frame.
void Mouse_NewFrame(void);

// -------------------------------------------------------
//  Basic helpers
// -------------------------------------------------------

// Returns true if the mouse cursor is inside the given rectangle.
bool IsMouseOverRect(Rectangle rect);

// -------------------------------------------------------
//  Room-world hover / click  (used by drawRoom.c)
// -------------------------------------------------------

// Returns true if the mouse is over the rect AND RoomSelect is false.
// Use this instead of IsMouseOverRect inside DrawRoom so that hover
// effects are suppressed while the buy-menu is open.
bool IsRoomHovered(Rectangle rect);

// Returns true if the left mouse button was just pressed while the
// cursor is over rect AND RoomSelect is false.
bool IsRoomClicked(Rectangle rect);

// -------------------------------------------------------
//  Card hover / click  (used by BuyRoom.c)
// -------------------------------------------------------

// Returns true if the mouse is over the card rect AND RoomSelect is true.
// Intentionally the inverse condition so cards are only interactive
// while the buy-menu is open.
bool IsCardHovered(Rectangle rect);

// Returns true if the left mouse button was just pressed over the card
// AND RoomSelect is true.
bool IsCardClicked(Rectangle rect);

#endif // MOUSE_H