#include "staticMode.hpp"
#include <FastLED.h>

Static statMode;

void staticMode()
{
    CRGB color = CRGB(statMode.red, statMode.green, statMode.blue);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds1[i] = color;
        leds2[i] = color;
    }
    FastLED.show();
}

void setStaticRed(uint8_t red)
{
    statMode.red = red;
}

void setStaticGreen(uint8_t green)
{
    statMode.green = green;
}

void setStaticBlue(uint8_t blue)
{
    statMode.blue = blue;
}