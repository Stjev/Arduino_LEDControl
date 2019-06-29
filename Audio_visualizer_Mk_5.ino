/**
   * Leds
   */
#include <FastLED.h>
#include "starMode.hpp"

#define STROBE 12
#define RESET 13
#define DC_One A0
#define DC_Two A1
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
float bass = 0;

int frequencies_One[7];
int frequencies_Two[7];

int brightness = 255;

/**
   * 0 = Sound reactive mode
   * 1 = Star mode
   * 2 = Static color mode
   * 3 = Color cycle mode
   */
uint8_t mode = 1;

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

// Gets the avarage of bands
float getAverageTone(int from, int to, char data[])
{
  float sum = 0;

  for (int i = from; i <= to; i++)
  {
    sum += data[i] * SENSITIVITY;
  }

  return sum / (to - from + 1);
}

/**
   * Static Mode
   */
void staticMode()
{
  for (int i = 0; i < NUM_LEDS; i++)
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
