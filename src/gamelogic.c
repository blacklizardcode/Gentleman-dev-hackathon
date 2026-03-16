#include "gamelogic.h"
#include "globals.h"
#include <stdio.h>

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
    Helper: simple pseudo-random float in [mn, mx]
    Uses a linear congruential generator so we
    don't need to call srand() again here.
-------------------------------------------------*/
static float SimpleRand(float mn, float mx) {
    static unsigned int seed = 12345;
    seed = seed * 1664525u + 1013904223u;
    float t = (float)(seed & 0xFFFF) / 65535.0f;   // 0..1
    return mn + t * (mx - mn);
}

/*-------------------------------------------------
    GameLogic_Update
-------------------------------------------------*/
void GameLogic_Update(float deltaTime) {
    int totalGuests = 0;

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
                    // schedule next check-in, or park the timer when full
                    inst->checkinTimer = (inst->currentGuests < rt->guestCapacity)
                                         ? CHECKIN_INTERVAL
                                         : 999999.0f;
                    // start checkout timer if none is running yet
                    if (inst->checkoutTimer <= 0.0f)
                        inst->checkoutTimer = SimpleRand(CHECKOUT_MIN, CHECKOUT_MAX);

                    printf("[Check-in]  %s  guests: %d/%d\n",
                           inst->name, inst->currentGuests, rt->guestCapacity);
                }
            }

            // --- Check-out: one guest leaves at a time ---
            if (inst->currentGuests > 0) {
                inst->checkoutTimer -= deltaTime;
                if (inst->checkoutTimer <= 0.0f) {
                    inst->currentGuests--;
                    // restart check-in timer now that a slot is free
                    inst->checkinTimer  = CHECKIN_INTERVAL;
                    // schedule next checkout if guests remain
                    inst->checkoutTimer = (inst->currentGuests > 0)
                                          ? SimpleRand(CHECKOUT_MIN, CHECKOUT_MAX)
                                          : 0.0f;
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
                    printf("[Income] %s  +%d  (total: %d)\n",
                           inst->name, earned, monney);
                }
            }
        }
    }

    guests = totalGuests;
}