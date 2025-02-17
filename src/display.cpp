#include "Display.h"

Display::Display() : tft(TFT_CS, TFT_DC, TFT_RST) {}

void Display::begin() {
    tft.initR(INITR_144GREENTAB);
    tft.fillScreen(ST77XX_BLACK);
    tft.setRotation(1);
}

void Display::showTime(const char* timeStr) {
    tft.fillRect(20, 50, 100, 21, ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(3);
    tft.setCursor(20, 50);
    tft.println(timeStr);
}
