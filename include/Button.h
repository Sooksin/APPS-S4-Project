#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
    uint8_t pin;
    bool lastState;
    unsigned long lastDebounceTime;
    static const unsigned long debounceDelay = 50;  // Debounce time in ms

public:
    Button(uint8_t pin);
    void begin();
    bool isPressed();
};

#endif
