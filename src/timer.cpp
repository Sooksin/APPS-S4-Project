#include "timer.h"

// Variables for software RTC (starting at 00:00:00)
unsigned long rtcStartMillis = 0;      // The time when the software RTC started
bool ntpSynced = false;                // Flag to check if NTP has been synced
struct tm softwareRTC = { 0 };         // Software RTC time structure
const char *ntp_s = "pool.ntp.org";   // NTP server
const char *ssid_STA = "STIJN 9916"; // Wi-Fi SSID
const char *password_STA = "1234567890"; // Wi-Fi password
char wifiname[20] = "APPS_Buddy";
unsigned long prev_sec;
int alarms[MAX_NUM_ALARMS] = {1525, 1140, 2300, 0600, 1335};  // Example alarms
unsigned long lastTimePrinted = 0;  // Store the last time the time was printed

// Function to set Wi-Fi name
void wifi_init() {
  WiFi.setHostname(wifiname);
}

void printTimePeriodically() {
  if (millis() - lastTimePrinted >= 10000) {  // Every 10 seconds
      lastTimePrinted = millis();
      String currentTime = getTimeString();
      Serial.println("Current Time: " + currentTime);
  }
}

// Function to check for an alarm ring
int checkAlarmRing(int* alarmIndex) {
  return alarm_ring(alarmIndex);  // Call existing alarm function
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

String getTimeString() {
    struct tm timeInfo;
  
    // Check if time has been synced with NTP
    if (!ntpSynced) {
      // If not synced, use the software RTC
      updateSoftwareRTC();
      timeInfo = softwareRTC;
    } else {
      // Use the NTP-synced time if available
      if (!getLocalTime(&timeInfo)) {
        return "Failed to obtain time";
      }
    }
  
    // Print raw time values for debugging
    Serial.print("Time: ");
    Serial.print(timeInfo.tm_hour);
    Serial.print(":");
    Serial.print(timeInfo.tm_min);
    Serial.print(":");
    Serial.println(timeInfo.tm_sec);
  
    char timeString[26];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return String(timeString);
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
