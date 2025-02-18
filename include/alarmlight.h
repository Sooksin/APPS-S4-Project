#ifndef ALARMLIGHT_H
#define ALARMLIGHT_H

#include <Adafruit_NeoPixel.h>

class NeoPixelRing {
public:
    NeoPixelRing(int pin, int numPixels);
    void begin();
    void setColor(int pixel, int red, int green, int blue);
    void show();
    void clear();
    
    // Non-blocking fade function
    void startFadeToWhite(int steps, int delayTime);
    void fadeToWhiteStep();  // Call this in loop()

private:
    Adafruit_NeoPixel strip;
    bool fadeActive;
    int fadeSteps, fadeStep, fadeDelay;
    unsigned long lastFadeUpdate;
};

#endif
