#include <Arduino.h>
#include <Esp.h>
#include <Ticker.h>
#include <ButtonDebounce.h>
#include <Preferences.h>

#include "definitions.h"
#include "variables.h"

#include ".\dimming\dimming.h"


void IRAM_ATTR updateButtons();
void saveAnalogDuty();

Ticker buttonReader;
Ticker dutySave;
Preferences pref;

#ifdef boardEsp8266
  #define onOffPin 14
  #define decreasePin 13
  #define increasePin 12
  #define ledPin 16
#endif
#ifdef boardEsp32
  #define onOffPin 3
  #define decreasePin 4
  #define increasePin 5
  #define ledPin 2
  #define pwmChannel 0
#endif



ButtonDebounce b_onOff = ButtonDebounce(onOffPin, true, false, *switchOnOff);
ButtonDebounce b_decrease = ButtonDebounce(decreasePin, true, false, *lumDecrease);
ButtonDebounce b_increase = ButtonDebounce(increasePin, true, false, *lumIncrease);
const int pwmPin = ledPin; 

bool onOff = true;
uint8_t b_onOff_State = FIRSTPRESS;
uint8_t b_decrease_State = FIRSTPRESS;
uint8_t b_increase_State = FIRSTPRESS;
int16_t analogDuty = 125;        //Just for first starts

void setup() {
  Serial.begin(115200);

  buttonReader.attach_ms(15, updateButtons);
  dutySave.attach(30000, saveAnalogDuty);
  onOff = true;

  pref.begin("duty", false);
  analogDuty = pref.getUChar("analogD", 100);

  #ifdef boardEsp8266
    analogWriteFreq(4000);
  #endif
  
  #ifdef boardEsp32
    ledcSetup(0, 5000, PWMRESOLUTION);  //use 8 bit for now
    ledcAttachPin(pwmPin, 0);
  #endif

  changePWMDuty(analogDuty);
}

void loop() {
  if (!b_onOff.anyPressed(true)) {  //switch on off function
    b_onOff_State = FIRSTPRESS;
  }
  if (!b_decrease.anyPressed(true)) { //
    b_decrease_State = FIRSTPRESS;
  }
  if (!b_increase.anyPressed(true)) {
    b_increase_State = FIRSTPRESS;
  }
}



void IRAM_ATTR updateButtons() {
  b_onOff.updateButton();
  b_decrease.updateButton();
  b_increase.updateButton();
}





void saveAnalogDuty() {
  pref.putUChar("analogD", analogDuty);
}



//EOF