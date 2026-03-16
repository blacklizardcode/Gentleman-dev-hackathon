#ifndef BUYROOM_H
#define BUYROOM_H

void GenerateRoomSelection(void);
const char** GetSelectedRooms(void);
void DrawBuyRoomUi(void);

void addRoomToList(const char *roomName);

#endif