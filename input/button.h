#pragma once
#include <vector>
#include <cstdint>

#ifndef PIN_BUTTON_PAGE
#define PIN_BUTTON_PAGE     0x1F
#endif

typedef void (*buttonCallback)(void);

namespace button {
    struct buttonMomentary {
        uint8_t pin;
        bool pressed;
        bool offHigh;
        buttonCallback pressCallback, releaseCallback;
    };

    extern std::vector<buttonMomentary> buttons_p0, buttons_p1;

    void addButton(uint8_t pin, uint8_t page, buttonCallback onPress, buttonCallback onRelease, bool offHigh);
    void updateButtons();
}