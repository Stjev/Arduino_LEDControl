#ifndef BLUETOOTH_HEADER
#define BLUETOOTH_HEADER

#define MAX_DATA_COUNT 1

#include <stdint.h>

void bluetooth_settings();
void setMode(uint8_t b);
void setDataBrightness(uint8_t b);

#endif