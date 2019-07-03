#include "bluetooth.hpp"
#include "soundReactive.hpp"
#include "starMode.hpp"
#include "staticMode.hpp"
#include "colorCycleMode.hpp"
#include <FastLED.h>

uint8_t data_counter = 0;
uint8_t byte_buffer[MAX_DATA_COUNT];

unsigned long timeStart = 0;
bool timerRunning = false;

void resetData()
{
    timerRunning = false;
    data_counter = -1;
}

/**
 * This is responsible for communication with the bluetooth module
 */
void bluetooth_settings()
{
    if (timerRunning && ((millis() - timeStart) >= 500))
    {
        resetData();
        data_counter++;
    }

    // Check if there is data available and it is not 10 (since 10) always gets send right after data
    if (Serial.available())
    {
        uint8_t b = Serial.read();

        // check if this is the first byte sent
        if (data_counter == 0)
        {
            timeStart = millis();
            timerRunning = true;
            setMode(b);
        }
        else if (data_counter == 1)
            setDataBrightness(b);
        else
            modeSpecific(b);

        Serial.print(b);
        Serial.print("\t");
        Serial.println(data_counter);

        // update the data_counter
        data_counter++;
    }
}

// this will set the mode to the incoming byte
void setMode(uint8_t b)
{
    if (b >= 0 && b < 4)
        mode = b;

    FastLED.clear(); // Reset the leds
}

void modeSpecific(uint8_t b)
{
    if (mode == 0)
        btSoundReactive(b);
    else if (mode == 1)
        btStarMode(b);
    else if (mode == 2)
        btStaticMode(b);
    else if (mode == 3)
        btColorCycleMode(b);
}

// Mode specific code for sound reactive
void btSoundReactive(uint8_t b)
{
    if (data_counter == 2)
    {
        setReactiveMode(b);
        if (b == 0)
            resetData();
    }
    else if (data_counter == 3)
    {
        setReactiveRed(b);
    }
    else if (data_counter == 4)
    {
        setReactiveGreen(b);
    }
    else if (data_counter == 5)
    {
        setReactiveBlue(b);
        resetData();
    }
}

// Mode specific code for the star mode
void btStarMode(uint8_t b)
{
    byte_buffer[data_counter - 2] = b;

    if (data_counter == 3)
    {
        setStarCount(byte_buffer[0]);
        setStarSpeed(byte_buffer[1]);
        resetData();
    }
}

// Mode specific code for the static color mode
void btStaticMode(uint8_t b)
{
    byte_buffer[data_counter - 2] = b;

    if (data_counter == 4)
    {
        setStaticRed(byte_buffer[0]);
        setStaticGreen(byte_buffer[1]);
        setStaticBlue(byte_buffer[2]);
        resetData();
    }
}

// Mode specific code for the color cycling mode
void btColorCycleMode(uint8_t b)
{
    if (data_counter == 2)
    {
        setCycleSpeed(b);
        resetData();
    }
}

// set the brightness of the to the incoming byte
void setDataBrightness(uint8_t b)
{
    brightness = b;

    FastLED.setBrightness(brightness);
}
