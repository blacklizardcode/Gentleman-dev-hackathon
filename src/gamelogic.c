#include "gamelogic.h"
#include "globals.h"
#include <stdio.h>
#include <stdbool.h>

/*-------------------------------------------------
    Timing constants
    *** Tweak these to change game feel! ***
-------------------------------------------------*/

// Seconds until a new guest checks in (when a slot is free)
#define CHECKIN_INTERVAL   8.0f

// How long a guest stays before checking out
#define CHECKOUT_MIN      20.0f
#define CHECKOUT_MAX      60.0f

// Seconds between two income ticks ("one day")
#define INCOME_INTERVAL   10.0f

/*-------------------------------------------------
    XP constants
    *** Adjust XP rewards here! ***
-------------------------------------------------*/

#define XP_PER_CHECKIN      5    // XP when a guest checks in
#define XP_PER_CHECKOUT     2    // XP when a guest checks out
#define XP_PER_INCOME_TICK  3    // XP each time a room earns income
#define XP_PER_ROOM_BUY    20    // XP when a room is purchased (called from BuyRoom.c via AddXP)

/*-------------------------------------------------
    Level-up notification state
-------------------------------------------------*/

static bool  leveledUpThisFrame  = false;
static int   levelUpDisplayLevel = 0;
static float levelUpTimer        = 0.0f;

#define LEVELUP_DISPLAY_SECONDS  3.0f   // how long the message stays on screen

bool GameLogic_JustLeveledUp(void) {
    return levelUpTimer > 0.0f;
}

const char *GameLogic_LevelUpMessage(void) {
    if (levelUpTimer > 0.0f)
        return TextFormat("LEVEL UP!  Now Level %d", levelUpDisplayLevel);
    return NULL;
}

/*-------------------------------------------------
    Helper: simple pseudo-random float in [mn, mx]
-------------------------------------------------*/
static float SimpleRand(float mn, float mx) {
    static unsigned int seed = 12345;
    seed = seed * 1664525u + 1013904223u;
    float t = (float)(seed & 0xFFFF) / 65535.0f;
    return mn + t * (mx - mn);
}

/*-------------------------------------------------
    Internal: wrapper around AddXP that also
    detects level-ups and sets the display timer.
-------------------------------------------------*/
static void EarnXP(int amount) {
    int levelBefore = levl;
    AddXP(amount);
    if (levl > levelBefore) {
        levelUpDisplayLevel = levl;
        levelUpTimer        = LEVELUP_DISPLAY_SECONDS;
    }
}

/*-------------------------------------------------
    GameLogic_Update
-------------------------------------------------*/
void GameLogic_Update(float deltaTime) {
    int totalGuests = 0;

    // Tick down the level-up display timer
    if (levelUpTimer > 0.0f) {
        levelUpTimer -= deltaTime;
        if (levelUpTimer < 0.0f) levelUpTimer = 0.0f;
    }

    for (int i = 0; i < roomListCount; i++) {
        RoomInstance *inst = &roomList[i];
        if (inst->name == NULL) continue;

        const RoomType *rt = GetRoomTypeByName(inst->name);
        if (rt == NULL) continue;

        // -----------------------------------------------
        //  Bedroom logic  (guestCapacity > 0)
        // -----------------------------------------------
        if (rt->guestCapacity > 0) {
            // --- Check-in: fill empty slots one guest at a time ---
            if (inst->currentGuests < rt->guestCapacity) {
                inst->checkinTimer -= deltaTime;
                if (inst->checkinTimer <= 0.0f) {
                    inst->currentGuests++;
                    inst->checkinTimer = (inst->currentGuests < rt->guestCapacity)
                                         ? CHECKIN_INTERVAL
                                         : 999999.0f;
                    if (inst->checkoutTimer <= 0.0f)
                        inst->checkoutTimer = SimpleRand(CHECKOUT_MIN, CHECKOUT_MAX);

                    EarnXP(XP_PER_CHECKIN);  // XP for check-in

                    printf("[Check-in]  %s  guests: %d/%d\n",
                           inst->name, inst->currentGuests, rt->guestCapacity);
                }
            }

            // --- Check-out: one guest leaves at a time ---
            if (inst->currentGuests > 0) {
                inst->checkoutTimer -= deltaTime;
                if (inst->checkoutTimer <= 0.0f) {
                    inst->currentGuests--;
                    inst->checkinTimer  = CHECKIN_INTERVAL;
                    inst->checkoutTimer = (inst->currentGuests > 0)
                                          ? SimpleRand(CHECKOUT_MIN, CHECKOUT_MAX)
                                          : 0.0f;

                    EarnXP(XP_PER_CHECKOUT);  // XP for check-out

                    printf("[Check-out] %s  guests: %d/%d\n",
                           inst->name, inst->currentGuests, rt->guestCapacity);
                }
            }

            totalGuests += inst->currentGuests;
        }

        // -----------------------------------------------
        //  Income tick  (every INCOME_INTERVAL seconds)
        // -----------------------------------------------
        if (rt->dailyIncome > 0 || rt->incomePerGuest > 0) {
            inst->incomeTimer -= deltaTime;
            if (inst->incomeTimer <= 0.0f) {
                inst->incomeTimer = INCOME_INTERVAL;

                int earned = rt->dailyIncome
                           + rt->incomePerGuest * inst->currentGuests;

                if (earned > 0) {
                    monney += earned;
                    EarnXP(XP_PER_INCOME_TICK);  // XP for earning income

                    printf("[Income] %s  +%d  (total: %d)\n",
                           inst->name, earned, monney);
                }
            }
        }
    }

    guests = totalGuests;
}