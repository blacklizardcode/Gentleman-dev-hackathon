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

extern bool RoomOwned[];      // same length as RoomTypes
extern int Roomcount;         // number of owned rooms

bool IsRoomOwned(const char *name);
void SetRoomOwned(const char *name, bool owned);

/*-------------------------------------------------
    Buy Room
-------------------------------------------------*/
extern bool RoomSelect;

