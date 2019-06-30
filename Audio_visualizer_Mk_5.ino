/**
   * Leds
   */
#include <FastLED.h>
#include "starMode.hpp"
#include "soundReactive.hpp"
#include "staticMode.hpp"

#define STROBE 12
#define RESET 13
#define NUM_LEDS 150
#define DATA_PIN_1 5
#define DATA_PIN_2 6
#define SENSITIVITY 15 // How sensitive are the leds

struct CRGB leds1[NUM_LEDS];
struct CRGB leds2[NUM_LEDS];

uint8_t brightness = 255;

/**
  * 0 = Sound reactive mode
  * 1 = Star mode
  * 2 = Static color mode
  * 3 = Color cycle mode
  */
uint8_t mode = 0;

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