#include "dashstorage.h"
#include <cstring>

DashStorage::_DataStruct    DashStorage::dataStruct = {};
DashStorage::_DriverScreen  DashStorage::driverScreen = {0, 0, 0};
DashStorage::_ErrorStack    DashStorage::errorStacks = {};

void DashStorage::pushBackError(bool highPriority, const char* id, const char* err){
    if(highPriority) {
        errorStacks.hpErrorStack[errorStacks.hpErrorPointer].errorPointer = err;
        strcpy(errorStacks.hpErrorStack[errorStacks.hpErrorPointer].id, id);
        errorStacks.hpErrorPointer++;

        if(errorStacks.hpErrorPointer == DASH_HP_MAX_ERRORS) errorStacks.hpfull = 1;
        errorStacks.hpErrorPointer %= DASH_HP_MAX_ERRORS;
    }
    else {
        errorStacks.lpErrorStack[errorStacks.lpErrorPointer].errorPointer = err;
        strcpy(errorStacks.lpErrorStack[errorStacks.lpErrorPointer].id, id);
        errorStacks.lpErrorPointer++;

        if(errorStacks.lpErrorPointer == DASH_HP_MAX_ERRORS) errorStacks.lpfull = 1;
        errorStacks.lpErrorPointer %= DASH_HP_MAX_ERRORS;
    }
}