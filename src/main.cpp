#include <WiFi.h>
#include <time.h>
#include "StateMachine.h"
#include "timer.h"

extern unsigned long rtcStartMillis;  // Declare the rtcStartMillis variable here
StateMachine stateMachine(State::S_Idle);  // Initialize the state machine

unsigned long lastTimePrinted = 0;  // Variable to store the last time time was printed

void setup() {
  Serial.begin(115200);

  wifi_init();  // Initialize Wi-Fi
  attemptWiFiConnection();  // Attempt Wi-Fi connection
  rtcStartMillis = millis();  // Start software RTC
}

void loop() {
  // Periodically print time
  printTimePeriodically();

  // Check if an alarm ring occurs
  int alarmIndex;
  if (checkAlarmRing(&alarmIndex) == 1) { 
      stateMachine.triggerEvent(Event::E_AlarmRing);
  }

  // Handle Wi-Fi updates
  if (Wifiupdate_flag()) {
    Serial.println("Wi-Fi update triggered.");
    attemptWiFiConnection();
  }

  // Handle events based on button presses (example)
  if (/* condition for alarm set button */) {
    stateMachine.triggerEvent(Event::E_AlarmSet);
  }

  if (/* condition for meditate button */) {
    stateMachine.triggerEvent(Event::E_Meditate);
  }

  if (/* condition for speech button */) {
    stateMachine.triggerEvent(Event::E_Speech);
  }

  if (/* condition for wake up from alarm */) {
    stateMachine.triggerEvent(Event::E_Wake);
  }

  // Check for the plus/minus button press for hour/minute adjustment
  if (/* condition for plus button */) {
    stateMachine.triggerEvent(Event::E_PlusPressed);
  }

  if (/* condition for minus button */) {
    stateMachine.triggerEvent(Event::E_MinusPressed);
  }

  if (/* condition for minus button */) {
    stateMachine.triggerEvent(Event::E_AlarmRing);
  }

  if (/* condition for minus button */) {
    stateMachine.triggerEvent(Event::E_ConfirmAlarm);
  }

  delay(100);  // Short delay to prevent CPU overuse
}