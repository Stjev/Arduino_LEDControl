#ifndef STATIC_COLOR_HEADER
#define STATIC_COLOR_HEADER

class Static
{
public:
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 255;
};

void staticMode();
void setStaticRed(uint8_t red);
void setStaticGreen(uint8_t green);
void setStaticBlue(uint8_t blue);

#endif