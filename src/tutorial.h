#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <stdbool.h>

void Tutorial_Init(void);
void Tutorial_Update(void);
void Tutorial_Draw(void);
bool Tutorial_IsFinished(void);
void Tutorial_Skip(void);

#endif