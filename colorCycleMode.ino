#include "colorCycleMode.hpp"
#include <FastLED.h>

float red = 255.0, green = 0.0, blue = 0.0;

Cycle cycle;

void colorCycleMode()
{
    if (red >= 255)
    {
        if (blue > 0)
        {
            blue = max(0, blue - cycle.speed);
        }
        else if (green >= 255)
        {
            red = max(0, red - cycle.speed);
        }
        else
        {
            green = min(255, green + cycle.speed);
        }
    }
    else if (green >= 255)
    {
        if (red > 0)
        {
            red = max(0, red - cycle.speed);
        }
        else if (blue >= 255)
        {
            green = max(0, green - cycle.speed);
        }
        else
        {
            blue = min(255, blue + cycle.speed);
        }
    }
    else if (blue >= 255)
    {
        if (green > 0)
        {
            green = max(0, green - cycle.speed);
        }
        else if (red >= 255)
        {
            blue = max(0, blue - cycle.speed);
        }
        else
        {
            red = min(255, red + cycle.speed);
        }
    }

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds1[i] = CRGB(red, green, blue);
        leds2[i] = CRGB(red, green, blue);
    }
    FastLED.show();
}

void setCycleSpeed(uint8_t speed)
{
    cycle.speed = speed / 50.0;
}