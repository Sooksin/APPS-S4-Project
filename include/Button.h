#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

#define ALARM_BUTTON_PIN   27   // Example pin numbers, modify as needed
#define PLUS_BUTTON_PIN    26
#define MINUS_BUTTON_PIN   25
#define SNOOZE_BUTTON_PIN  33

class Button {
public:
  // Constructor
  Button();

  // Initialization method
  void begin();

  // Getter functions to check if buttons were pressed
  bool isAlarmButtonPressed() const;
  bool isPlusButtonPressed() const;
  bool isMinusButtonPressed() const;
  bool isSnoozeButtonPressed() const;

  // Methods to clear the flags after processing
  void clearAlarmButtonFlag();
  void clearPlusButtonFlag();
  void clearMinusButtonFlag();
  void clearSnoozeButtonFlag();

private:
  // Flags to track button presses
  volatile bool alarmButtonPressed;
  volatile bool plusButtonPressed;
  volatile bool minusButtonPressed;
  volatile bool snoozeButtonPressed;

  // Static ISR helper functions
  static void IRAM_ATTR alarmButtonISR();
  static void IRAM_ATTR plusButtonISR();
  static void IRAM_ATTR minusButtonISR();
  static void IRAM_ATTR snoozeButtonISR();

  // Static instance of the class
  static Button* instance;
};

#endif // BUTTON_H
