#include "buttons.h"

AlarmInput* AlarmInput::instance = nullptr;

AlarmInput::AlarmInput()
  : settingState(NORMAL),
    alarmHour(0),
    alarmMinute(0),
    confirmed(false),
    snoozeRequested(false)
{
}

void AlarmInput::begin() {
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

uint16_t AlarmInput::getAlarmTime() const {
  return alarmHour * 100 + alarmMinute;
}

bool AlarmInput::isConfirmed() const {
  return confirmed;
}

void AlarmInput::clearConfirmed() {
  confirmed = false;
}

bool AlarmInput::isSnoozeRequested() const {
  return snoozeRequested;
}

void AlarmInput::clearSnoozeRequested() {
  snoozeRequested = false;
}

AlarmInput::SettingState AlarmInput::getSettingState() const {
  return settingState;
}

// Static ISR functions
void IRAM_ATTR AlarmInput::alarmButtonISR() {
  if (instance) {
    instance->handleAlarmButton();
  }
}

void IRAM_ATTR AlarmInput::plusButtonISR() {
  if (instance) {
    instance->handlePlusButton();
  }
}

void IRAM_ATTR AlarmInput::minusButtonISR() {
  if (instance) {
    instance->handleMinusButton();
  }
}

void IRAM_ATTR AlarmInput::snoozeButtonISR() {
  if (instance) {
    instance->handleSnoozeButton();
  }
}

// ISR Helper functions
void AlarmInput::handleAlarmButton() {
  if(settingState == NORMAL) {
    settingState = SETTING_HOUR;
    alarmHour = 0;
  }
  else if(settingState == SETTING_HOUR) {
    settingState = SETTING_MINUTE;
    alarmMinute = 0;
  }
  else if(settingState == SETTING_MINUTE) {
    confirmed = true;
    settingState = NORMAL;
  }
}

void AlarmInput::handlePlusButton() {
  if(settingState == SETTING_HOUR) {
    alarmHour = (alarmHour + 1) % 24;
  }
  else if(settingState == SETTING_MINUTE) {
    alarmMinute = (alarmMinute + 1) % 60;
  }
}

void AlarmInput::handleMinusButton() {
  if(settingState == SETTING_HOUR) {
      alarmHour = (alarmHour == 0) ? 23 : alarmHour - 1;
  }
  else if(settingState == SETTING_MINUTE) {
    alarmMinute = (alarmMinute == 0) ? 59 : alarmMinute - 1;
  }
}

void AlarmInput::handleSnoozeButton() {
  snoozeRequested = true;
}