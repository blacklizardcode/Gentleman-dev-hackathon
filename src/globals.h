#include "raylib.h"
#include "Ui.h"

/*-------------------------------------------------
    Rooms
-------------------------------------------------*/
enum RoomTypes {
    FrontDesk,
    kitchen,
    room,
    Suites,
    PresidentialSuite,
    swimmingPool,
    Restaurant,
    Bar,
    RoomService,
    Sauna,
    MovieTheater,
    Disco,
};

struct Rooms {
    int id;
    enum RoomTypes roomtype;
};
extern int Roomcount;

