#include <Arduino.h>
#include "LEDSoundmeter.h"

LedMeter::LedMeter(float new_min_value, float new_max_value, int new_pins[])
{
  // Copy pins, no fault checking
  for (int i = 0; i < 8; i++)
  {
    LedMeter::pins[i] = new_pins[i];
  }

  pinMode(LedMeter::pins[0], OUTPUT);
  pinMode(LedMeter::pins[1], OUTPUT);
  pinMode(LedMeter::pins[2], OUTPUT);
  pinMode(LedMeter::pins[3], OUTPUT);
  pinMode(LedMeter::pins[4], OUTPUT);
  pinMode(LedMeter::pins[5], OUTPUT);
  pinMode(LedMeter::pins[6], OUTPUT);
  pinMode(LedMeter::pins[7], OUTPUT);
  LedMeter::max_value = new_max_value;
  LedMeter::min_value = new_min_value;
}

LedMeter::LedMeter(float new_min_value, float new_max_value)
{
  LedMeter(min_value, max_value, LedMeter::pins);
}

void LedMeter::write_value(float value)
{
  // Convert the level to be between 0 and 8
  int leds_to_turn_on = (int)ceil((value / max_value) * 8);
  leds_to_turn_on -= 1;
  if (value == -1.0)
  {
    leds_to_turn_on = 0;
  }
  // Turn on whatever LED's needs to be turned on
  // Fall-through is expected and desired.
  switch (leds_to_turn_on)
  {
  default:
    // Number is expected to be above 8, turn on all LED's
  case 8:
    digitalWrite(LedMeter::pins[7], 1);
  case 7:
    digitalWrite(LedMeter::pins[6], 1);
  case 6:
    digitalWrite(LedMeter::pins[5], 1);
  case 5:
    digitalWrite(LedMeter::pins[4], 1);
  case 4:
    digitalWrite(LedMeter::pins[3], 1);
  case 3:
    digitalWrite(LedMeter::pins[2], 1);
  case 2:
    digitalWrite(LedMeter::pins[1], 1);
  case 1:
    digitalWrite(LedMeter::pins[0], 1);
    break;
  case 0:
    break;
  }
  // Turn OFF whatever LEDs need to be turned off
  switch (leds_to_turn_on)
  {
  case 0:
    digitalWrite(LedMeter::pins[0], 0);
  case 1:
    digitalWrite(LedMeter::pins[1], 0);
  case 2:
    digitalWrite(LedMeter::pins[2], 0);
  case 3:
    digitalWrite(LedMeter::pins[3], 0);
  case 4:
    digitalWrite(LedMeter::pins[4], 0);
  case 5:
    digitalWrite(LedMeter::pins[5], 0);
  case 6:
    digitalWrite(LedMeter::pins[6], 0);
  case 7:
    digitalWrite(LedMeter::pins[7], 0);
    break;
  default:
    // Number is expected to be above 8, don't turn off LED's
    break;
  }
}

float LedMeter::get_max_value()
{
  return LedMeter::max_value;
}

void LedMeter::set_max_value(float new_max_value)
{
  this->max_value = new_max_value;
}
void LedMeter::set_min_value(float new_min_value)
{
  this->min_value = new_min_value;
}

LedMeter::~LedMeter()
{
}

// Create class for the LED control, using new pins and minmax values 0-8
int reverseLEDpins[8] = {13, 12, 11, 10, 9, 8, 7, 6}; // Reverse order pins
LedMeter earMeter(0, 8);                              // Use this for min 0, max 8, default pins
// LedMeter earMeter(0, 8, reverseLEDpins);
unsigned long LED_last_updated = 0;
unsigned long LED_now = 0;
unsigned long LED_update_rate = 500; // ms
int cycle_value = 0;

void setupLED()
{
  earMeter.write_value(8);
}

void loopLED()
{
  LED_now = millis();
  if (LED_now - LED_last_updated > LED_update_rate)
  {
    LED_last_updated = millis();
    earMeter.write_value(cycle_value);
    cycle_value++;
    if (cycle_value > 8)
    {
      cycle_value = 0;
    }
  }
}

float getLedMaxValue()
{
  return earMeter.get_max_value();
}

void writeLedCurrentValue(float current_value)
{
  earMeter.write_value(current_value);
}