#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
public:
    Buzzer(int pin);
    void beep(int duration);
    void update();

private:
    int buzzerPin;
    unsigned long beepEndTime;
    bool isBeeping;
};

#endif
