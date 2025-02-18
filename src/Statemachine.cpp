#include <WiFi.h>
#include <time.h>
#include "StateMachine.h"
#include "timer.h"
#include "Arduino.h"
#include "alarmLight.h"
#include "Buzzer.h"
#include "Button.h"
#include "display.h"

#define LED_PIN 4
#define LED_COUNT 24
#define BUZZER_PIN 14

extern unsigned long rtcStartMillis;  
NeoPixelRing ring(LED_PIN, LED_COUNT);
Buzzer buzzer(BUZZER_PIN);

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
Display display(tft);

StateMachine::StateMachine(State initialState) 
    : currentState(initialState), lastState(initialState), stateChanged(true) {}


bool alarmTriggered = false;  // Flag to track if the alarm LED effect was triggered

bool adjustingHour = true;
bool adjustingMinute = false;
bool blinkHour = false;
bool blinkMinute = false;
unsigned long lastBlinkTime = 0;
const int blinkInterval = 500; // Blink every 500ms
bool alarmActive = true; // Set this to true if alarm is enabled


const Transition StateMachine::transitions[] = {
    {State::S_Off, Event::E_Start, State::S_Idle},
    {State::S_Init, Event::E_InitComplete, State::S_Idle},   
    {State::S_Alarm, Event::E_Wake, State::S_Idle},
    {State::S_Idle, Event::E_AlarmRing, State::S_Alarm},
    {State::S_Idle, Event::E_AlarmSet, State::S_SetAlarm},
    {State::S_SetAlarm, Event::E_PlusPressed, State::S_AdjustHours},
    {State::S_SetAlarm, Event::E_MinusPressed, State::S_AdjustMinutes},
    {State::S_AdjustHours, Event::E_ConfirmAlarm, State::S_Idle},
    {State::S_AdjustMinutes, Event::E_ConfirmAlarm, State::S_Idle},
    {State::S_SetAlarm, Event::E_ConfirmAlarm, State::S_Idle},
    {State::S_Idle, Event::E_Meditate, State::S_Meditate},
    {State::S_Meditate, Event::E_EndMeditate, State::S_Idle},
    {State::S_Idle, Event::E_Speech, State::S_Speech},
    {State::S_Speech, Event::E_EndSpeech, State::S_Idle},
    {State::S_Idle, Event::E_AlarmSet, State::S_SetAlarm}
};

State StateMachine::getCurrentState() const {
    return currentState;
}

State StateMachine::getLastState() const {
    return lastState;
}

// Trigger event to transition between states
void StateMachine::triggerEvent(Event event) {
    for (const auto& transition : transitions) {
        if (transition.from == currentState && transition.event == event) {
            lastState = currentState;  // Store the last state before transitioning
            currentState = transition.to;
            handleStateActions(currentState);  // Call action handler only when state changes
            return;
        }
    }
}

void StateMachine::handleStateActions(State state) {
    static int tempHours = 0;
    static int tempMinutes = 0;
    static unsigned long lastBeepTime = 0;

    switch (state) {
        case State::S_Init: {
            Serial.println("Entering Init state...");
            wifi_init();
            attemptWiFiConnection();
            rtcStartMillis = millis();
            ring.begin();
            ring.clear();
            display.begin();
            triggerEvent(Event::E_InitComplete);
            break;
        }

        case State::S_Idle: { 
            if (currentState != lastState) {
                Serial.println("Entering Idle mode...");
            }
            printTime();

            const char* timeChar = getTimeString();
            display.showTime(timeChar);
            alarmTriggered = false;
            int alarmIndex;
            if (checkAlarmRing(&alarmIndex) == 1) { 
                triggerEvent(Event::E_AlarmRing);
            }
            break;
        }

        case State::S_Alarm: {
            if (!alarmTriggered) {  
                if (currentState != lastState) Serial.println("Alarm is ringing, buzzer is beeping");
                alarmTriggered = true;
            }
            if (millis() - lastBeepTime > 2000) {
                buzzer.beep(500);  
                lastBeepTime = millis();
            }
            buzzer.update();
            break;
        }

        case State::S_SetAlarm: {
            if (currentState != lastState) Serial.println("Set alarm entered");
              
            if (adjustingHour) {
                if (millis() - lastBlinkTime > blinkInterval) {
                    blinkHour = !blinkHour; // Toggle blink state for hour
                    lastBlinkTime = millis();
                }
            }
            const char* timeChar = getTimeString();
            display.showTimeBlink(timeChar, blinkHour, blinkMinute, alarmActive);   
            break;
        }

        case State::S_AdjustHours: {
            if (currentState != lastState) Serial.println("Adjusting hours");
            if (adjustingHour) {
                if (millis() - lastBlinkTime > blinkInterval) {
                    blinkHour = !blinkHour; // Toggle blink state for hour
                    lastBlinkTime = millis();
                }
            }
            const char* timeChar = getTimeString();
            display.showTimeBlink(timeChar, blinkHour, blinkMinute, alarmActive);
            break;
        }

        case State::S_AdjustMinutes: {
            if (currentState != lastState) Serial.println("Adjusting minutes..");
            if (adjustingMinute) {
                if (millis() - lastBlinkTime > blinkInterval) {
                    blinkMinute = !blinkMinute; // Toggle blink state for minute
                    lastBlinkTime = millis();
                }
            }
            const char* timeChar = getTimeString();
            display.showTimeBlink(timeChar, blinkHour, blinkMinute, alarmActive);
            break;
        }

        case State::S_Meditate: {
            if (currentState != lastState) Serial.println("Entering meditate mode...");
            break;
        }

        case State::S_Speech: {
            if (currentState != lastState) Serial.println("Entering speech mode...");
            break;
        }

        default:
            break;
    }
}
