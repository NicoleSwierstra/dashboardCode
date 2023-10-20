#pragma once
#include <stdint.h>
#define DASH_HP_MAX_ERRORS 16
#define DASH_LP_MAX_ERRORS 31

namespace dashstorage {
    volatile struct datastruct {
        //MCU TEMP
        //BATTERY VOLTAGE
        //BATTERY TEMPURTURE
        //MOTOR TEMP
    } dashData {};

    struct driverScreen {
        uint32_t lapsOnCurrentCharge;
        uint32_t lapTimeMS;
        uint32_t lapTimeDeltaMS;
    } driverscreen {0, 0, 0};

    struct errorScreen {
        uint16_t errortype;
        uint16_t errormessage;
    } hpErrorScreenStack[DASH_HP_MAX_ERRORS], lpErrorScreenStack[DASH_LP_MAX_ERRORS];

    uint8_t lpErrorScreen = 0, hpErrorScreen = 0; //number of errors displayed on screens.

    /**
     * @brief pushes a new error onto error screen stack
     * 
     * @param highPriority if the message is a high priority or low priority message
     * @param er the error screen to be pushed back
     * @return true if error pushed back pops an error off the back of the array, otherwise false
     */
    bool pushBackError(bool highPriority, errorScreen er);
}