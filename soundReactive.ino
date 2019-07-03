#include "soundReactive.hpp"
#include "staticMode.hpp"
#include <FastLED.h>

float amplitude = 0;

int frequencies[CHANNEL_COUNT];
float scaling[CHANNEL_COUNT] = {.4, .2, .15, .1, .075, .05, .025}; // Sum should be 1

SoundReactive soundReactive;

// Pull frquencies from Spectrum Shield
void read_Frequencies()
{
    //Read frequencies for each band
    for (uint8_t freq_amp = 0; freq_amp < CHANNEL_COUNT; freq_amp++)
    {
        if (LEFT_RIGHT_CHANNEL_MODE == 0) // Average
            frequencies[freq_amp] = (analogRead(DC_One) + analogRead(DC_Two)) / 2;
        else if (LEFT_RIGHT_CHANNEL_MODE == 1) // Max between left and right
            frequencies[freq_amp] = max(analogRead(DC_One), analogRead(DC_Two));

        digitalWrite(STROBE, HIGH);
        digitalWrite(STROBE, LOW);
    }
}

// This code is used to initialize the shield to split up the frequencies
void initialize_shield()
{
    //Set spectrum Shield pin configurations
    pinMode(STROBE, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(DC_One, INPUT);
    pinMode(DC_Two, INPUT);
    digitalWrite(STROBE, HIGH);
    digitalWrite(RESET, HIGH);

    //Initialize Spectrum Analyzers
    digitalWrite(STROBE, LOW);
    delay(1);
    digitalWrite(RESET, HIGH);
    delay(1);
    digitalWrite(STROBE, HIGH);
    delay(1);
    digitalWrite(STROBE, LOW);
    delay(1);
    digitalWrite(RESET, LOW);
}

void setReactiveMode(uint8_t mode)
{
    soundReactive.reactive_mode = mode;
}

void setReactiveRed(uint8_t red)
{
    soundReactive.red = red;
}

void setReactiveGreen(uint8_t green)
{
    soundReactive.green = green;
}

void setReactiveBlue(uint8_t blue)
{
    soundReactive.blue = blue;
}

/**  
 * Sound reactive Mode
 */
void soundReactiveMode()
{
    read_Frequencies();

    uint16_t newData = 0;

    for (uint8_t i = 0; i < CHANNEL_COUNT; i++)
    {
        // Scale the different frequencies
        newData += scaling[i] * frequencies[i];
    }

    // Get a beautified version of the lowest band
    amplitude = min(
        nextValue(
            newData / SENSITIVITY,
            amplitude),
        NUM_LEDS);

    // Get a beautified version of the lowest band
    drawLeds();
}

// Calculate the next value for the band, this function will smoothen out the graph.
float nextValue(float data, float curr)
{
    data = data - THRESHOLD;

    if (data > curr - 5 && data < curr + (curr / 10))
        return curr - 0.25;
    else if (data > curr)
        return data;
    else
        return curr - curr / RETRACT_RATE;
}

// Method to draw the leds
void drawLeds()
{
    FastLED.clear(); // Turn all leds off
    FastLED.setBrightness(map(amplitude, 0, 150, map(brightness, 0, 255, 0, 20), brightness));

    if (amplitude < 1)
        amplitude = 0;

    if (soundReactive.reactive_mode == 0)
    {
        // Turn the leds on by amplitude
        for (uint8_t i = 0; i < amplitude; i++)
        {
            // Color the leds, more blue at the bottom and more red at the top.
            CRGB color = CRGB(
                map(i, 0, amplitude, 0, 255), // Red
                0,                            // Green
                map(i, 0, amplitude, 255, 0)  // Blue
            );

            leds1[i] = color;
            leds2[i] = color;
        }
    }
    else
    {
        CRGB color = CRGB(soundReactive.red, soundReactive.green, soundReactive.blue);
        for (uint8_t i = 0; i < NUM_LEDS; i++)
        {
            leds1[i] = color;
            leds2[i] = color;
        }
    }

    FastLED.show();
}
