#ifndef SOUND_REACTIVE_HEADER
#define SOUND_REACTIVE_HEADER

#define DC_One A0
#define DC_Two A1
#define STROBE 12
#define RESET 13
#define CHANNEL_COUNT 7           // The channel count
#define LEFT_RIGHT_CHANNEL_MODE 0 // 0 = average, 1 = max
#define SENSITIVITY 3             // Lower = more sensitive
#define RETRACT_RATE 50           // The speed the leds go down, higher = faster
#define THRESHOLD 23              // The threshold for the leds to start reacting to music

class SoundReactive
{
public:
    // 0 = reaction is the amount of leds
    // 1 = reaction is the brightness of the leds (all leds are on)
    uint8_t reactive_mode = 0;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 255;
};

void initialize_shield();
void soundReactiveMode();
void drawLeds();
void setReactiveMode(uint8_t mode);
void setReactiveRed(uint8_t red);
void setReactiveGreen(uint8_t green);
void setReactiveBlue(uint8_t blue);

#endif