#include "alarmlight.h"

NeoPixelRing::NeoPixelRing(int pin, int numPixels) : strip(numPixels, pin, NEO_GRB + NEO_KHZ800) {}

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
}

void NeoPixelRing::fadeToWhite(int steps, int delayTime) {
    int red = 255, green = 50, blue = 0;  // Even warmer: Deep Amber (R:255, G:80, B:0)

    for (int step = 0; step <= steps; step++) {
        int newBlue = map(step, 0, steps, 0, 255);      // Gradually increase blue
        int newGreen = map(step, 0, steps, 80, 255);    // Gradually adjust green
        int newRed = map(step, 0, steps, 255, 255);     // Keep red at max
        
        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(newRed, newGreen, newBlue));
        }
        strip.show();
        delay(delayTime);  // Slows down the transition
    }
}