#pragma once
#include <vector>
#include <cstdint>

typedef void (*buttonCallback)(void);

namespace button {
    struct buttonMomentary {
        uint8_t pin;
        bool pressed;
        bool offHigh;
        buttonCallback pressCallback, releaseCallback;
    };

    extern std::vector<buttonMomentary> buttons;

    void addButton(uint8_t pin, buttonCallback onPress, buttonCallback onRelease, bool offHigh);
    void updateButtons();
}