#include "timer.h"

// ===================== GLOBAL VARIABLES =====================
unsigned long rtcStartMillis = 0;      // RTC start timestamp
bool ntpSynced = false;                // Flag for NTP sync status
struct tm softwareRTC = { 0 };         // Local software RTC
const char *ntp_s = "pool.ntp.org";   // NTP server
const char *ssid_STA = "STIJN 9916";  // Wi-Fi SSID
const char *password_STA = "1234567890"; // Wi-Fi Password
char wifiname[20] = "APPS_Buddy";

unsigned long prev_sec;
unsigned long lastTimePrinted = 0;  // Store last printed time

// Alarm storage
int alarms[MAX_NUM_ALARMS] = {};  // Example alarms
int alarmCount = 5;  // Current number of alarms


// Add a new alarm (time format: HHMM)
bool addAlarm(int time) {
    if (alarmCount >= MAX_NUM_ALARMS) {
        Serial.println("Alarm list is full!");
        return false;
    }

    // Check if alarm already exists
    for (int i = 0; i < alarmCount; i++) {
        if (alarms[i] == time) {
            Serial.println("Alarm already set!");
            return false;
        }
    }

    alarms[alarmCount] = time;
    alarmCount++;
    Serial.print("Alarm added: ");
    Serial.println(time);
    return true;
}

// Remove an existing alarm
bool removeAlarm(int time) {
    for (int i = 0; i < alarmCount; i++) {
        if (alarms[i] == time) {
            for (int j = i; j < alarmCount - 1; j++) {
                alarms[j] = alarms[j + 1]; // Shift elements
            }
            alarmCount--;
            Serial.print("Alarm removed: ");
            Serial.println(time);
            return true;
        }
    }
    Serial.println("Alarm not found!");
    return false;
}

// Retrieve current alarms as a formatted string
String getAlarms() {
    if (alarmCount == 0) {
        return "No alarms set";
    }

    String alarmList = "Alarms: ";
    for (int i = 0; i < alarmCount; i++) {
        int hours = alarms[i] / 100;
        int minutes = alarms[i] % 100;
        alarmList += String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes);
        if (i < alarmCount - 1) {
            alarmList += ", ";
        }
    }
    return alarmList;
}

// Function to check for millis on a specific interval (replaces the need to have large if statements in the state machine)
bool interval_flag(int interval) {
  unsigned long cur_sec = millis();
  if ((cur_sec - prev_sec) > interval) {
    prev_sec = cur_sec; 
    return true;
  }
  return false;
}

// Function to set Wi-Fi name
void wifi_init() {
  WiFi.setHostname(wifiname);
}


void printTime() {
      String currentTime = getTimeString();
      Serial.println("Current Time: " + currentTime);
}

// Function to check for an alarm ring
int checkAlarmRing(int* alarmIndex) {
  return alarm_ring(alarmIndex);  // Call existing alarm function
}

bool Wifiupdate_flag() { // Sets a flag when Wi-Fi should update
  int interval = 300000; // 5 minutes
  unsigned long cur_sec = millis();
  if ((cur_sec - prev_sec) > interval) {
    prev_sec = cur_sec; 
    return true;
  }
  return false;
}

int alarm_ring(int *nmr) { // Function to check alarms against the current time
  time_t now = time(nullptr);
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
    return -1; // Couldn't obtain time
  }
  int hours = timeInfo.tm_hour;
  int minutes = timeInfo.tm_min;

  for (int i = 0; i < MAX_NUM_ALARMS; i++) {
    if (alarms[i] == 0) { return 0; } // No alarm was set
    if ((hours == (alarms[i] / 100)) && (minutes == (alarms[i] % 100))) { // Check if an alarm matches the current time
      *nmr = i; // Set the number to give the alarm ringing
      return 1;  // Alarm is ringing!!
    }
  }
  return 0; // No alarm is ringing
}

// Function to increment the software RTC manually
void updateSoftwareRTC() {
  unsigned long currentMillis = millis();
  unsigned long elapsedSeconds = (currentMillis - rtcStartMillis) / 1000;

  // Update the software RTC by incrementing the seconds, minutes, and hours
  softwareRTC.tm_sec = elapsedSeconds % 60;
  softwareRTC.tm_min = (elapsedSeconds / 60) % 60;
  softwareRTC.tm_hour = (elapsedSeconds / 3600) % 24;
}

// Function to sync time via NTP
void syncTime() {
  configTime(gmtOff, daylightOffset_sec, ntp_s);  // Set the time zone and NTP server
  struct tm timeInfo;
  if (getLocalTime(&timeInfo)) {
    Serial.println("NTP time sync successful");
    ntpSynced = true;  // Set the flag to true once NTP is synced
  } else {
    Serial.println("Failed to synchronize with NTP");
  }
}

char* getTimeString() {
  static char timeString[6];  // "HH:MM" + null terminator
  struct tm timeInfo;

  // Check if time has been synced with NTP
  if (!ntpSynced) {
      updateSoftwareRTC();
      timeInfo = softwareRTC;
  } else {
      if (!getLocalTime(&timeInfo)) {
          return (char*)"Failed";
      }
  }

  // Format only HH:MM
  strftime(timeString, sizeof(timeString), "%H:%M", &timeInfo);
  return timeString;
}
  

// Function to attempt Wi-Fi connection
void attemptWiFiConnection() {
  // Register the event handler to handle Wi-Fi events
  WiFi.onEvent(WiFiEvent);

  // Start the Wi-Fi connection process
  WiFi.begin(ssid_STA, password_STA);
  Serial.println("Attempting to connect to external Wi-Fi...");
}

// Wi-Fi event handler
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("Connected to Wi-Fi. IP address: ");
      Serial.println(WiFi.localIP());
      // Sync time after successful connection
      syncTime();
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("Wi-Fi disconnected, attempting to reconnect...");
      WiFi.disconnect();
      WiFi.begin(ssid_STA, password_STA);
      break;

    default:
      break;
  }
}