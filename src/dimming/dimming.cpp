#include <Esp.h>

#include "definitions.h"
#include "variables.h"
#include "dimming.h"


uint32_t b_decrease_Time = 0;
uint32_t b_increase_Time = 0;



//Button functions
void switchOnOff() {
  if (b_onOff_State == LONGPRESSWAIT) return;
  b_onOff_State = LONGPRESSWAIT;
  uint16_t tempAnalog = analogDuty;
  if (onOff) {
    tempAnalog = 0;
  } 
  onOff = !onOff;
  changePWMDuty(tempAnalog);
}
void lumIncrease() {
    changeDimming(+1);
}
void lumDecrease() {
    changeDimming(-1);
}

//PWM functions
void changePWMDuty(uint16_t duty) {
    duty = coerceDimming(duty);
    #ifdef boardEsp8266
        analogWrite(pwmPin, duty);
    #endif
    #ifdef boardEsp32
        ledcWrite(0, duty);
    #endif
}
int16_t coerceDimming(int16_t tempAnalogDuty) {
    if (tempAnalogDuty > DIMHIGHLIMIT) {
        return DIMHIGHLIMIT;
    }
    if (tempAnalogDuty < DIMLOWERLIMIT) {
        return DIMLOWERLIMIT;
    }
    return tempAnalogDuty;
}


//Dimming
void changeDimming(int8_t dir) {

    //Switch on if the lamp is off
    if (!onOff) {
        switchOnOff();
        return;
    }
    
    //Initialize pointers to the according button values
    uint8_t *pointer2State;
    uint32_t *pointer2Time;
    switch (dir) {
        case 0:
            return;
        case 1:
            pointer2State = &b_increase_State;
            pointer2Time = &b_increase_Time;
            break;
        case -1:
            pointer2State = &b_decrease_State;
            pointer2Time = &b_decrease_Time;
            break;
    }

    //Action to first press
    if (*pointer2State == FIRSTPRESS) {
        analogDuty += (dir*FIRSTDIMSTEP);
        changePWMDuty(analogDuty);
        *pointer2Time = millis();
        *pointer2State = LONGPRESSWAIT;
        return;
    }
    
    //Check for longpresses, switch if WAITTIMEDIM is over
    if (*pointer2State == LONGPRESSWAIT) {
        if (millis() > (*pointer2Time + WAITTIMEDIM)) {
            *pointer2Time = millis();
            *pointer2State = LONGPRESSACTION;
            return;
        }
    }

    //dim lamp according to dimrate
    if (*pointer2State == LONGPRESSACTION) {
        uint32_t timePassed = millis() - *pointer2Time;
        *pointer2Time = millis();
        analogDuty += dir*(timePassed/1000*DIMRATE);
        return;
    }
}
//EOF