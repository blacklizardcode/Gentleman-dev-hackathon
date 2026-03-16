#include "raylib.h"
#include "globals.h"
#include <string.h>
#include <stdio.h>

/*-------------------------------------------------
    Room Types  (catalogue of all buyable rooms)
-------------------------------------------------*/

const RoomType RoomTypes[] = {
    {"FrontDesk",         1}, // not buyable
    {"room",              1},
    {"kitchen",           2},
    {"Suites",            4},
    {"PresidentialSuite", 5},
    {"Sauna",             6},
    {"MovieTheater",      7},
    {"Restaurant",        8},
    {"BuyNewRoom",        1}, // not buyable (internal placeholder)
};

const int RoomTypesCount = sizeof(RoomTypes) / sizeof(RoomTypes[0]);

/*-------------------------------------------------
    Room Instance List
-------------------------------------------------*/

RoomInstance roomList[MAX_ROOM_INSTANCES];
int          roomListCount = 0;

// Find the "BuyNewRoom" entry and return its index.
// Returns roomListCount if not found.
static int FindBuyNewRoomIndex(void) {
    for (int i = 0; i < roomListCount; i++) {
        if (roomList[i].name != NULL &&
            strcmp(roomList[i].name, "BuyNewRoom") == 0) {
            return i;
        }
    }
    return roomListCount;
}

void RoomList_Init(void) {
    roomListCount = 0;

    // Initial state: buy button on top, lobby at the bottom
    roomList[roomListCount++].name = "BuyNewRoom";
    roomList[roomListCount++].name = "FrontDesk";
}

void RoomList_Add(const char *roomName) {
    if (roomName == NULL) return;
    if (roomListCount >= MAX_ROOM_INSTANCES) {
        printf("RoomList_Add: list full!\n");
        return;
    }

    // Insert the new room directly AFTER "BuyNewRoom" (index 0),
    // so new rooms stack up between the buy button and the lobby.
    int insertPos = FindBuyNewRoomIndex() + 1;

    // Shift everything from insertPos one slot toward the end.
    for (int i = roomListCount; i > insertPos; i--) {
        roomList[i] = roomList[i - 1];
    }

    roomList[insertPos].name = roomName;
    roomListCount++;

    printf("Room '%s' added (position %d, total %d)\n",
           roomName, insertPos, roomListCount);
}

/*-------------------------------------------------
    Buy Room
-------------------------------------------------*/
bool RoomSelect = false;

/*-------------------------------------------------
    Level
-------------------------------------------------*/
int levl = 50;