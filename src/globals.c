#include "raylib.h"
#include "globals.h"

/*-------------------------------------------------
    Rooms
-------------------------------------------------*/

const RoomType RoomTypes[] = {
    {"FrontDesk", 1}, //cant be bought
    {"kitchen", 2},
    {"room", 1},
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
    {"BuyNewRoom", 1}, //cant be bought
};

const int RoomTypesCount = sizeof(RoomTypes) / sizeof(RoomTypes[0]);

/*-------------------------------------------------
    Buy Room
-------------------------------------------------*/
bool RoomSelect = false;

//levl
int levl = 5;

