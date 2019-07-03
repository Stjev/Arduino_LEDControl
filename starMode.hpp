#ifndef STAR_HEADER
#define STAR_HEADER

#include <stdint.h>

#define STARCOUNT_PER_STRIP 20 // The amount of stars displayed at a time

class Star
{
public:
    uint8_t count = 20;
    uint16_t speed = 30000;
};

class StarStatus
{
public:
    int16_t active_led;       // The led active
    int brightness;           // The brightness of this led
    int16_t brightness_speed; // The speed the led is getting brighter
};

void initialize_stars();
void starmode();
void setStarSpeed(uint8_t speed);
void setStarCount(uint8_t count);

#endif
