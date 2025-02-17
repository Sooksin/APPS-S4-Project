#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS     5   // Chip Select
#define TFT_RST    4   // Reset
#define TFT_DC     2   // Data/Command
#define TFT_MOSI  23   // SPI MOSI
#define TFT_SCLK  18   // SPI Clock

class Display {
public:
    Display();
    void begin();
    void showTime(const char* timeStr);

private:
    Adafruit_ST7735 tft;
};

#endif