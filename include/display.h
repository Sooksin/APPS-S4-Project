/*

bool adjustingHour = true;
bool adjustingMinute = false;
bool blinkHour = false;
bool blinkMinute = false;
unsigned long lastBlinkTime = 0;
const int blinkInterval = 500; // Blink every 500ms
bool alarmActive = true; // Set this to true if alarm is enabled

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
Display display(tft);

void displayTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }

    char timeStr[10];
    strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    
    if (adjustingHour) {
        if (millis() - lastBlinkTime > blinkInterval) {
            blinkHour = !blinkHour; // Toggle blink state for hour
            lastBlinkTime = millis();
        }
    }
     if (adjustingMinute) {
        if (millis() - lastBlinkTime > blinkInterval) {
            blinkMinute = !blinkMinute; // Toggle blink state for minute
            lastBlinkTime = millis();
        }
    }

    display.showTimeBlink(timeStr, blinkHour, blinkMinute, alarmActive);
    Serial.print("Current Time: ");
    Serial.println(timeStr);
}

*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS     5   // Chip Select
#define TFT_RST    4   // Reset
#define TFT_DC     2   // Data/Command

class Display {
public:
    Display(Adafruit_ST7735& tft); // Dependency Injection
    void begin();
    void showTime(const char* timeStr);
    void showTimeBlink(const char* timeStr, bool blinkHour, bool blinkMinute, bool alarmActive);
    void clearScreen();

private:
    Adafruit_ST7735& tft; // Reference to the display object
    void drawAlarmIcon(bool alarmActive);
};

#endif
