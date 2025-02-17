#include "StateMachine.h"
#include "Arduino.h"

StateMachine::StateMachine(State initialState) : currentState(initialState) {}

const Transition StateMachine::transitions[] = {
    {State::S_Alarm, Event::E_Wake, State::S_Idle},
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

// Trigger event to transition between states
void StateMachine::triggerEvent(Event event) {
    for (const auto& transition : transitions) {
        if (transition.from == currentState && transition.event == event) {
            currentState = transition.to;
            handleStateActions(currentState);  // Call action handler when transitioning
            return;
        }
    }
}

// Handle the actions that need to occur when entering or exiting a state
void StateMachine::handleStateActions(State state) {
    switch (state) {
        case State::S_Idle:
            // Display time on screen, this could be a function to update the display
            Serial.println("Entering Idle state, displaying time...");
            break;

        case State::S_Alarm:
            // Activate buzzer and light
            Serial.println("Alarm is ringing! Activating buzzer and light.");
            break;

        case State::S_SetAlarm:
            // Display alarm setting mode
            Serial.println("Entering Set Alarm mode...");
            break;

        case State::S_AdjustHours:
            Serial.println("Adjusting hours...");
            break;

        case State::S_AdjustMinutes:
            Serial.println("Adjusting minutes...");
            break;

        case State::S_Meditate:
            // Start meditation exercise
            Serial.println("Starting meditation...");
            break;

        case State::S_Speech:
            // Start speech exercise
            Serial.println("Starting speech exercise...");
            break;

        default:
            break;
    }
}

