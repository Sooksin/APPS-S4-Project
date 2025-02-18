#include "Button.h"

Button* Button::instance = nullptr;

Button::Button()
  : alarmButtonPressed(false),
    plusButtonPressed(false),
    minusButtonPressed(false),
    snoozeButtonPressed(false)
{
}

void Button::begin() {
  instance = this;

  // Set button pins as input with pullup
  pinMode(ALARM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PLUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MINUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SNOOZE_BUTTON_PIN, INPUT_PULLUP);

  // Attach interrupts (FALLING edge since buttons are active LOW)
  attachInterrupt(digitalPinToInterrupt(ALARM_BUTTON_PIN), alarmButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PLUS_BUTTON_PIN), plusButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(MINUS_BUTTON_PIN), minusButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(SNOOZE_BUTTON_PIN), snoozeButtonISR, FALLING);
}

// Getter functions
bool Button::isAlarmButtonPressed() const {
  return alarmButtonPressed;
}

bool Button::isPlusButtonPressed() const {
  return plusButtonPressed;
}

bool Button::isMinusButtonPressed() const {
  return minusButtonPressed;
}

bool Button::isSnoozeButtonPressed() const {
  return snoozeButtonPressed;
}

// Clear flag functions
void Button::clearAlarmButtonFlag() {
  alarmButtonPressed = false;
}

void Button::clearPlusButtonFlag() {
  plusButtonPressed = false;
}

void Button::clearMinusButtonFlag() {
  minusButtonPressed = false;
}

void Button::clearSnoozeButtonFlag() {
  snoozeButtonPressed = false;
}

// Static ISR functions
void IRAM_ATTR Button::alarmButtonISR() {
  if (instance) {
    instance->alarmButtonPressed = true;
  }
}

void IRAM_ATTR Button::plusButtonISR() {
  if (instance) {
    instance->plusButtonPressed = true;
  }
}

void IRAM_ATTR Button::minusButtonISR() {
  if (instance) {
    instance->minusButtonPressed = true;
  }
}

void IRAM_ATTR Button::snoozeButtonISR() {
  if (instance) {
    instance->snoozeButtonPressed = true;
  }
}
