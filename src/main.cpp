#include <WiFi.h>
#include <time.h>
#include "StateMachine.h"
#include "timer.h"

StateMachine stateMachine(State::S_Init);  // Initialize the state machine

void setup() {
  Serial.begin(115200);
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

  // Handle events based on button presses (example)
  // if (/* condition for alarm set button */) {
  //   stateMachine.triggerEvent(Event::E_AlarmSet);
  // }

  // if (/* condition for meditate button */) {
  //   stateMachine.triggerEvent(Event::E_Meditate);
  // }

  // if (/* condition for speech button */) {
  //   stateMachine.triggerEvent(Event::E_Speech);
  // }

  // if (/* condition for wake up from alarm */) {
  //   stateMachine.triggerEvent(Event::E_Wake);
  // }

  // // Check for the plus/minus button press for hour/minute adjustment
  // if (/* condition for plus button */) {
  //   stateMachine.triggerEvent(Event::E_PlusPressed);
  // }

  // if (/* condition for minus button */) {
  //   stateMachine.triggerEvent(Event::E_MinusPressed);
  // }

  // if (/* condition for minus button */) {
  //   stateMachine.triggerEvent(Event::E_ConfirmAlarm);
  // }

  delay(100);  // Short delay to prevent CPU overuse
}