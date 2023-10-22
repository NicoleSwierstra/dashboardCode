#pragma once
#include <stdint.h>
#define DASH_HP_MAX_ERRORS 16
#define DASH_LP_MAX_ERRORS 32

namespace DashStorage {
    struct _DataStruct {
        //MCU TEMP
        //BATTERY VOLTAGE
        //BATTERY TEMPURTURE
        //MOTOR TEMP
    } extern dataStruct;

    struct _DriverScreen {
        uint32_t lapsOnCurrentCharge;
        uint32_t lapTimeMS;
        uint32_t lapTimeDeltaMS;
    } extern driverScreen;

    struct _DashError { 
        char id[20];
        const char* errorPointer = 0;
    };

    struct _ErrorStack {
        _DashError hpErrorStack[DASH_HP_MAX_ERRORS];
        _DashError lpErrorStack[DASH_LP_MAX_ERRORS];
        uint8_t lpErrorPointer = 0, hpErrorPointer = 0;
        bool lpfull, hpfull;
    } extern errorStacks;

    /**
     * @brief pushes a new error onto error screen stack
     * 
     * @param highPriority if the message is a high priority or low priority message
     * @param er the error screen to be pushed back
     * @return true if error pushed back pops an error off the back of the array, otherwise false
     */
    void pushBackError(bool highPriority, const char* id, const char* err);
}