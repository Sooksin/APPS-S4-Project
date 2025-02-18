#ifndef STATEMACHINE_H
#define STATEMACHINE_H

// State enum
enum class State {
    S_Off,
    S_Init,
    S_Idle,
    S_Alarm,
    S_SetAlarm,
    S_AdjustHours,
    S_AdjustMinutes,
    S_Meditate,
    S_Speech
  };
  
  // Event enum
  enum class Event {
    E_Start,
    E_InitComplete,
    E_AlarmSet,
    E_Meditate,
    E_Speech,
    E_Time,
    E_EndMeditate,
    E_EndSpeech,
    E_PlusPressed,
    E_MinusPressed,
    E_AlarmRing,
    E_Wake,
    E_ConfirmAlarm
  };

struct Transition {
    State from;
    Event event;
    State to;
};

class StateMachine {
public:
    StateMachine(State initialState);
    void triggerEvent(Event event);
    void handleStateActions(State state);
    State getCurrentState() const;

private:
    State currentState;
    State lastState;
    static const Transition transitions[20];
    int selectedAlarmTime = 0;  // Stores alarm time in HHMM format
};

#endif // STATEMACHINE_H
