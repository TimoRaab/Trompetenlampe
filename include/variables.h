//Variables for Lampete

#ifndef variables_H
#define variables_H

#include <cstdint>

/// @brief Current state of lamp.
extern bool onOff;
/// @brief Current state of state machine for on-off buttton.
extern uint8_t b_onOff_State;
/// @brief Current state of state machine for decrease buttton.
extern uint8_t b_decrease_State;
/// @brief Current state of state machine for increase buttton.
extern uint8_t b_increase_State;
/// @brief variable for state machine behavior. Saves time of last relevant action for decrease luminescence.
extern uint32_t b_decrease_Time;
/// @brief variable for state machine behavior. Saves time of last relevant action for increase luminescence.
extern uint32_t b_increase_Time;
/// @brief Current duty cycle of lamp. Only for on-state!
extern int16_t analogDuty;

#endif
//EOF