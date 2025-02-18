#ifndef BUTTONS_H
#define BUTTONS_H

#include <arduino.h>

class AlarmInput {
  public:
    enum SettingState{
      NORMAL,
      SETTING_HOUR,
      SETTING_MINUTE,
    };

    AlarmInput();

    // Call from setup to initialize
    void begin();
    
    // Get the alarm time as integer in HHMM format
    uint16_t getAlarmTime() const;

    // Returns true if alarm button was pressed
    bool isConfirmed() const;

    // Clear the confirmation flag
    void clearConfirmed();

    bool isSnoozeRequested() const;

    void clearSnoozeRequested();

    // Get current internal state of alarm input
    SettingState getSettingState() const;

  private:
    volatile uint8_t alarmHour;       // Stored hour (0-23)
    volatile uint8_t alarmMinute;     // Stored minute (0-59)
    volatile SettingState settingState;
    volatile bool confirmed;
    volatile bool snoozeRequested;    // Flag to be set by snooze button

    // Pin assignments
    static const uint8_t ALARM_BUTTON_PIN = 13;
    static const uint8_t PLUS_BUTTON_PIN = 12;
    static const uint8_t MINUS_BUTTON_PIN = 14;
    static const uint8_t SNOOZE_BUTTON_PIN = 27;

    // ISR
    static void IRAM_ATTR alarmButtonISR();
    static void IRAM_ATTR plusButtonISR();
    static void IRAM_ATTR minusButtonISR();
    static void IRAM_ATTR snoozeButtonISR();

    // Helper functions called from ISR
    void handleAlarmButton();
    void handlePlusButton();
    void handleMinusButton();
    void handleSnoozeButton();

    // Pointer
    static AlarmInput* instance;
};

#endif