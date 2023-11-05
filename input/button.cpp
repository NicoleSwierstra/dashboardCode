#include "button.h"
#include <vector>
#include <Arduino.h>

std::vector<button::buttonMomentary> button::buttons = std::vector<button::buttonMomentary>();

void button::addButton(uint8_t pin, buttonCallback onPress, buttonCallback onRelease, bool offHigh){
    buttons.push_back(buttonMomentary{pin, 0, offHigh, onPress, onRelease});
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
}

void button::updateButtons(){
    for(buttonMomentary& b : buttons){
        bool on = b.offHigh ? !digitalRead(b.pin) : digitalRead(b.pin);

        if(on && !b.pressed){
            b.pressed = true;
            if(b.pressCallback != nullptr)
                b.pressCallback();
        }
        else if(!on){
            b.pressed = false;
            if(b.releaseCallback != nullptr)
                b.releaseCallback();
        }
    }
}