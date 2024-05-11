
#ifndef dimming_H
#define dimming_H

#include <cstdint>

/**
 * @brief Switches the lamps on and off.
 * 
 * The function switches the duty cycle to 0 (off) or the old duty cycle value (on).
 */
void switchOnOff();

/**
 * @brief Increases the luminescence.
 * 
 * @note Calls changeDimming with a positive value (+1). 
 */
void lumIncrease();

/**
 * @brief Decreases the luminescence.
 * 
 * @note Calls changeDimming with a negative value (-1).
 */
void lumDecrease();

/**
 * @brief State machine for deciding the dimming behavior when a button is pressed.
 * 
 * @param dir dimming direction. Positive values increase the luminescence, negative decrease
 */
void changeDimming(int8_t dir);

/**
 * @brief  Changes the duty cycle for the PWM signal for the lamp. 
 * 
 * @param duty duty cycle for PWM signal
 * @note Coerces the duty cycle in a range. 
 */
int16_t changePWMDuty(uint16_t duty, boolean coerce = true);

/**
 * @brief Coerces the duty value in a predefined range.
 * 
 * @param tempAnalogDuty duty cycle to coerce
 * @return int16_t coerced duty cycle
 */
int16_t coerceDimming(int16_t tempAnalogDuty);


#endif
//EOF