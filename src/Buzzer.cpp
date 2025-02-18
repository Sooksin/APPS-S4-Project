#include "Buzzer.h"

Buzzer::Buzzer(int pin) {
    buzzerPin = pin;
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
    isBeeping = false;
}

void Buzzer::beep(int duration) {
    beepEndTime = millis() + duration;
    isBeeping = true;
    digitalWrite(buzzerPin, HIGH);
}

void Buzzer::update() {
    if (isBeeping && millis() >= beepEndTime) {
        digitalWrite(buzzerPin, LOW);
        isBeeping = false;
    }
}
