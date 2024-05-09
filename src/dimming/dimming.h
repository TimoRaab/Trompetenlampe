
#ifndef dimming_H
#define dimming_H

#include <cstdint>


void switchOnOff();
void lumIncrease();
void lumDecrease();
void changePWMDuty(uint16_t duty);
int16_t coerceDimming(int16_t tempAnalogDuty);
void changeDimming(int8_t dir);

#endif
//EOF