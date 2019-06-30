#include "colorCycleMode.hpp"
#include <FastLED.h>

float red = 255.0, green = 0.0, blue = 0.0;

void colorCycleMode()
{
    if (red >= 255)
    {
        if (blue > 0)
        {
            blue -= CYCLESPEED;
        }
        else if (green >= 255)
        {
            red -= CYCLESPEED;
        }
        else
        {
            green += CYCLESPEED;
        }
    }
    else if (green >= 255)
    {
        if (red > 0)
        {
            red -= CYCLESPEED;
        }
        else if (blue >= 255)
        {
            green -= CYCLESPEED;
        }
        else
        {
            blue += CYCLESPEED;
        }
    }
    else if (blue >= 255)
    {
        if (green > 0)
        {
            green -= CYCLESPEED;
        }
        else if (red >= 255)
        {
            blue -= CYCLESPEED;
        }
        else
        {
            red += CYCLESPEED;
        }
    }

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds1[i] = CRGB(red, green, blue);
        leds2[i] = CRGB(red, green, blue);
    }
    FastLED.show();
}