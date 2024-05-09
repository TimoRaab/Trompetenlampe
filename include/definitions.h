//Definitions for Lampete

#ifndef definitions_H
#define definitions_H

#define FIRSTPRESS 0
#define LONGPRESSWAIT 1
#define LONGPRESSACTION 2

#define WAITTIMEDIM 1000



#define PWMRESOLUTION   8                                   //in bits
#define DIMRATE         (0.078125*(pow(2, PWMRESOLUTION)))  //per second
#define DIMLOWERLIMIT   (0.00390625*pow(2, PWMRESOLUTION))  //lowest dimming limit
#define DIMHIGHLIMIT    (pow(2, PWMRESOLUTION)-1)           //Highest pwm duty cycle
#define FIRSTDIMSTEP    (0.03125*(pow(2, PWMRESOLUTION)))   //Dimmming step when only pressing the button once

#endif
//EOF