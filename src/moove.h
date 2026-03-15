#ifndef MOOOVE_H
#define MOOOVE_H

#include "raylib.h"

// Updates the camera position based on input (keyboard, mouse wheel, mouse drag).
// worldHeight: total height of the room world (in pixels).
// windowHeight: current viewport height (in pixels).
// deltaTime: frame time in seconds.
void Camera_Update(int worldHeight, int windowHeight, float deltaTime);

// Returns the current vertical camera offset.
float Camera_GetY(void);

#endif // MOOOVE_H
