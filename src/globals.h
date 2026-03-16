#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"
#include "Ui.h"
#include <stdbool.h>

/*-------------------------------------------------
    Room Types  (catalogue of all buyable rooms)

    Fields:
      name          - internal name (must match texture name)
      unlockLevel   - player level required to unlock
      cost          - purchase price
      guestCapacity - max guests that can stay at once (0 = not a bedroom)
      incomePerGuest- money earned per guest per "day"
      dailyIncome   - flat daily income regardless of guests (service rooms)

    *** Adjust all balancing values here! ***
-------------------------------------------------*/

typedef struct {
    const char *name;
    int  unlockLevel;
    int  cost;
    int  guestCapacity;   // 0 = not a bedroom
    int  incomePerGuest;  // income per guest per day
    int  dailyIncome;     // flat daily income (service room)
} RoomType;

extern const RoomType RoomTypes[];
extern const int RoomTypesCount;

// Returns a pointer to the RoomType with the given name, or NULL.
const RoomType *GetRoomTypeByName(const char *name);

/*-------------------------------------------------
    Room Instance List  (rooms built in the hotel)
-------------------------------------------------*/

typedef struct {
    const char *name;          // points to RoomType.name (no owned memory)
    int  currentGuests;        // guests currently checked in
    float checkoutTimer;       // seconds until the next guest checks out
    float checkinTimer;        // seconds until the next guest checks in
    float incomeTimer;         // seconds until the next income tick
} RoomInstance;

#define MAX_ROOM_INSTANCES 256

extern RoomInstance roomList[];
extern int          roomListCount;

void RoomList_Add(const char *roomName);
void RoomList_Init(void);

/*-------------------------------------------------
    Buy Room
-------------------------------------------------*/
extern bool RoomSelect;

/*-------------------------------------------------
    Player Stats
-------------------------------------------------*/
extern int levl;
extern int monney;
extern int guests;   // total guests in the hotel (for UI)

// XP system
extern int xp;           // current XP points
extern int xpForNextLevel; // XP needed to reach next level

// Adds XP and handles leveling up. Call this whenever XP is earned.
void AddXP(int amount);

#endif // GLOBALS_H