#include <WiFi.h>
#include <time.h>
#include "StateMachine.h"
#include "timer.h"
#include "Arduino.h"
#include "alarmlight.h"
#include "Buzzer.h"
#include "Button.h"

#define LED_PIN 13
#define LED_COUNT 24
#define BUZZER_PIN 14

extern unsigned long rtcStartMillis;  
NeoPixelRing ring(LED_PIN, LED_COUNT);
Buzzer buzzer(BUZZER_PIN);

StateMachine::StateMachine(State initialState) 
    : currentState(initialState), lastState(initialState), stateChanged(true) {}


bool alarmTriggered = false;  // Flag to track if the alarm LED effect was triggered

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

// Handle the actions that need to occur when entering or exiting a state
void StateMachine::handleStateActions(State state) {
    static int tempHours = 0;
    static int tempMinutes = 0;
    static unsigned long lastBeepTime = 0;

    switch (state) {
        case State::S_Init:
            Serial.println("Entering Init state...");
            wifi_init();
            attemptWiFiConnection();
            rtcStartMillis = millis();
            ring.begin();
            ring.clear();
            triggerEvent(Event::E_InitComplete);
            break;

        case State::S_Idle:  
        if (currentState != lastState) {
            Serial.println("Entering Idle mode...");
        }
;            ring.fadeToWhite(200, 200);
            //printTime();
            alarmTriggered = false;
            //ring.clear();
            int alarmIndex;
            if (checkAlarmRing(&alarmIndex) == 1) { 
                triggerEvent(Event::E_AlarmRing);
            }
            break;

        case State::S_Alarm:
            if (!alarmTriggered) {  
                if (currentState != lastState) Serial.println("Alarm is ringing, buzzer is beeping");
                alarmTriggered = true;
            }
             

            // Beep every 5 seconds without blocking execution
            if (millis() - lastBeepTime > 2000) {
                buzzer.beep(500);  
                lastBeepTime = millis();
            }
            buzzer.update();  // Ensure buzzer turns off properly
            break;

        case State::S_SetAlarm:
            if (currentState != lastState) Serial.println("Entering Set Alarm mode...");
            break;

        case State::S_AdjustHours:
        if (currentState != lastState) Serial.println("Adjusting hours");
            break;

        case State::S_AdjustMinutes:
        if (currentState != lastState) Serial.println("Adjusting minutes..");
            break;

        case State::S_Meditate:
        if (currentState != lastState) Serial.println("Entering meditate mode...");
            break;

        case State::S_Speech:
        if (currentState != lastState) Serial.println("Entering speech mode...");
            break;

        default:
            break;
    }
}