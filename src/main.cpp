#include <WiFi.h>
#include <time.h>
#include "StateMachine.h"
#include "timer.h"
#include "Button.h"


// Define buttons dynamically
const int NUM_BUTTONS = 4;
Button buttons[NUM_BUTTONS] = { Button(27), Button(26), Button(25), Button(33) };  // Assign pins
bool setAlarmFlag = false;

enum ButtonFunctions { SET_ALARM, ADJUST_HOURS, ADJUST_MINUTES, CONFIRM_ALARM };
StateMachine stateMachine(State::S_Init);  // Initialize the state machine

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].begin();
}
  stateMachine.triggerEvent(Event::E_Start);
}

void loop() {
  auto currentState = stateMachine.getCurrentState();

  // Handle Wi-Fi updates
  if (Wifiupdate_flag()) {
    Serial.println("Wi-Fi update triggered.");
    attemptWiFiConnection();
  }

  stateMachine.handleStateActions(currentState);

  //Handle events based on button presses (example)
  if (buttons[SET_ALARM].isPressed() && setAlarmFlag == false) {
    stateMachine.triggerEvent(Event::E_AlarmSet);
    setAlarmFlag = true;
  }

  if (buttons[ADJUST_HOURS].isPressed()) {
      stateMachine.triggerEvent(Event::E_PlusPressed);
  }

  if (buttons[ADJUST_MINUTES].isPressed()) {
      stateMachine.triggerEvent(Event::E_MinusPressed);
  }

  if (buttons[SET_ALARM].isPressed() && setAlarmFlag == true) {
      stateMachine.triggerEvent(Event::E_ConfirmAlarm);
  } 

  // if (/* condition for meditate button */) {
  //   stateMachine.triggerEvent(Event::E_Meditate);
  // }

  // if (/* condition for speech button */) {
  //   stateMachine.triggerEvent(Event::E_Speech);
  // }

  // if (/* condition for wake up from alarm */) {
  //   stateMachine.triggerEvent(Event::E_Wake);
  // }

  delay(100);  // Short delay to prevent CPU overuse
}