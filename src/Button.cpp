#include "button.h"

Button::Button(uint8_t pin) : pin(pin), lastState(false), lastDebounceTime(0) {}

void Button::begin() {
    pinMode(pin, INPUT_PULLUP);  // Enable internal pull-up resistor
}

bool Button::isPressed() {
    bool currentState = digitalRead(pin) == LOW;  // Button is active LOW
    unsigned long currentTime = millis();

    if (currentState != lastState) {
        lastDebounceTime = currentTime;  // Reset debounce timer
    }

    if ((currentTime - lastDebounceTime) > debounceDelay) {
        if (currentState != lastState) {
            lastState = currentState;
            if (currentState) {
                return true;  // Button press detected
            }
        }
    }

    return false;
}
