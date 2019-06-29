#ifndef STAR_HEADER
#define STAR_HEADER

#include <stdint.h>

#define STARCOUNT_PER_STRIP 20 // The amount of stars displayed at a time
#define SPEED 30000            // The speed the stars get brighter / dimmer, lower is faster

class StarStatus
{
public:
    int16_t active_led;      // The led active
    int brightness;          // The brightness of this led
    int16_t brightness_speed; // The speed the led is getting brighter
};

void initialize_stars();
void starmode();

#endif
