#include "staticMode.hpp"
#include <FastLED.h>

CRGB color = CRGB::Blue;

void staticMode()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds1[i] = color;
        leds2[i] = color;
    }
    FastLED.show();
}
