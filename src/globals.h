#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"
#include "Ui.h"
#include <stdbool.h>

/*-------------------------------------------------
    Room Types  (catalogue of all buyable rooms)
-------------------------------------------------*/

typedef struct {
    const char *name;   // matches texture + identifier
    int unlockLevel;    // player level required to unlock
} RoomType;

extern const RoomType RoomTypes[];
extern const int RoomTypesCount;

/*-------------------------------------------------
    Room Instance List  (rooms built in the hotel)
    Dynamic — grows as new rooms are purchased.
    Layout order (top to bottom):
      [0]      BuyNewRoom
      [1..n-2] purchased rooms (newest at top)
      [n-1]    FrontDesk
-------------------------------------------------*/

typedef struct {
    const char *name;   // points to a RoomType.name (no owned memory)
} RoomInstance;

#define MAX_ROOM_INSTANCES 256

extern RoomInstance roomList[];     // currently built rooms
extern int          roomListCount;  // number of entries

// Inserts a room directly after the "BuyNewRoom" button (index 1),
// so newly purchased rooms always appear at the top of the stack.
// roomName must be a valid RoomType name.
void RoomList_Add(const char *roomName);

// Initialises the list: BuyNewRoom (top) + FrontDesk (bottom).
void RoomList_Init(void);

/*-------------------------------------------------
    Buy Room
-------------------------------------------------*/
extern bool RoomSelect;

/*-------------------------------------------------
    Level
-------------------------------------------------*/
extern int levl;
extern int monney;
extern int guests;
#endif // GLOBALS_H