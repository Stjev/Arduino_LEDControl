/**
   * Leds
   */
#include <FastLED.h>
#include "starMode.hpp"
#include "soundReactive.hpp"
#include "staticMode.hpp"

#define NUM_LEDS 150
#define DATA_PIN_1 3
#define DATA_PIN_2 6

struct CRGB leds1[NUM_LEDS];
struct CRGB leds2[NUM_LEDS];

uint8_t brightness = 255;

/**
  * 0 = Sound reactive mode
  * 1 = Star mode
  * 2 = Static color mode
  * 3 = Color cycle mode
  */
uint8_t mode = 1;

/**
   * Runs only once when the arduino starts
   */
void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN_1, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2812, DATA_PIN_2, GRB>(leds2, NUM_LEDS);
  FastLED.setBrightness(brightness);

  initialize_stars();
  initialize_shield();

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
    soundReactiveMode();
  // Mode 2: Star mode
  else if (mode == 1)
    starmode();
  else if (mode == 2)
    staticMode();
  else if (mode == 3)
    colorCycleMode();
}
