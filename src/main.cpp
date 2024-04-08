#include <Arduino.h>
#include <Esp.h>
#include <Ticker.h>
#include <ButtonDebounce.h>
#include <Preferences.h>


#define FIRSTPRESS 0
#define LONGPRESSFIRST 1
#define LONGPRESSSECOND 2

#define WAITTIMEFIRST 1000
#define WAITTIMESECOND 500


void IRAM_ATTR updateButtons();
void saveAnalogDuty();
void switchOnOff();
void lumIncrease();
void lumDecrease();

Ticker buttonReader;
Ticker dutySave;
Preferences pref;
ButtonDebounce b_onOff = ButtonDebounce(14, true, false, *switchOnOff);
ButtonDebounce b_decrease = ButtonDebounce(13, true, false, *lumDecrease);
ButtonDebounce b_increase = ButtonDebounce(12, true, false, *lumIncrease);



const int ledPin = 16; 

bool onOff = true;
uint8 b_onOff_State = FIRSTPRESS;
uint8 b_decrease_State = FIRSTPRESS;
uint8 b_increase_State = FIRSTPRESS;
uint32 b_decrease_time = 0;
uint32 b_increase_time = 0;

uint8 analogDuty = 125;

void setup() {
  Serial.begin(115200);
  analogWriteFreq(4000);

  buttonReader.attach_ms(15, updateButtons);
  dutySave.attach(30000, saveAnalogDuty);
  onOff = true;

  pref.begin("duty", false);
  analogDuty = pref.getUChar("analogD", 100);
  analogWrite(ledPin, analogDuty);
}

void loop() {
  if (!b_onOff.anyPressed(true)) {
    b_onOff_State = FIRSTPRESS;
  }
  if (!b_decrease.anyPressed(true)) {
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


void switchOnOff() {
  if (b_onOff_State == LONGPRESSFIRST) return;
  b_onOff_State = LONGPRESSFIRST;
  uint8 tempAnalog = analogDuty;
  if (onOff) {
    tempAnalog = 0;
  } 
  onOff = !onOff;
  analogWrite(ledPin, tempAnalog);
}


void lumIncrease() {
  if (!onOff) {
    switchOnOff();
    return;
  }
  if (analogDuty == 255) return;
  if (b_increase_State == FIRSTPRESS) {
    if (analogDuty == 0) {
      analogDuty = 15;
    } else {
      analogDuty += 10;
    }
    analogWrite(ledPin, analogDuty);
    b_increase_time = millis();
    b_increase_State = LONGPRESSFIRST;
    return;
  }
  
  if (b_increase_State == LONGPRESSFIRST) {
    if (millis() > (b_increase_time + WAITTIMEFIRST)) {
      analogDuty += 10;
      analogWrite(ledPin, analogDuty);
      b_increase_time = millis();
      b_increase_State = LONGPRESSSECOND;
      return;
    } 
  }

  if (b_increase_State == LONGPRESSSECOND) {
    if (millis() > (b_increase_time + WAITTIMESECOND)) {
      analogDuty += 10;
      analogWrite(ledPin, analogDuty);
      b_increase_time = millis();
      b_increase_State = LONGPRESSSECOND;
      return;
    } 
  }
}


void lumDecrease() {
  Serial.println(b_decrease_State);
  if (!onOff) {
    switchOnOff();
    return;
  }
  if (analogDuty == 0) return;
  if (b_decrease_State == FIRSTPRESS) {
    if (analogDuty == 15) {
      analogDuty = 0;
    } else {
      analogDuty -= 10;
    }
    analogWrite(ledPin, analogDuty);
    b_decrease_time = millis();
    b_decrease_State = LONGPRESSFIRST;
    return;
  }
  
  if (b_decrease_State == LONGPRESSFIRST) {
    if (millis() > (b_decrease_time + WAITTIMEFIRST)) {
      if (analogDuty == 15) {
        analogDuty = 0;
      } else {
        analogDuty -= 10;
      }
      analogWrite(ledPin, analogDuty);
      b_decrease_time = millis();
      b_decrease_State = LONGPRESSSECOND;
      return;
    } 
  }

  if (b_decrease_State == LONGPRESSSECOND) {
    if (millis() > (b_decrease_time + WAITTIMESECOND)) {
      if (analogDuty == 15) {
        analogDuty = 0;
      } else {
        analogDuty -= 10;
      }
      analogWrite(ledPin, analogDuty);
      b_decrease_time = millis();
      b_decrease_State = LONGPRESSSECOND;
      return;
    } 
  }
}


void saveAnalogDuty() {
  pref.putUChar("analogD", analogDuty);
}
//EOF