#include "raylib.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "loadTexture.h"
#include "Ui.h"
#include "BuyRoom.h"
#include "mouse.h"
#include "globals.h"

// -------------------------------------------------------
//  Room Selection (3 random rooms for the current level)
// -------------------------------------------------------

#define SELECTED_ROOM_COUNT 3

static const char* selectedRooms[SELECTED_ROOM_COUNT] = {NULL, NULL, NULL};
static bool selectionInitialized = false;

void GenerateRoomSelection(void) {
    srand((unsigned int)time(NULL));

    const char* available[64];
    int availableCount = 0;

    for (int i = 0; i < RoomTypesCount && availableCount < 64; i++) {
        const RoomType *rt = &RoomTypes[i];
        if (strcmp(rt->name, "FrontDesk")  == 0) continue;
        if (strcmp(rt->name, "BuyNewRoom") == 0) continue;
        if (rt->unlockLevel <= levl) {
            available[availableCount++] = rt->name;
        }
    }

    if (availableCount == 0) {
        for (int i = 0; i < SELECTED_ROOM_COUNT; i++) selectedRooms[i] = NULL;
        selectionInitialized = true;
        return;
    }

    for (int i = 0; i < SELECTED_ROOM_COUNT; i++) {
        if (availableCount >= SELECTED_ROOM_COUNT) {
            int attempts = 0;
            int idx;
            bool duplicate;
            do {
                idx = rand() % availableCount;
                duplicate = false;
                for (int j = 0; j < i; j++) {
                    if (selectedRooms[j] == available[idx]) { duplicate = true; break; }
                }
                attempts++;
            } while (duplicate && attempts < 20);
            selectedRooms[i] = available[idx];
        } else {
            selectedRooms[i] = available[rand() % availableCount];
        }
    }

    selectionInitialized = true;
}

const char** GetSelectedRooms(void) {
    if (!selectionInitialized) GenerateRoomSelection();
    return selectedRooms;
}

// -------------------------------------------------------
//  Card texture name mapping
// -------------------------------------------------------

static const char* GetCardTextureName(const char* roomName) {
    if (roomName == NULL) return NULL;
    if (strcmp(roomName, "kitchen")           == 0) return "kitchenCard";
    if (strcmp(roomName, "PresidentialSuite") == 0) return "PresidentialSuiteCard";
    if (strcmp(roomName, "room")              == 0) return "RoomCard";
    if (strcmp(roomName, "Suites")            == 0) return "SuiteCard";
    if (strcmp(roomName, "Sauna")             == 0) return "SaunaCard";
    if (strcmp(roomName, "MovieTheater")      == 0) return "MovieTheaterCard";
    return NULL;
}

// -------------------------------------------------------
//  Draw Buy-Room UI  — 3 floating cards
// -------------------------------------------------------

