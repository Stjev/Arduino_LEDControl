#include "starMode.hpp"
#include <FastLED.h>

Star starm;
uint8_t last_count = 20;

StarStatus *dots1 = (StarStatus *)malloc(sizeof(StarStatus) * starm.count);
StarStatus *dots2 = (StarStatus *)malloc(sizeof(StarStatus) * starm.count);

/**
 * Initialize the start to not be assigned
 */
void initialize_stars()
{
	dots1 = (StarStatus *)realloc(dots1, sizeof(StarStatus) * starm.count);
	dots2 = (StarStatus *)realloc(dots2, sizeof(StarStatus) * starm.count);

	// This is a fail safe for whenever the memory could not be allocated. The previous (working size will be set)
	if (dots1 == NULL || dots2 == NULL)
	{
		starm.count = last_count;
		dots1 = (StarStatus *)realloc(dots1, sizeof(StarStatus) * starm.count);
		dots2 = (StarStatus *)realloc(dots2, sizeof(StarStatus) * starm.count);
	}

	for (int i = 0; i < starm.count / 2; i++)
	{
		newRandomStar(i, 1);
		newRandomStar(i, 2);
	}
}

/**
 * Generates random stars for the starmode
 */
void newRandomStar(uint8_t pos, uint8_t strip)
{
	StarStatus &led = (strip == 1) ? dots1[pos] : dots2[pos];

	led.active_led = random(pos * NUM_LEDS / starm.count, min((pos + 1) * NUM_LEDS / starm.count, NUM_LEDS - 1));
	led.brightness = 1;
	led.brightness_speed = random(32, 96);
}

void setStarCount(uint8_t count)
{
	last_count = starm.count;
	starm.count = count;
	initialize_stars();
}

void setStarSpeed(uint8_t speed)
{
	starm.speed = map(speed, 20, 100, 32000, 15000);
	Serial.println("SPEED:");
	Serial.println(starm.speed);
}

/**
 * Control the leds
 */
void starmode()
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		leds1[i] = CRGB::Black;
		leds2[i] = CRGB::Black;
	}

	for (uint8_t i = 0; i < starm.count; i++)
	{
		starLogic(i, 1);
		starLogic(i, 2);
	}

	FastLED.show();
}

void starLogic(uint8_t i, uint8_t strip)
{
	StarStatus &star = (strip == 1) ? dots1[i] : dots2[i];

	// Generate a star
	if (star.active_led == -1)
		newRandomStar(i, strip);
	else
	{
		// The led is at its max brightness and it is not yet going down
		if (star.brightness >= starm.speed && star.brightness_speed > 0)
			star.brightness_speed *= -1;

		star.brightness = min(starm.speed, max(0, star.brightness + star.brightness_speed));

		CRGB color = CRGB(0, 0, map(star.brightness, 0, starm.speed, 0, brightness));

		if (strip == 1)
			leds1[star.active_led] = color;
		else if (strip == 2)
			leds2[star.active_led] = color;

		// The led dimmed out, reset it
		if (star.brightness < 1)
			newRandomStar(i, strip);
	}
}
