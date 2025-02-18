#include "alarmlight.h"

NeoPixelRing::NeoPixelRing(int pin, int numPixels) : strip(numPixels, pin, NEO_GRB + NEO_KHZ800) {
    fadeActive = false;
    fadeStep = 0;
    lastFadeUpdate = 0;
}

void NeoPixelRing::begin() {
    strip.begin();
    strip.show();  // Initialize all pixels to 'off'
}

void NeoPixelRing::setColor(int pixel, int red, int green, int blue) {
    if (pixel >= 0 && pixel < strip.numPixels()) {
        strip.setPixelColor(pixel, strip.Color(red, green, blue));
    }
}

void NeoPixelRing::show() {
    strip.show();
}

void NeoPixelRing::clear() {
    strip.clear();
    strip.show();
    fadeActive = false;  // Stop fade if it's running
}

void NeoPixelRing::startFadeToWhite(int steps, int delayTime) {
    fadeSteps = steps;
    fadeDelay = delayTime;
    fadeStep = 0;
    fadeActive = true;
    lastFadeUpdate = millis();  // Start timer
}

// Call this function in the main loop to update the fade effect
void NeoPixelRing::fadeToWhiteStep() {
    if (!fadeActive) return; // Only proceed if fading is active

    unsigned long currentMillis = millis();
    if (currentMillis - lastFadeUpdate >= fadeDelay) {
        lastFadeUpdate = currentMillis; // Update time reference

        // Compute new color values
        int newBlue = map(fadeStep, 0, fadeSteps, 0, 255);
        int newGreen = map(fadeStep, 0, fadeSteps, 80, 255);
        int newRed = 255; // Red stays max

        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(newRed, newGreen, newBlue));
        }
        strip.show();

        fadeStep++;
        if (fadeStep > fadeSteps) {
            fadeActive = false; // Stop fading when finished
        }
    }
}
