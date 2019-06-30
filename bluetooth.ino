#include "bluetooth.hpp"
#include <FastLED.h>

uint8_t data_counter = 0;

/**
 * This is responsible for communication with the bluetooth module
 */
void bluetooth_settings()
{
    // Check if there is data available and it is not 10 (since 10) always gets send right after data
    if (Serial.available())
    {
        uint8_t b = Serial.read();

        // check if this is the first byte sent
        if (data_counter == 0)
            setMode(b);
        else if (data_counter == 1)
            setDataBrightness(b);

        // update the data_counter
        if (data_counter < MAX_DATA_COUNT)
            data_counter++;
        else
            data_counter = 0; // reset the datacounter

        Serial.println(b);
    }
}

// this will set the mode to the incoming byte
void setMode(uint8_t b)
{
    if (b >= 0 && b < 4)
        mode = b;

    FastLED.clear(); // Reset the leds
}

// set the brightness of the to the incoming byte
void setDataBrightness(uint8_t b)
{
    brightness = b;

    FastLED.setBrightness(brightness);
}
