#include "raylib.h"
#include "globals.h"
#include <string.h>

/*-------------------------------------------------
    Rooms

-------------------------------------------------*/

const RoomType RoomTypes[] = {
    {"FrontDesk", 1},
    {"kitchen", 2},
    {"room", 3},
    {"Suites", 4},
    {"PresidentialSuite", 5},
    {"swimmingPool", 6},
    {"Restaurant", 7},
    {"Bar", 8},
    {"RoomService", 9},
    {"Sauna", 10},
    {"MovieTheater", 11},
    {"Disco", 12},
    //{"basicRoom", 1},
    {"BuyNewRoom", 1},
};

const int RoomTypesCount = sizeof(RoomTypes) / sizeof(RoomTypes[0]);

// Which rooms are currently owned by the player.
// Matches the order of RoomTypes.
bool RoomOwned[] = {
    true, // FrontDesk
    false, // kitchen
    false, // room
    false, // Suites
    false, // PresidentialSuite
    false, // swimmingPool
    false, // Restaurant
    false, // Bar
    false, // RoomService
    false, // Sauna
    false, // MovieTheater
    false, // Disco
    //true,  // basicRoom 
    false, // BuyNewRoom (not a real room)
};

// Convenience count of owned rooms (excludes the BuyNewRoom pseudo-room).
int Roomcount = 1;

bool IsRoomOwned(const char *name) {
    for (int i = 0; i < RoomTypesCount; i++) {
        if (strcmp(RoomTypes[i].name, name) == 0) {
            return RoomOwned[i];
        }
    }
    return false;
}

void SetRoomOwned(const char *name, bool owned) {
    for (int i = 0; i < RoomTypesCount; i++) {
        if (strcmp(RoomTypes[i].name, name) == 0) {
            RoomOwned[i] = owned;
            // update owner count with a quick scan
            int count = 0;
            for (int j = 0; j < RoomTypesCount; j++) {
                // ignore pseudo entries like BuyNewRoom
                if (strcmp(RoomTypes[j].name, "BuyNewRoom") == 0) continue;
                if (RoomOwned[j]) count++;
            }
            Roomcount = count;
            return;
        }
    }
}

/*-------------------------------------------------
    Buy Room
-------------------------------------------------*/
bool RoomSelect = false;
