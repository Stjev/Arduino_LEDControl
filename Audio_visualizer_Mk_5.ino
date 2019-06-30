/**
   * Leds
   */
#include <FastLED.h>
#include "starMode.hpp"
#include "soundReactive.hpp"

#define STROBE 12
#define RESET 13
#define NUM_LEDS 150
#define DATA_PIN_1 5
#define DATA_PIN_2 6
#define SENSITIVITY 15 // How sensitive are the leds
#define MAX_DATA_COUNT 1

// COLOR CYCLE MODE
#define CYCLESPEED 0.5

struct CRGB leds1[NUM_LEDS];
struct CRGB leds2[NUM_LEDS];

/**
   * Custom
   */

int brightness = 255;

/**
  * 0 = Sound reactive mode
  * 1 = Star mode
  * 2 = Static color mode
  * 3 = Color cycle mode
  */
uint8_t mode = 0;

uint8_t data_counter = 0;

float red = 255.0, green = 0.0, blue = 0.0;

/**
   * Star leds
   */

/**
   * Static
   */
//CRGB color = CRGB(140, 40, 0); //orange
//CRGB color = CRGB(255, 96, 33);
CRGB color = CRGB::White;

/**
   * Runs only once when the arduino starts
   */
void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN_1, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2812, DATA_PIN_2, GRB>(leds2, NUM_LEDS);
  FastLED.setBrightness(brightness);

  initialize_stars();

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

  Serial.begin(9600);
}

/**
   * Infinitely loops
   */
void loop()
{
  bluetooth_settings();

  // Mode 1: The leds react to the music
  if (mode == 0)
  {
    soundReactive();
  }
  // Mode 2: Star mode
  else if (mode == 1)
  {
    starmode();
  }
  else if (mode == 2)
  {
    staticMode();
  }
  else if (mode == 3)
  {
    colorCycleMode();
  }
}

/**
   * This is responsible for the communication with the bluetooth module
  */
void bluetooth_settings()
{
  // Check if there is data available and it is not 10 (since 10) always gets send right after data
  if (Serial.available())
  {
    int b = Serial.read();

    // check if this is the first byte sent
    if (data_counter == 0)
    {
      setMode(b);
    }
    else if (data_counter == 1)
    {
      setDataBrightness(b);
    }

    // update the data_counter
    if (data_counter < MAX_DATA_COUNT)
    {
      data_counter++;
    }
    else
    {
      data_counter = 0; // reset the datacounter
    }

    Serial.println(b);
  }
}

// this will set the mode to the incoming byte
void setMode(int b)
{
  if (b >= 0 && b < 4)
  {
    mode = (b & 0xFF);
  }

  resetLEDS();
}

// set the brightness of the to the incoming byte
void setDataBrightness(int b)
{
  brightness = b;

  FastLED.setBrightness(brightness);
}

void resetLEDS()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds1[i] = CRGB::Black;
    leds2[i] = CRGB::Black;
  }
}

/**
   * Static Mode
   */
void staticMode()
{
  x for (int i = 0; i < NUM_LEDS; i++)
  {
    leds1[i] = color;
    leds2[i] = color;
  }
  FastLED.show();
}

/**
    * Color cycle Mode
    */
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
