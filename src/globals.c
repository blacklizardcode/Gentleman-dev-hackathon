#include "raylib.h"
#include "globals.h"
#include <string.h>
#include <stdio.h>

/*-------------------------------------------------
    Room Types  (main balancing table)

    Columns: name | unlockLevel | cost | guestCapacity | incomePerGuest | dailyIncome
    *** Change all numbers here! ***
-------------------------------------------------*/

const RoomType RoomTypes[] = {
    // name                 lvl  cost  cap  perGuest  daily
    {"FrontDesk",            1,    0,   0,     0,       0  },  // not buyable
    {"BuyNewRoom",           1,    0,   0,     0,       0  },  // not buyable

    // Bedrooms (earn per guest + small base income)
    {"room",                 1,  100,   1,     5,       5  },  // Basic room:          1 guest,  5/guest/day, +5 base
    {"Suites",               4,  200,   5,    10,      10  },  // Suite:               5 guests, 10/guest/day, +10 base
    {"PresidentialSuite",    5,  700,   8,    20,      20  },  // Presidential suite:  8 guests, 20/guest/day, +20 base

    // Service rooms (flat daily income, no guests)
    {"kitchen",              2,  200,   0,     0,       5  },  // Kitchen:       200 cost,  5/day
    {"MovieTheater",         7,  900,   0,     0,      15  },  // Movie theater: 900 cost, 15/day
    {"Restaurant",           8,  800,   0,     0,      10  },  // Restaurant:    800 cost, 10/day
    {"Sauna",                6,  700,   0,     0,      70  },  // Sauna:         700 cost, 70/day
};

const int RoomTypesCount = sizeof(RoomTypes) / sizeof(RoomTypes[0]);

const RoomType *GetRoomTypeByName(const char *name) {
    if (name == NULL) return NULL;
    for (int i = 0; i < RoomTypesCount; i++) {
        if (strcmp(RoomTypes[i].name, name) == 0)
            return &RoomTypes[i];
    }
    return NULL;
}

/*-------------------------------------------------
    Room Instance List
-------------------------------------------------*/

RoomInstance roomList[MAX_ROOM_INSTANCES];
int          roomListCount = 0;

static int FindBuyNewRoomIndex(void) {
    for (int i = 0; i < roomListCount; i++) {
        if (roomList[i].name != NULL &&
            strcmp(roomList[i].name, "BuyNewRoom") == 0)
            return i;
    }
    return roomListCount;
}

void RoomList_Init(void) {
    roomListCount = 0;
    roomList[roomListCount] = (RoomInstance){ "BuyNewRoom", 0, 0.0f, 0.0f, 0.0f };
    roomListCount++;
    roomList[roomListCount] = (RoomInstance){ "FrontDesk",  0, 0.0f, 0.0f, 0.0f };
    roomListCount++;
}

void RoomList_Add(const char *roomName) {
    if (roomName == NULL) return;
    if (roomListCount >= MAX_ROOM_INSTANCES) {
        printf("RoomList_Add: list full!\n");
        return;
    }

    int insertPos = FindBuyNewRoomIndex() + 1;

    for (int i = roomListCount; i > insertPos; i--)
        roomList[i] = roomList[i - 1];

    const RoomType *rt = GetRoomTypeByName(roomName);

    roomList[insertPos].name          = roomName;
    roomList[insertPos].currentGuests = 0;
    roomList[insertPos].checkoutTimer = 0.0f;
    roomList[insertPos].checkinTimer  = (rt && rt->guestCapacity > 0) ? 8.0f : 0.0f;
    roomList[insertPos].incomeTimer   = 10.0f;   // first income tick after 10 s
    roomListCount++;

    printf("Room '%s' added (position %d, total %d)\n",
           roomName, insertPos, roomListCount);
}

/*-------------------------------------------------
    Buy Room
-------------------------------------------------*/
bool RoomSelect = false;

/*-------------------------------------------------
    Player Stats
-------------------------------------------------*/
int levl   = 1;
int monney = 500;
int guests = 0;

// XP system
int xp           = 0;
int xpForNextLevel = 100;   // XP needed for level 2; grows each level

void AddXP(int amount) {
    xp += amount;
    while (xp >= xpForNextLevel) {
        xp -= xpForNextLevel;
        levl++;
        // Each level requires 20% more XP than the previous one
        xpForNextLevel = (int)(xpForNextLevel * 1.2f);
        printf("[Level Up!] Now level %d  (next level needs %d XP)\n", levl, xpForNextLevel);
    }
}