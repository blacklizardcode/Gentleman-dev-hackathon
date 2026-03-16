#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stdbool.h>

// Must be called once per frame.
// deltaTime = GetFrameTime()
void GameLogic_Update(float deltaTime);

// Returns true while the level-up animation is playing.
bool GameLogic_JustLeveledUp(void);

// Returns the level-up message string, or NULL if no level-up active.
const char *GameLogic_LevelUpMessage(void);

#endif // GAMELOGIC_H