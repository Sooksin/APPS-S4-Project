#ifndef NEOPIXELRING_H
#define NEOPIXELRING_H

#include <Adafruit_NeoPixel.h>

class NeoPixelRing {
public:
    NeoPixelRing(int pin, int numPixels);  // Constructor to initialize with pin and number of pixels

    void begin();  // Initialize the NeoPixel ring
    void setColor(int pixel, int red, int green, int blue);  // Set color for a specific pixel
    void show();  // Display the current color configuration
    void clear();  // Clear the strip (turn off all pixels)
    void fadeToWhite(int steps, int delayTime);  // Fade all pixels to white over specified steps with delay

private:
    Adafruit_NeoPixel strip;  // NeoPixel strip object
};

#endif
