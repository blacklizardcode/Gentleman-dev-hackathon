#include "raylib.h"
#include "Ui.h"

/*-------------------------------------------------
    Rooms

-------------------------------------------------*/

typedef struct {
    const char *name;   // matches texture + identifier
    int unlockLevel;    // player level required to be able to buy
} RoomType;

extern const RoomType RoomTypes[];
extern const int RoomTypesCount;

// (No ownership tracking; all rooms are available to build.)

/*-------------------------------------------------
    Buy Room
-------------------------------------------------*/
extern bool RoomSelect;

