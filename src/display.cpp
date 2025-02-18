#include "Display.h"

Display::Display(Adafruit_ST7735& tftRef) : tft(tftRef) {}

void Display::begin() {
    tft.initR(INITR_144GREENTAB);
    tft.fillScreen(ST77XX_BLACK);
    tft.setRotation(1);
}

void Display::clearScreen() {
    tft.fillScreen(ST77XX_BLACK);
}

void Display::showTime(const char* timeStr) {
    tft.fillRect(5, 50, 150, 30, ST77XX_BLACK); // Clear previous time
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(4);
    tft.setCursor(5, 50);
    tft.println(timeStr);
}

void Display::showTimeBlink(const char* timeStr, bool blinkHour, bool blinkMinute, bool alarmActive) {
    char displayStr[10];
    strncpy(displayStr, timeStr, sizeof(displayStr));
    
    if (blinkHour) {
        displayStr[0] = ' ';
        displayStr[1] = ' ';
    }
    if (blinkMinute) {
        displayStr[3] = ' ';
        displayStr[4] = ' ';
    }

    showTime(displayStr);
    drawAlarmIcon(alarmActive);
}

void Display::drawAlarmIcon(bool alarmActive) {
    if (alarmActive) {
        tft.drawCircle(10, 10, 6, ST77XX_WHITE);
        tft.drawCircle(10, 10, 5, ST77XX_WHITE);
        tft.drawLine(10, 10, 10, 6, ST77XX_WHITE);
        tft.drawLine(10, 10, 14, 10, ST77XX_WHITE);
        tft.fillTriangle(4, 4, 6, 2, 8, 4, ST77XX_WHITE);
        tft.fillTriangle(12, 4, 14, 2, 16, 4, ST77XX_WHITE);
    } else {
        tft.fillRect(2, 2, 16, 16, ST77XX_BLACK); // Clear previous alarm icon
    }
}
