//Source file for the LED sound meter. This class sets and writed the LED color bar
//This source file also calculates the number of LEDs corresponding to the noise level in dB

#include <Arduino.h>
#include "LEDSoundmeter.h"

LedMeter::LedMeter(float new_min_value, float new_max_value, int new_pins[])
{
  // Copy pins, no fault checking
  for (int i = 0; i < 8; i++)
  {
    LedMeter::pins[i] = new_pins[i];
  }
//LED pints are set as an output
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
  LED_max_fs = 0;
  LED_min_fs = UINT16_MAX;
  LED_last_sample_time = 0;
}

LedMeter::LedMeter(float new_min_value, float new_max_value)
{
  LedMeter(min_value, max_value, LedMeter::pins);
}

void LedMeter::write_value(float value)
{
  int leds_to_turn_on;

  // Timing
  if (measure_LED_time)
  {
    LED_last_sample_time = micros();
  }

  // Convert the level to be between 0 and 8

  if (value == -1.0)
  {
    leds_to_turn_on = 0;
  }
  else
  {
    leds_to_turn_on = (int)ceil(((value - this->min_value) / (this->max_value - this->min_value)) * 8);
    leds_to_turn_on -= 1;
  }

  // Handle the edge case of underflow
  if (leds_to_turn_on < 0)
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
  if (measure_LED_time == true) // The ""== true" is for readability purposes
  {
    // Timing measurements, measures the time since the last update of LED_last_sample_time
    unsigned long current_fs;
    current_fs = micros() - LED_last_sample_time; // Calculate sample time
    if (current_fs > LED_max_fs)
    {
      LED_max_fs = current_fs;
    }
    else if (current_fs < LED_min_fs)
    {
      LED_min_fs = current_fs;
    }
  //Print to serial. Used for debugging and timing measurement
    Serial.print("Timing, LED, min: ");
    Serial.print(LED_min_fs);
    Serial.print("; max: ");
    Serial.print(LED_max_fs);
    Serial.print("; Current: ");
    Serial.println(current_fs);
  }
}

float LedMeter::get_max_value()
{
  return LedMeter::max_value; //Get the max value to the LED
}

void LedMeter::set_max_value(float new_max_value)
{
  this->max_value = new_max_value;
}
void LedMeter::set_min_value(float new_min_value)
{
  this->min_value = new_min_value;
}

float LedMeter::get_max_value_dB()
{
  return this->max_value * 0.0095 - 114.57; //Calculate max value in dB
}

void LedMeter::set_max_value_dB(float new_max_value_dB)
{
  this->max_value = 105 * new_max_value_dB + 12030;  //Calculate max value in dB
}

LedMeter::~LedMeter()
{
}

// Create class for the LED control, using new pins and minmax values 0-8
int reverseLEDpins[8] = {13, 12, 11, 10, 9, 8, 7, 6}; // Reverse order pins
int LEDpins[8] = {6, 7, 8, 9, 10, A6, 14, 13}; //One analog A6 is used, since 12 and 13 is reserved for WiFi
LedMeter earMeter(0, 8, LEDpins); // Use this for min 0, max 8, default pins
// LedMeter earMeter(0, 8, reverseLEDpins);
unsigned long LED_last_updated = 0;
unsigned long LED_now = 0;
unsigned long LED_update_rate = 500; // ms, updates every 500 ms
int cycle_value = 0;

void setupLED()
{
  earMeter.write_value(8); //Set the number of LEDs
}

void loopLED()
{
  //Function to update the LEDs
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
  return earMeter.get_max_value(); //Get the max LED value 
}

void writeLedCurrentValue(float current_value)
{
  earMeter.write_value(current_value); //Set the current LED 
}