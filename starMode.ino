#include "starMode.hpp"
#include <FastLED.h>

StarStatus dots1[STARCOUNT_PER_STRIP];
StarStatus dots2[STARCOUNT_PER_STRIP];

/**
 * Initialize the start to not be assigned
 */
void initialize_stars()
{
  for (int i = 0; i < STARCOUNT_PER_STRIP / 2; i++)
  {
    dots1[i].active_led = -1;
    dots2[i].active_led = -1;
  }
}

/**
 * Generates random stars for the starmode
 */
void newRandomStar(uint8_t pos, uint8_t strip)
{
  StarStatus& led = (strip == 1)? dots1[pos] : dots2[pos];

  led.active_led = random(pos * NUM_LEDS / STARCOUNT_PER_STRIP, min((pos + 1) * NUM_LEDS / STARCOUNT_PER_STRIP, NUM_LEDS - 1));
  led.brightness = 1;
  led.brightness_speed = random(50, 150);
}

/**
 * Control the leds
 */
void starmode()
{
  for (uint8_t i = 0; i < STARCOUNT_PER_STRIP; i++)
  {
    starLogic(i, 1);
    starLogic(i, 2);
  }

  FastLED.show();
}

void starLogic(uint8_t i, uint8_t strip)
{
  StarStatus& star = (strip == 1)? dots1[i] : dots2[i];

  // Generate stars for strip 1
  if (star.active_led == -1)
    newRandomStar(i, strip);
  else
  {
    // The led is at its max brightness
    if (star.brightness > SPEED)
      star.brightness_speed *= -1;

    star.brightness += star.brightness_speed;

    CRGB color = CRGB(0, 0, map(star.brightness, 0, SPEED, 0, brightness));

    if (strip == 1)
      leds1[star.active_led] = color;
    else if (strip == 2)
      leds2[star.active_led] = color;

    // The led dimmed out, reset it
    if (star.brightness < 1)
      newRandomStar(i, strip);
  }
}
