#include <WiFi.h>
#include <time.h>
#include "StateMachine.h"
#include "timer.h"
#include "Button.h"
#include "stdio.h"

Button button;    //instance of the button class
StateMachine stateMachine(State::S_Init);  // Initialize the state machine
bool setAlarmFlag = false;

int tempHours = 0;
int tempMinutes = 0;

void setup() {
  Serial.begin(115200);
  stateMachine.triggerEvent(Event::E_Start);
  button.begin();
}

void loop() {
  auto currentState = stateMachine.getCurrentState();
  auto lastState = stateMachine.getLastState();

  stateMachine.handleStateActions(currentState);
  
  // Handle Wi-Fi updates
  if (Wifiupdate_flag()) {
    Serial.println("Wi-Fi update triggered.");
    attemptWiFiConnection();
  }

// Handle alarm setting process
if (button.isAlarmButtonPressed()) {
  if (!setAlarmFlag) {
      stateMachine.triggerEvent(Event::E_AlarmSet);
      setAlarmFlag = true;
      tempHours = 0;
      tempMinutes = 0;
  } else {
      stateMachine.triggerEvent(Event::E_ConfirmAlarm);
      setAlarmFlag = false;
      
      char alarmStr[5];  // Buffer for "HHMM" (4 digits + null terminator)
      sprintf(alarmStr, "%02d%02d", tempHours, tempMinutes);  // Ensures leading zeros

      int alarm = atoi(alarmStr);  // Convert "HHMM" string to an integer (0905 → 905)
      
      Serial.print("Alarm set for: ");
      Serial.println(alarm);  // Prints correctly formatted HHMM
  }
  button.clearAlarmButtonFlag();
}

  if (button.isSnoozeButtonPressed() && setAlarmFlag == true) {
    stateMachine.triggerEvent(Event::E_Wake);
    button.clearSnoozeButtonFlag();
  }

    // Adjust hours and minutes only when setting alarm
    if (setAlarmFlag) {
      if (button.isMinusButtonPressed()) {
          tempHours = (tempHours + 1) % 24;  // Loop hours 0-23
          Serial.print("Temp Hours: ");
          Serial.println(tempHours);
          button.clearMinusButtonFlag();
          stateMachine.triggerEvent(Event::E_MinusPressed);          
      }

      if (button.isPlusButtonPressed()) {
          tempMinutes = (tempMinutes + 1) % 60;  // Loop minutes 0-59
          Serial.print("Temp Minutes: ");
          Serial.println(tempMinutes);
          button.clearPlusButtonFlag();
          stateMachine.triggerEvent(Event::E_PlusPressed);  
      }
  }

  // if (/* condition for meditate button */) {
  //   stateMachine.triggerEvent(Event::E_Meditate);
  // }

  // if (/* condition for speech button */) {
  //   stateMachine.triggerEvent(Event::E_Speech);
  // }

  delay(100);  // Short delay to prevent CPU overuse
}