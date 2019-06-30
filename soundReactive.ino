#include "soundReactive.hpp"
#include <FastLED.h>

float bass = 0;

int frequencies_One[7];
int frequencies_Two[7];

/*************Pull frquencies from Spectrum Shield****************/
void read_Frequencies()
{
    //Read frequencies for each band
    for (int freq_amp = 0; freq_amp < 7; freq_amp++)
    {
        frequencies_One[freq_amp] = analogRead(DC_One);
        frequencies_Two[freq_amp] = analogRead(DC_Two);
        digitalWrite(STROBE, HIGH);
        digitalWrite(STROBE, LOW);
    }
}

/**  
 * Sound reactive Mode
 */
void soundReactive()
{
    read_Frequencies();

    int newData = 0;

    for (int i = 0; i < 7; i++)
    {
        // For now only using bass
        if (i == 0)
        {
            newData = max(frequencies_Two[i], frequencies_One[i]);
        }
    }

    // Get a beautified version of the lowest band
    bass = min(nextValue(newData / 3, bass), 150);
    Serial.println(bass);

    // Get a beautified version of the lowest band
    drawLeds();
}

// Calculate the next value for the band, this function will smoothen out the graph.
float nextValue(float data, float band)
{
    data = data - 23;
    if (data > band)
    {
        return data;
    }
    else
    {
        return band - band / 50;
    }
}

// Method to draw the leds
void drawLeds()
{
    FastLED.clear(); // Turn all leds off

    FastLED.setBrightness(map(bass, 0, 150, 20, brightness));

    if (bass < 1)
        bass = 0;

    // Turn the leds on by bass on strip 1
    for (int i = 0; i < bass; i++)
    {
        // Color the leds, more blue at the bottom and more red at the top.
        leds1[i] = CRGB(
            map(i, 0, bass, 0, 255), // Red
            0,                       // Green
            map(i, 0, bass, 255, 0)  // Blue
        );
    }

    // Turn the leds on by bass on strip 2
    for (int i = 0; i < bass; i++)
    {
        // Color the leds, more blue at the bottom and more red at the top.
        leds2[i] = CRGB(
            map(i, 0, bass, 0, 255), // Red
            0,                       // Green
            map(i, 0, bass, 255, 0)  // Blue
        );
    }

    FastLED.show();
}