void DrawBuyRoomUi(void) {
    if (!selectionInitialized) GenerateRoomSelection();

    static Texture2D cardTex[SELECTED_ROOM_COUNT]            = {{0}};
    static const char* cardTexNameCache[SELECTED_ROOM_COUNT] = {NULL};

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    int padding  = (int)(sw * 0.02f);
    int maxCardH = (int)(sh * 0.80f);

    float scaledW[SELECTED_ROOM_COUNT];
    float scaledH[SELECTED_ROOM_COUNT];

    // --- Load / cache textures and compute scaled sizes ---
    for (int i = 0; i < SELECTED_ROOM_COUNT; i++) {
        scaledW[i] = 0;
        scaledH[i] = 0;
        const char* roomName    = selectedRooms[i];
        const char* cardTexName = GetCardTextureName(roomName);
        if (cardTexName == NULL) continue;
        Image* img = GetTexture(cardTexName);
        if (img == NULL || img->data == NULL) continue;
        if (cardTexNameCache[i] == NULL || strcmp(cardTexNameCache[i], cardTexName) != 0) {
            if (cardTex[i].id != 0) UnloadTexture(cardTex[i]);
            cardTex[i]          = LoadTextureFromImage(*img);
            cardTexNameCache[i] = cardTexName;
        }
        if (cardTex[i].id != 0) {
            float aspect = (float)cardTex[i].width / (float)cardTex[i].height;
            scaledH[i]   = (float)maxCardH;
            scaledW[i]   = scaledH[i] * aspect;
        }
    }

    // --- Compute layout ---
    float totalWidth = 0;
    for (int i = 0; i < SELECTED_ROOM_COUNT; i++) totalWidth += scaledW[i];
    totalWidth += (float)(padding * (SELECTED_ROOM_COUNT - 1));

    // --- Draw cards ---
    Rectangle cardDest[SELECTED_ROOM_COUNT];
    float curX = (sw - totalWidth) / 2.0f;
    for (int i = 0; i < SELECTED_ROOM_COUNT; i++) {
        cardDest[i] = (Rectangle){0};
        if (cardTex[i].id == 0) { curX += padding; continue; }

        const char* roomName = selectedRooms[i];
        const RoomType *rt   = GetRoomTypeByName(roomName);
        bool canAfford       = (rt == NULL) || (monney >= rt->cost);

        float cardY = (sh - scaledH[i]) / 2.0f;
        Rectangle src  = {0, 0, (float)cardTex[i].width, (float)cardTex[i].height};
        Rectangle dest = {curX, cardY, scaledW[i], scaledH[i]};
        cardDest[i] = dest;

        // Dim card if player cannot afford it
        Color tint = canAfford ? WHITE : (Color){120, 120, 120, 200};
        DrawTexturePro(cardTex[i], src, dest, (Vector2){0, 0}, 0.0f, tint);

        // Show price below the card
        if (rt != NULL) {
            const char *priceText = TextFormat("%d G", rt->cost);
            int fontSize   = (int)(sw * 0.018f);
            if (fontSize < 12) fontSize = 12;
            int textW      = MeasureText(priceText, fontSize);
            int textX      = (int)(dest.x + dest.width / 2.0f) - textW / 2;
            int textY      = (int)(dest.y + dest.height) + 6;
            Color priceCol = canAfford ? GREEN : RED;
            DrawText(priceText, textX, textY, fontSize, priceCol);
        }

        // Hover: white border (only when affordable)
        if (canAfford && IsCardHovered(dest)) {
            Rectangle border = {dest.x + 2, dest.y + 2, dest.width - 4, dest.height - 4};
            DrawRectangleLinesEx(border, 3, WHITE);

            // Click: buy the room
            if (IsCardClicked(dest)) {
                addRoomToList(selectedRooms[i]);
                RoomSelect = false;
                selectionInitialized = false;
            }
        }

        curX += scaledW[i] + padding;
    }

    // --- Click outside all cards closes the menu ---
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        bool overAnyCard = false;
        for (int i = 0; i < SELECTED_ROOM_COUNT; i++) {
            if (cardDest[i].width > 0 && IsMouseOverRect(cardDest[i])) {
                overAnyCard = true;
                break;
            }
        }
        if (!overAnyCard) {
            RoomSelect = false;
        }
    }
}

// -------------------------------------------------------
//  addRoomToList
// -------------------------------------------------------

void addRoomToList(const char *roomName) {
    if (roomName == NULL) {
        printf("addRoomToList: roomName is NULL\n");
        return;
    }

    const RoomType *rt = GetRoomTypeByName(roomName);
    if (rt == NULL) {
        printf("addRoomToList: unknown room type '%s'\n", roomName);
        return;
    }

    // Funds check
    if (monney < rt->cost) {
        printf("addRoomToList: not enough money! need %d, have %d\n", rt->cost, monney);
        return;
    }

    monney -= rt->cost;
    printf("Bought '%s' for %d G. Remaining money: %d\n", roomName, rt->cost, monney);

    RoomList_Add(roomName);
}