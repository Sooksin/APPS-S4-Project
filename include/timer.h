#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include <time.h>
#include <WiFi.h>

#define MAX_NUM_ALARMS 10

// Function to synchronize time via NTP
void syncTime();
void updateSoftwareRTC();
// Function to get the current time as a formatted string
String getTimeString();

// Function to attempt Wi-Fi connection and sync time
void WiFiEvent(WiFiEvent_t event);
void attemptWiFiConnection();
const unsigned long connectionTimeout = 10000;  // 10 seconds timeout
void wifi_init(void);
bool interval_flag(int interval);
bool Wifiupdate_flag();
int alarm_ring(int *nmr);
void printTime();  // Function to print time every 10s
int checkAlarmRing(int* alarmIndex);  // Function to check for alarm

// NTP settings
const int gmtOff = 3600;  // GMT offset for NTP
const int daylightOffset_sec = 3600;  // Daylight saving time offset

extern bool ntpSynced;  // Declare the external flag for NTP sync

#endif  // TIMER_H
