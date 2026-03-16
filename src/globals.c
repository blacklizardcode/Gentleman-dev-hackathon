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

    // -------------------------------------------------------
    //  Bedrooms  (earn per guest each income tick + flat base)
    //  Amortisation target: ~25-35 income ticks to break even
    // -------------------------------------------------------
    //                       lvl  cost  cap  perGuest  daily
    {"room",                  1,   80,   1,     6,       4  },  // Basic room:  1 Gast,  6/Gast + 4 base  => 10/tick  | BEP ~8 ticks
    {"Suites",                3,  450,   3,    12,       8  },  // Suite:       3 Gäste, 12/Gast + 8 base => 44/tick  | BEP ~11 ticks
    {"PresidentialSuite",     6,  900,   5,    25,      15  },  // Presi-Suite: 5 Gäste, 25/Gast +15 base =>140/tick  | BEP ~7 ticks (Prestige)

    // -------------------------------------------------------
    //  Service rooms  (flat income, no guests, higher returns)
    //  Amortisation target: ~20-30 income ticks
    // -------------------------------------------------------
    //                       lvl   cost  cap  perGuest  daily
    {"kitchen",               2,   150,   0,     0,      10  },  // Küche:         150 cost, 10/tick  | BEP 15 ticks
    {"Sauna",                 4,   400,   0,     0,      25  },  // Sauna:         400 cost, 25/tick  | BEP 16 ticks
    {"Restaurant",            5,   600,   0,     0,      40  },  // Restaurant:    600 cost, 40/tick  | BEP 15 ticks
    {"MovieTheater",          7,  1100,   0,     0,      70  },  // Kino:         1100 cost, 70/tick  | BEP ~16 ticks (late game)
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
int monney = 200;   // Startgeld: nur 1-2 Basic Rooms moeglich -> zwingt zum Wirtschaften
int guests = 0;

// XP system
int xp             = 0;
int xpForNextLevel = 60;    // XP needed for level 2 (schneller Start)

void AddXP(int amount) {
    xp += amount;
    while (xp >= xpForNextLevel) {
        xp -= xpForNextLevel;
        levl++;
        // Jedes Level braucht 30% mehr XP als das vorherige (flacher als vorher => besser skalierend)
        xpForNextLevel = (int)(xpForNextLevel * 1.30f);
        printf("[Level Up!] Now level %d  (next level needs %d XP)\n", levl, xpForNextLevel);
    }
}